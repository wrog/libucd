include MCONFIG

#
# Headers included from libucd_int.h
#
HDRS = libucd_int.h ucd.h int24.h compiler.h

#
# These are the files produced by convert_ucd.pl
#
CVT_FILES = gen/jamo.c gen/nameslist.tab gen/nametoucs.keys gen/nametoucs.tab \
	gen/ucstoname.keys gen/proparray.c gen/proparrayindex

# -----------------------------------------------------------------------

.SUFFIXES: .c .o .lo .s .ls .i .ho .hs .hi .cc .h
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
.c.lo:
	$(CC) $(CFLAGS) $(PICFLAGS) -c -o $@ $<
.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.c.ls:
	$(CC) $(CFLAGS) $(PICFLAGS) -S -o $@ $<
.c.i:
	$(CC) $(CFLAGS) -E -o $@ $<
.c.ho:
	$(HOST_CC) $(HOST_CFLAGS) -c -o $@ $<
.c.hs:
	$(HOST_CC) $(HOST_CFLAGS) -S -o $@ $<
.c.hi:
	$(HOST_CC) $(HOST_CFLAGS) -E -o $@ $<

enums/%.o: enums/%.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<
enums/%.lo: enums/%.c $(HDRS)
	$(CC) $(CFLAGS) $(PICFLAGS) -c -o $@ $<

# -----------------------------------------------------------------------

LIBSRCS = proparray.c gen/nametoucs_hash.c gen/ucstoname_hash.c \
	  gen/jamo.c gen/nameslist.c gen/nameslist_dict.c \
	  gen/ucstoname_tab.c gen/nametoucs_tab.c nametoucs.c \
	  ucslookup.c cache.c gen/ucdversion.c $(wildcard enums/*.c)

LIBOBJS = $(patsubst %.c,%.o,$(LIBSRCS))
SO_OBJS = $(patsubst %.c,%.lo,$(LIBSRCS))

# -----------------------------------------------------------------------

all : $(LIB_FILE) $(SO_FILE) $(SO_NAME) ucd.h

clean:
	rm -rf gen enums
	rm -f *.o *.ho *.lo *.i *.s *.a *.so *.so.* ucd.h
	$(MAKE) -C perfect clean

distclean: clean
	find . -name \*~ -print0 | xargs -0rt rm -f

install: all
	mkdir -p $(INSTALLROOT)$(libdir)
	$(INSTALL_LIB) $(LIB_FILE) $(SO_FILE) $(SO_NAME) $(INSTALLROOT)$(libdir)
	mkdir -p $(INSTALLROOT)$(installdir)
	$(INSTALL_DATA) ucd.h $(INSTALLROOT)$(installdir)
	mkdir -p $(INSTALLROOT)$(mandir)/man3
	$(INSTALL_DATA) *.3 $(INSTALLROOT)$(mandir)/man3

# -----------------------------------------------------------------------

$(CVT_FILES) : gen/done

gen/done: convert_ucd.pl $(wildcard ucd/*.txt ucd/extracted/*.txt)
	mkdir -p gen
	$(PERL) convert_ucd.pl
	touch gen/done

perfect/perfect: $(wildcard perfect/*.c perfect/*.h)
	$(MAKE) -C perfect

gen/nametoucs_hash.c: gen/nametoucs.keys perfect/perfect
	perfect/perfect -im _libucd_nametoucs_hash gen/nametoucs_hash.c \
		gen/nametoucs_hash.h < gen/nametoucs.keys 

gen/nametoucs_hash.h: gen/nametoucs_hash.c
	: Generated by side effect

gen/ucstoname_hash.c: gen/ucstoname.keys perfect/perfect
	perfect/perfect -hm _libucd_ucstoname_hash gen/ucstoname_hash.c \
		gen/ucstoname_hash.h < gen/ucstoname.keys 

gen/ucstoname_hash.h: gen/ucstoname_hash.c
	: Generated by side effect

gen/nameslist.compr: gen/nameslist.tab simplecomp.pl
	$(PERL) simplecomp.pl < $<

gen/nameslist_tab.c gen/nameslist_dict.c gen/nameslist.offset: gen/nameslist.compr
	: Generated by side effect

gen/nameslist.c: gen/nameslist.compr bin2c.pl
	$(PERL) bin2c.pl _libucd_names_list < $< > $@ || rm -f $@

gen/mk_ucstoname_tab: mk_ucstoname_tab.ho gen/ucstoname_hash.ho
	$(HOST_CC) $(HOST_LDFLAGS) -o $@ $^ $(HOST_LIBS)

gen/ucstoname_tab.c: gen/mk_ucstoname_tab \
		     gen/proparrayindex gen/nameslist.offset
	gen/mk_ucstoname_tab

gen/mk_nametoucs_tab: mk_nametoucs_tab.ho gen/nametoucs_hash.ho
	$(HOST_CC) $(HOST_LDFLAGS) -o $@ $^ $(HOST_LIBS)

gen/nametoucs_tab.c: gen/mk_nametoucs_tab gen/nametoucs.tab
	gen/mk_nametoucs_tab

gen/ucdversion.c: ucd/version ucdversion.pl
	$(PERL) ucdversion.pl

$(LIB_FILE): $(LIBOBJS)
	rm -f $(LIB_FILE)
	$(AR) cq $(LIB_FILE) $^
	$(RANLIB) $(LIB_FILE)

$(SO_FILE): $(SO_OBJS)
	$(CC) $(SOFLAGS) -o $(SO_FILE) $^

ifneq ($(SO_NAME),$(SO_FILE))
$(SO_NAME): $(SO_FILE)
	ln -f $(SO_FILE) $(SO_NAME)
endif

ucd.h: ucd.h.in enum.list makeenums.pl
	$(PERL) makeenums.pl

gen/cache.c: gencache.pl
	$(PERL) gencache.pl

# -----------------------------------------------------------------------

proparray.o: proparray.c ucd.h $(HDRS) gen/proparray.c
proparray.lo: proparray.c ucd.h $(HDRS) gen/proparray.c

mk_ucstoname_tab.ho: mk_ucstoname_tab.c gen/ucstoname_hash.h
mk_nametoucs_tab.ho: mk_nametoucs_tab.c gen/nametoucs_hash.h

gen/ucstoname_tab.o: gen/ucstoname_tab.c $(HDRS)
gen/ucstoname_tab.lo: gen/ucstoname_tab.c $(HDRS)

gen/nametoucs_tab.o: gen/nametoucs_tab.c $(HDRS)
gen/nametoucs_tab.lo: gen/nametoucs_tab.c $(HDRS)

gen/nameslist_dict.o: gen/nameslist_dict.c
gen/nameslist_dict.lo: gen/nameslist_dict.c

nametoucs.o: nametoucs.c $(HDRS) gen/nametoucs_hash.h
nametoucs.lo: nametoucs.c $(HDRS) gen/nametoucs_hash.h

ucslookup.o: ucslookup.c $(HDRS) gen/ucstoname_hash.h
ucslookup.lo: ucslookup.c $(HDRS) gen/ucstoname_hash.h

cache.o: cache.c gen/cache.c $(HDRS)
cache.lo: cache.c gen/cache.c $(HDRS)
