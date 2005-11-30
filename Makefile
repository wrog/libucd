CC	     = cc
CFLAGS	     = -g -O -I.
PICFLAGS     = -fPIC
LDFLAGS      =

HOST_CC      = cc
HOST_CFLAGS  = -g -O
HOST_LDFLAGS =
HOST_LIBS    =

#
# These are the files produced by convert_ucd.pl
#
CVT_FILES = gen/jamo.c gen/nameslist.c gen/nametoucs.keys \
	gen/ucstoname.keys gen/proparray.c

# -----------------------------------------------------------------------

.SUFFIXES: .c .o .lo .s .ls .i .li .cc .h
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.c.i:
	$(CC) $(CFLAGS) -E -o $@ $<
.c.lo:
	$(HOST_CC) $(HOST_CFLAGS) -c -o $@ $<
.c.ls:
	$(HOST_CC) $(HOST_CFLAGS) -S -o $@ $<
.c.li:
	$(HOST_CC) $(HOST_CFLAGS) -E -o $@ $<

# -----------------------------------------------------------------------

all : perfect/perfect gen/nametoucs_hash.o gen/ucstoname_hash.o \
	proparray.o


clean:
	rm -rf gen
	rm -f *.o *.a *.so *.so.*
	$(MAKE) -C perfect clean

# -----------------------------------------------------------------------

$(CVT_FILES) : gen/done

gen/done: convert_ucd.pl $(wildcard ucd/*.txt ucd/extracted/*.txt)
	mkdir -p gen
	$(PERL) convert_ucd.pl
	touch gen/done

perfect/perfect: $(wildcard perfect/*.c perfect/*.h)
	$(MAKE) -C perfect

gen/nametoucs_hash.c: gen/nametoucs.keys perfect/perfect
	perfect/perfect -im libucd_nametoucs_hash gen/nametoucs_hash.c \
		gen/nametoucs_hash.h < gen/nametoucs.keys 

gen/nametoucs_hash.h: gen/nametoucs_hash.c
	: Generated by side effect

gen/ucstoname_hash.c: gen/ucstoname.keys perfect/perfect
	perfect/perfect -hm libucd_ucstoname_hash gen/ucstoname_hash.c \
		gen/ucstoname_hash.h < gen/ucstoname.keys 

gen/ucstoname_hash.h: gen/ucstoname_hash.c
	: Generated by side effect

# -----------------------------------------------------------------------

proparray.o: proparray.c ucd.h libucd_int.h gen/proparray.c
