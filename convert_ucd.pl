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
    
    # None of the syllables exceed 4 bytes, so let's not waste
    # pointer space that might have to be relocated...

    print "const char libucd_hangul_jamo_l[$LCount][4] = {\n";
    for ( $i = 0 ; $i < $LCount ; $i++ ) {
	printf "\t%s,\n", make_jamo_string(${$ucs_props{$LBase+$i}}{'Jamo_Short_Name'});
    }
    print "};\n";
    print "const char libucd_hangul_jamo_v[$VCount][4] = {\n";
    for ( $i = 0 ; $i < $VCount ; $i++ ) {
	printf "\t%s,\n", make_jamo_string(${$ucs_props{$VBase+$i}}{'Jamo_Short_Name'});
    }
    print "};\n";
    print "const char libucd_hangul_jamo_t[$TCount][4] = {\n";
    for ( $i = 0 ; $i < $TCount ; $i++ ) {
	printf "\t%s,\n", make_jamo_string(${$ucs_props{$TBase+$i}}{'Jamo_Short_Name'});
    }
    print "};\n";
}

#
# Import files
#
read_separated_file('UnicodeData.txt',
		    ['!Name', 'eGeneral_Category', 'nCanonical_Combining_Class',
		     'eBidi_Class', '!Decomposition', undef, undef,
		     'eNumeric_Value', 'bBidi_Mirrored',
		     'mUnicode_1_Name', 'mISO_Comment', 'pSimple_Uppercase_Mapping',
		     'pSimple_Lowercase_Mapping', 'pSimple_Titlecase_Mapping'],
		    ['<reserved>', 'Cn', 0, undef, undef, undef, undef, undef,
		     'N', undef, undef, '=', '=', '=']);

read_separated_file('extracted/DerivedNumericType.txt', ['eNumeric_Type'], []);
read_separated_file('extracted/DerivedNumericValues.txt', ['eNumeric_Value'], []);
read_separated_file('extracted/DerivedBidiClass.txt', ['eBidi_Class'], ['L']);
read_separated_file('ArabicShaping.txt', [undef, 'eJoining_Type', 'eJoining_Group'], []);
read_separated_file('BidiMirroring.txt', ['pBidi_Mirroring_Glyph'], []);
read_separated_file('Blocks.txt', ['cBlock'], []);
read_separated_file('CompositionExclusions.txt', 'bComposition_Exclusion', []);
read_separated_file('CaseFolding.txt', ['eCase_Folding_Type', 'sCase_Folding'], []);
read_separated_file('DerivedAge.txt', ['cAge'], []);
read_separated_file('EastAsianWidth.txt', ['eEast_Asian_Width'], []);
read_separated_file('HangulSyllableType.txt', ['eHangul_Syllable_Type'], []);
read_separated_file('LineBreak.txt', ['eLine_Break'], []);
read_separated_file('Scripts.txt', ['cScript'], ['Common']);
read_separated_file('SpecialCasing.txt', ['sUppercase_Mapping', 'sLowercase_Mapping',
					  'sTitlecase_Mapping', 'mSpecial_Case_Condition'], []);
read_separated_file('Jamo.txt', ['mJamo_Short_Name'], []);
read_boolean_file('DerivedCoreProperties.txt');
read_boolean_file('PropList.txt');

#
# Produce output
#
print "#include \"libucd_int.h\"\n\n";
make_jamo_tables();


