#!/usr/bin/perl
#
# Perl script to convert the Unicode Character Database source files
# into data for libucd.
#

use POSIX;

#
# Internally this file uses a hash with the UCS value as key, and
# as data another hash from property name to value.
#

%ucs_props = ();

sub parse_unicode_string($) {
    my ($str) = @_;
    my @str = split(/\s+/, $str, 0);
    my @xv = ();
    my $x;

    foreach $x ( @str ) {
	push(@xv, hex $x);
    }

    return pack("U*", @xv);
}

#
# File to read a UCD file with a list of properties (no names)
#
sub read_separated_file($$$) {
    my ($filename, $proplist, $default) = @_;
    my $fh;
    my $line, @fields, $c0, $c1, $c;
    my $was_first = 0;

    open($fh, '<', $filename) or return 0;
    while ( defined($line = <$fh>) ) {
	chomp $line;
	$line =~ s/\s*(|\#.*)$//;
	@fields = split(/\s*\;\s*/, $line);

	if ( $fields[0] =~ /^([0-9a-f]+)(|..([0-9a-f]+))$/i ) {
	    if ( $was_first ) {
		$c1 = hex $1;
		$was_first = 0;
	    } else {
		$c0 = hex $1;
		$c1 = ($2 eq '') ? $c0 : hex $3;
	    }

	    for ( $c = $c0 ; $c <= $c1 ; $c++ ) {
		my $p, $f;

		$ucs_props{$c} = {} unless ( exists($ucs_props{$c}) );
		
		for ( $f = 1 ; $f < scalar(@fields) ; $f++ ) {
		    my $prop = ${$proplist}[$f-1];
		    if ( defined($prop) ) {
			my $type = substr($prop, 0, 1);
			my $prop = substr($prop, 1);
			my $def = ${$default}[$f-1];
			$def = sprintf("%04X", $c) if ( $def eq '=' );
			my $val = $fields[$f];
			$val = $def if ( $val eq '' );

			if ( $type eq 'b' ) {
			    # Boolean (Y/N)
			    $val = ($val eq 'N') ? 0 : 1;
			} elsif ( $type eq 'p' ) {
			    # Code point
			    $val = hex $val;
			} elsif ( $type eq 's' ) {
			    # String of code points
			    $val = parse_unicode_string($val);
			} elsif ( $type eq '!' ) {
			    # Special case
			    if ( $prop eq 'Name' ) {
				# In UnicodeData.txt, ranges aren't encoded the same way
				# as elsewhere, instead the first and last code point are
				# entered, with <..., first> or <..., last> as the name
				if ( $val =~ /^\<.*, First\>$/ ) {
				    $was_first = 1;
				}
				$val = undef if ( $val =~ /^\<.*\>$/ );
			    } elsif ( $prop eq 'Decomposition' ) {
				$prop = 'Decomposition_Mapping';
				if ( $val =~ /^(\<([a-z0-9]+)\>\s*|)([0-9a-f\s]+)$/i ) {
				    my $dct = $2 || 'canonical';
				    $val = parse_unicode_string($3);

				    ${$ucs_props{$c}}{'Decomposition_Type'} = $dct;
				}
			    } else {
				die "$0: Unknown special: $prop\n";
			    }
			}

			${$ucs_props{$c}}{$prop} = $val;
		    }
		}
	    }
	}
    }
    close($fh);

    return 1;
}

#
# File to read a UCD file with boolean properties
#
sub read_boolean_file($) {
    my ($filename) = @_;
    my $fh;
    my $line, @fields, $c0, $c1, $c;

    open($fh, '<', $filename) or return 0;
    while ( defined($line = <$fh>) ) {
	chomp $line;
	$line =~ s/\s*(|\#.*)$//;
	@fields = split(/\s*\;\s*/, $line);

	if ( $fields[0] =~ /^([0-9A-F]+)(|..([0-9A-F]+))$/ &&
	     $fields[1] ne '' ) {
	    $c0 = hex $1;
	    $c1 = ($2 eq '') ? $c0 : hex $3;

	    for ( $c = $c0 ; $c <= $c1 ; $c++ ) {
		my $p, $f;

		$ucs_props{$c} = {} unless ( exists($ucs_props{$c}) );
		
		${$ucs_props{$c}}{$fields[1]} = 1;
	    }
	}
    }
    close($fh);

    return 1;
}

# UCD numeric values are given as 8-significant figures floating-point
# numbers, but in reality they are all fractions.  This converts a
# floating-point number to a numerator and denominator with just about
# enough fuzz.
#
# Note: the denominator will always be positive, and will always be 1
# if the number is an integer.
sub make_fraction($) {
    my ($v) = @_;
    my $n, $d, $minus;

    return [0, 1] if ( $v == 0 );

    $d = 1;
    while ( 1 ) {
	$n = floor($v*$d + 0.5);
	return [$n, $d] if ( abs($n/$d-$v)/$v < 1e-7 );
	$d++;
    }
}

sub make_jamo_string($) {
    my ($s) = @_;
    my $i, $c;
    my $o = '';

    $o .= "{";
    for ( $i = 0 ; $i < 4 ; $i++ ) {
	$c = substr($s,$i,1);
	$o .= length($c) ? "\'$c\'" : '0';
	$o .= ($i == 3) ? '}' : ',';
    }

    return $o;
}

# This produces tables used to generate the systematic Hangul syllables
sub make_jamo_tables() {
    my $LBase = 0x1100;
    my $VBase = 0x1161;
    my $TBase = 0x11A7;
    my $LCount = 19;
    my $VCount = 21;
    my $TCount = 28;
    my $i;
    my $fh;
    
    # None of the syllables exceed 4 bytes, so let's not waste
    # pointer space that might have to be relocated...

    open($fh, '>', 'gen/jamo.c') or die "$0 cannot create gen/jamo.c";
    print $fh "#include \"libucd_int.h\"\n\n";

    print $fh "const char _libucd_hangul_jamo_l[$LCount][4] = {\n";
    for ( $i = 0 ; $i < $LCount ; $i++ ) {
	printf $fh "\t%s,\n", make_jamo_string(${$ucs_props{$LBase+$i}}{'Jamo_Short_Name'});
    }
    print $fh "};\n";
    print $fh "const char _libucd_hangul_jamo_v[$VCount][4] = {\n";
    for ( $i = 0 ; $i < $VCount ; $i++ ) {
	printf $fh "\t%s,\n", make_jamo_string(${$ucs_props{$VBase+$i}}{'Jamo_Short_Name'});
    }
    print $fh "};\n";
    print $fh "const char _libucd_hangul_jamo_t[$TCount][4] = {\n";
    for ( $i = 0 ; $i < $TCount ; $i++ ) {
	printf $fh "\t%s,\n", make_jamo_string(${$ucs_props{$TBase+$i}}{'Jamo_Short_Name'});
    }
    print $fh "};\n";

    close($fh);
}

# This produces a names list, and stores for each character which index in
# the names list they correspond to.  We use a long names list with indexes
# instead of pointers to avoid fixups, and to conserve space on 64-bit
# architectures.  Sort it in order by keyspace to improve cache locality.
%name_to_index = ();
%name_to_ucs   = ();

sub make_names_list() {
    my $k;
    my $pos = 0;
    my $fh;
    my $col;

    open($fh, '>', 'gen/nameslist.c') or die "$0: Cannot create gen/nameslist.c";

    print $fh "#include \"libucd_int.h\"\n\n";
    print $fh "const char _libucd_names_list[] = {";
    $col = 9999;

    foreach $k ( sort {$a <=> $b} (keys(%ucs_props)) ) {
	print STDERR "Not a number: \"$k\"\n" if ( $k ne ($k+0) );

	my $n = ${$ucs_props{$k}}{'Name'};
	if ( defined($n) ) {
	    if ( defined($name_to_ucs{$n}) ) {
		printf STDERR "WARNING: Name \"%s\" duplicated from U+%04X to U+%04X\n",
		$n, $k, $name_to_ucs{$n};
	    } else {
		$name_to_ucs{$n} = $k;
		$name_to_index{$n} = $pos;
		$pos += length($n) + 1;
		my $i;
		for ( $i = 0 ; $i <= length($n) ; $i++ ) {
		    my $c = substr($n,$i,1);
		    
		    if ( $col >= 72 ) {
			print $fh "\n\t";
			$col = 8;
		    }
		    print $fh ( $c ne '' ) ? " \'$c\'," : "   0,";
		    $col += 5;
		}
	    }
	}
    }
    print $fh "\n};\n";
    close($fh);
}

#
# Produce a list of names for automatic hash table generation.
# This includes the Hangul syllables, but not systematically
# named CJK.
#
sub write_hangul_names($)
{
    my ($fh) = @_;
    my $SBase = 0xAC00;
    my $LBase = 0x1100;
    my $VBase = 0x1161;
    my $TBase = 0x11A7;
    my $LCount = 19;
    my $VCount = 21;
    my $TCount = 28;
    my $SCount = $LCount*$VCount*$TCount;
    my $l, $v, $t;

    for ( $l = 0 ; $l < $LCount ; $l++ ) {
	for ( $v = 0 ; $v < $VCount ; $v++ ) {
	    for ( $t = 0 ; $t < $TCount ; $t++) {
		printf $fh "HANGUL SYLLABLE %s%s%s\n",
		${$ucs_props{$LBase+$l}}{'Jamo_Short_Name'},
		${$ucs_props{$VBase+$v}}{'Jamo_Short_Name'},
		${$ucs_props{$TBase+$t}}{'Jamo_Short_Name'};
	    }
	}
    }
}

sub make_name_keyfile()
{
    my $fh;
    my $k;

    open($fh, '>', 'gen/nametoucs.keys')
	or die "$0: cannot write gen/nametoucs.keys\n";

    foreach $k ( keys(%name_to_ucs) ) {
	printf $fh "%s\n", $k;
    }

    write_hangul_names($fh);

    close($fh);
}

#
# Make a keyfile for all non-systematically named codepoints
#
sub make_named_ucs_keyfile()
{
    my $fh;
    my $k;

    open($fh, '>', 'gen/ucstoname.keys')
	or die "$0: cannot write gen/ucstoname.keys\n";

    foreach $k ( values(%name_to_ucs) ) {
	printf $fh "%08x\n", $k;
    }

    close($fh);
}

#
# Produce a list of character properties, sans names; this is
# a test in order to figure out how much we could save from a
# range-oriented table for everything except names.
#
sub dump_prop_list()
{
    my $fh, $c;

    open($fh, '>', 'gen/propdump.txt')
	or die "$0: cannot write gen/propdump.txt\n";
    binmode $fh, ':utf8';
    
    for ( $c = 0 ; $c <= 0x10ffff ; $c++ ) {
	my %h = %{$ucs_props{$c}};

	# Handle these separately
	delete $h{'Name'};
	delete $h{'Unicode_1_Name'};
	delete $h{'ISO_Comment'};
	delete $h{'Decomposition_Mapping'};
	# delete $h{'Uppercase_Mapping'};
	# delete $h{'Lowercase_Mapping'};
	# delete $h{'Titlecase_Mapping'};
	# delete $h{'Special_Case_Condition'};
	delete $h{'Jamo_Short_Name'};

	# Store these as offsets.
	my $k;
	foreach $k ( 'Simple_Uppercase_Mapping',
		     'Simple_Lowercase_Mapping',
		     'Simple_Titlecase_Mapping' ) {
	    if ( defined($h{$k}) ) {
		$h{$k} -= $c;	# Convert to offset
	    } else {
		$h{$k} = 0;	# Default is zero offset
	    }
	}
    
	my @l = sort(keys(%h));
	my $p;
	printf $fh "%05X ", $c;
	foreach $p ( @l ) {
	    print $fh $p,':',$h{$p},';';
	}
	print $fh "\n";
    }
}

#
# Import files
#
read_separated_file('ucd/UnicodeData.txt',
		    ['!Name', 'eGeneral_Category', 'nCanonical_Combining_Class',
		     'eBidi_Class', '!Decomposition', undef, undef,
		     'eNumeric_Value', 'bBidi_Mirrored',
		     'mUnicode_1_Name', 'mISO_Comment', 'pSimple_Uppercase_Mapping',
		     'pSimple_Lowercase_Mapping', 'pSimple_Titlecase_Mapping'],
		    ['<reserved>', 'Cn', 0, undef, undef, undef, undef, undef,
		     'N', undef, undef, '=', '=', '=']);

read_separated_file('ucd/extracted/DerivedNumericType.txt', ['eNumeric_Type'], []);
read_separated_file('ucd/extracted/DerivedNumericValues.txt', ['eNumeric_Value'], []);
read_separated_file('ucd/extracted/DerivedBidiClass.txt', ['eBidi_Class'], ['L']);
read_separated_file('ucd/ArabicShaping.txt', [undef, 'eJoining_Type', 'eJoining_Group'], []);
read_separated_file('ucd/BidiMirroring.txt', ['pBidi_Mirroring_Glyph'], []);
read_separated_file('ucd/Blocks.txt', ['cBlock'], []);
read_separated_file('ucd/CompositionExclusions.txt', 'bComposition_Exclusion', []);
# read_separated_file('ucd/CaseFolding.txt', ['eCase_Folding_Type', 'sCase_Folding'], []);
read_separated_file('ucd/DerivedAge.txt', ['cAge'], []);
read_separated_file('ucd/EastAsianWidth.txt', ['eEast_Asian_Width'], []);
read_separated_file('ucd/HangulSyllableType.txt', ['eHangul_Syllable_Type'], []);
read_separated_file('ucd/LineBreak.txt', ['eLine_Break'], []);
read_separated_file('ucd/Scripts.txt', ['cScript'], ['Common']);
read_separated_file('ucd/SpecialCasing.txt', ['sUppercase_Mapping', 'sLowercase_Mapping',
					  'sTitlecase_Mapping', 'mSpecial_Case_Condition'], []);
read_separated_file('ucd/Jamo.txt', ['mJamo_Short_Name'], []);
read_boolean_file('ucd/DerivedCoreProperties.txt');
read_boolean_file('ucd/PropList.txt');

#
# Produce output
#
make_jamo_tables();
make_names_list();
make_name_keyfile();
make_named_ucs_keyfile();
dump_prop_list();
