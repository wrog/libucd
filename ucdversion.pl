#!/usr/bin/perl

@vinfo = ();

open(UCDVERSION, '<', 'ucd/version') or die;
$line = <UCDVERSION>;
chomp $line;
close(UCDVERSION);

push @vinfo, unicode_database_version => $line;

open(LIBVERSION,'-|','git describe --tags') or die;
<LIBVERSION> =~ m/libucd-([.0-9]+)-/;
close(LIBVERSION);

push @vinfo, unicode_library_version => $1;

open(GV, '>', 'gen/ucdversion.c') or die;
print  GV "#include \"ucd.h\"\n";

while (@vinfo) {

    my $fname = shift @vinfo;
    my @v = split(/\./, shift @vinfo);

    printf  GV <<'END' , $fname, ($v[0] << 16)+($v[1] << 8)+$v[2];

int %s(void)
{
	return 0x%x;
}
END

}

close(GV);
