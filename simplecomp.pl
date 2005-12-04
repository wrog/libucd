#!/usr/bin/perl
#
# Simple-minded compression algorithm for the Unicode names database
#
# We create a fixed dictionary of 255 symbols (with 0 = end of string);
# the first 38 are the symbols space, dash, 0-9, A-Z which are the
# characters used in Unicode names, and the remaining 217 are common
# phrases.
#

use bytes;

sub split_by_word($) {
    my ($str) = @_;
    my @l = ();
    my @s = split(/([-\s]+)/, $str);

    # Append separated whitespace to each string
    while ( scalar(@s) ) {
	my $x = shift(@s);
	$x .= shift(@s);
	push(@l, $x);
    }

    return @l;
}

@names = ();

# Treat these combinations as single tokens
# This list should really be generated automatically
@unitokens = ('LATIN SMALL LETTER', 'LATIN CAPITAL LETTER',
	      'CAPITAL LETTER', 'SMALL LETTER', 'BRAILLE PATTERN',
	      'BYZANTINE MUSICAL SYMBOL', 'CANADIAN SYLLABICS',
	      'CHEROKEE LETTER', 'VARIATION SELECTOR',
	      'APL FUNCTIONAL SYMBOL', 'BOX DRAWINGS',
	      'CJK COMPATIBILITY IDEOGRAPH', 'KANGXI RADICAL',
	      'LINEAR B', 'MUSICAL SYMBOL', 'ROMAN NUMERAL',
	      'SANS-SERIF', 'LESS-THAN', 'GREATER-THAN', 'SYLOTI NAGRI',
	      'TAI LE LETTER', 'TETRAGRAM FOR', 'THAI CHARACTER',
	      'TIBETAN SUBJOINED LETTER', 'VULGAR FRACTION',
	      'YI SYLLABLE', 'CJK RADICAL', 'YI RADICAL',
	      'ETHIOPIC SYLLABLE', 'IDEOGRAPHIC TELEGRAPH SYMBOL FOR',
	      'DOUBLE-STRUCK', 'NEW TAI LUE', 'PRESENTATION FORM FOR',
	      'UGARITIC LETTER', 'CYPRIOT SYLLABLE'
	      );

while ( defined($line = <STDIN>) ) {
    chomp $line;

    $ix = hex substr($line,0,5);
    $name = substr($line,6);

    # Add a redundant space to each name; we remove this one
    # automatically during decoding
    # XXX: Try with and without this
    $name .= ' ';

    my $ut, $utx;
    foreach $ut ( @unitokens ) {
	($utx = $ut) =~ tr/ -/_+/;
	$name =~ s/\b$ut\b/$utx/g;
    }
    push(@names, $name);
    $name_to_ucs{$name} = $ix;
}

#
# Split sets into words and count
#
%word_weight = ();

foreach $n ( @names ) {
    foreach $w ( split_by_word($n) ) {
	if ( defined($word_weight{$w}) ) {
	    $word_weight{$w} += length($w)-1;
	} else {
	    $word_weight{$w} = -1; # First encounter saves nothing
	}
    }
}

@commons = sort { $word_weight{$b} <=> $word_weight{$a} } keys(%word_weight);

@dictionary = split(//, " -0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

$base_dict = scalar(@dictionary);
$dict_len = 255;

%symbol_index = ();
@symbols = (undef) x ($dict_len+1);
$symbols[0] = '';

# Identity-map single characters
foreach $scs ( @dictionary ) {
    $symbols[ord($scs)] = $scs;
    $symbol_index{$scs} = ord($scs);
}
$next_index = 1;

while ( scalar(@dictionary) < $dict_len ) {
    push(@dictionary, shift(@commons));
}


$s = 0;
for ( $i = 0 ; $i < $dict_len ; $i++ ) {
    $w = $dictionary[$i];
    printf("%3d %8d \"%s\"\n", $i, $word_weight{$w}, $w);
    if ( length($w) > 1 ) {
	$s += $word_weight{$w};
	while ( defined($symbols[$next_index]) ) {
	    $next_index++;
	}
	$symbols[$next_index] = $w;
	$symbol_index{$w} = $next_index;
	$next_index++;
    }
}


print "Bytes saved: $s\n";

# Sort dictionary in order by decreasing length
@dictionary = sort { length($b) <=> length($a) } @dictionary;

$offset = 0;
open(NLC, '>', 'gen/nameslist.compr') or die;
open(NLO, '>', 'gen/nameslist.offset') or die;
foreach $n ( @names ) {
    ($na = $n) =~ tr/_+/ -/;
    foreach $di ( @dictionary ) {
	$c = chr($symbol_index{$di});
	$di =~ tr/_+/ -/;
	$na =~ s/$di/$c/g;
    }
    print  NLC $na, "\0";
    printf NLO "%05x %d\n", $name_to_ucs{$n}, $offset;
    $offset += length($na)+1;
}
close(NLC);
close(NLO);

open(NLD, '>', 'gen/nameslist_dict.c') or die;
printf NLD "const char * const _libucd_nameslist_dict[%d] = {\n", $dict_len+1;
for ( $i = 0 ; $i <= $dict_len ; $i++ ) {
    $sym = $symbols[$i];
    $sym =~ tr/_+/ -/;
    printf NLD "\t\"%s\",\n", $sym;
}
print NLD "};\n";
close(NLD);

