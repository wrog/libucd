/* ----------------------------------------------------------------------- *
 *   
 *   Copyright 2005 H. Peter Anvin - All Rights Reserved
 *
 *   Permission is hereby granted, free of charge, to any person
 *   obtaining a copy of this software and associated documentation
 *   files (the "Software"), to deal in the Software without
 *   restriction, including without limitation the rights to use,
 *   copy, modify, merge, publish, distribute, sublicense, and/or
 *   sell copies of the Software, and to permit persons to whom
 *   the Software is furnished to do so, subject to the following
 *   conditions:
 *   
 *   The above copyright notice and this permission notice shall
 *   be included in all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------- */

/*
 * ucd.h
 *
 * Simple interface to the Unicode Character Database
 */

#ifndef UCD_H
#define UCD_H

#include <stdint.h>

enum unicode_bidi_class {
  UC_BIDI_AL,			/* Arabic_Letter */
  UC_BIDI_AN,			/* Arabic_Number */
  UC_BIDI_B,			/* Paragraph_Separator */
  UC_BIDI_BN,			/* Boundary_Neutral */
  UC_BIDI_CS,			/* Common_Separator */
  UC_BIDI_EN,			/* European_Number */
  UC_BIDI_ES,			/* European_Separator */
  UC_BIDI_ET,			/* European_Terminator */
  UC_BIDI_L,			/* Left_To_Right */
  UC_BIDI_LRE,			/* Left_To_Right_Embedding */
  UC_BIDI_LRO,			/* Left_To_Right_Override */
  UC_BIDI_NSM,			/* Nonspacing_Mark */
  UC_BIDI_ON,			/* Other_Neutral */
  UC_BIDI_PDF,			/* Pop_Directional_Format */
  UC_BIDI_R,			/* Right_To_Left */
  UC_BIDI_RLE,			/* Right_To_Left_Embedding */
  UC_BIDI_RLO,			/* Right_To_Left_Override */
  UC_BIDI_S,			/* Segment_Separator */
  UC_BIDI_WS,			/* White_Space */
};

enum unicode_combining_class {
  UC_CCC_NR = 0,		/* Not_Reordered */
  UC_CCC_OV = 1,		/* Overlay */
  UC_CCC_NK = 7,		/* Nukta */
  UC_CCC_KV = 8,		/* Kana_Voicing */
  UC_CCC_VR = 9,		/* Virama */
  UC_CCC_ATBL = 200,		/* Attached_Below_Left */
  UC_CCC_ATB = 202,		/* Attached_Below */
  UC_CCC_ATAR = 216,		/* Attached_Above_Right */
  UC_CCC_BL = 218,		/* Below_Left */
  UC_CCC_B = 220,		/* Below */
  UC_CCC_BR = 222,		/* Below_Right */
  UC_CCC_L = 224,		/* Left */
  UC_CCC_R = 226,		/* Right */
  UC_CCC_AL = 228,		/* Above_Left */
  UC_CCC_A = 230,		/* Above */
  UC_CCC_AR = 232,		/* Above_Right */
  UC_CCC_DB = 233,		/* Double_Below */
  UC_CCC_DA = 234,		/* Double_Above */
  UC_CCC_IS = 240,		/* Iota_Subscript */
};

enum unicode_east_asian_width {
  UC_EAW_A,			/* Ambiguous */
  UC_EAW_F,			/* Fullwidth */
  UC_EAW_H,			/* Halfwidth */
  UC_EAW_N,			/* Neutral */
  UC_EAW_Na,			/* Narrow */
  UC_EAW_W,			/* Wide */
};

enum unicode_grapheme_cluster_break {
  UC_GCB_Other = 0,		/* Other */
  UC_GCB_Control,
  UC_GCB_CR,
  UC_GCB_Extend,
  UC_GCB_L,
  UC_GCB_LF,
  UC_GCB_LV,
  UC_GCB_LVT,
  UC_GCB_T,
  UC_GCB_V,
};

enum unicode_hangul_syllable_type {
  UC_HST_NA = 0,		/* Not applicable */
  UC_HST_L,			/* Leading_Jamo */
  UC_HST_LV,			/* LV syllable */
  UC_HST_LVT,			/* LVT syllable */
  UC_HST_T,			/* Trailing_Jamo */
  UC_HST_V,			/* Vowel_Jamo */
};
enum unicode_joining_group {
  UC_JG_No_Joining_Group = 0,
  UC_JG_Ain,
  UC_JG_Alaph,
  UC_JG_Alef,
  UC_JG_Beh,
  UC_JG_Beth,
  UC_JG_Dal,
  UC_JG_Dalath_Rish,
  UC_JG_E,
  UC_JG_Fe,
  UC_JG_Feh,
  UC_JG_Final_Semkath,
  UC_JG_Gaf,
  UC_JG_Gamal,
  UC_JG_Hah,
  UC_JG_Hamza_On_Heh_Goal,
  UC_JG_He,
  UC_JG_Heh,
  UC_JG_Heh_Goal,
  UC_JG_Heth,
  UC_JG_Kaf,
  UC_JG_Kaph,
  UC_JG_Khaph,
  UC_JG_Knotted_Heh,
  UC_JG_Lam,
  UC_JG_Lamadh,
  UC_JG_Meem,
  UC_JG_Mim,
  UC_JG_Noon,
  UC_JG_Nun,
  UC_JG_Pe,
  UC_JG_Qaf,
  UC_JG_Qaph,
  UC_JG_Reh,
  UC_JG_Reversed_Pe,
  UC_JG_Sad,
  UC_JG_Sadhe,
  UC_JG_Seen,
  UC_JG_Semkath,
  UC_JG_Shin,
  UC_JG_Swash_Kaf,
  UC_JG_Syriac_Waw,
  UC_JG_Tah,
  UC_JG_Taw,
  UC_JG_Teh_Marbuta,
  UC_JG_Teth,
  UC_JG_Waw,
  UC_JG_Yeh,
  UC_JG_Yeh_Barree,
  UC_JG_Yeh_With_Tail,
  UC_JG_Yudh,
  UC_JG_Yudh_He,
  UC_JG_Zain,
  UC_JG_Zhain,
};

enum unicode_joining_type {
  UC_JT_U = 0,
  UC_JT_R,
  UC_JT_L,
  UC_JT_D,
  UC_JT_C,
  UC_JT_T,
};

enum unicode_ternary {
  UC_FALSE = 0,
  UC_TRUE = 1,
  UC_MAYBE = 2
};

enum unicode_numeric_type {
  UC_NT_None = 0,
  UC_NT_Numeric,
  UC_NT_Digit,
  UC_NT_Decimal,
};

enum unicode_sentence_break {
  UC_SB_Other = 0,
  UC_SB_Sep,
  UC_SB_Format,
  UC_SB_Sp,
  UC_SB_Lower,
  UC_SB_Upper,
  UC_SB_OLetter,
  UC_SB_Numeric,
  UC_SB_ATerm,
  UC_SB_STerm,
  UC_SB_Close,
};

enum unicode_word_break {
  UC_WB_Other = 0,
  UC_WB_Format,
  UC_WB_Katakana,
  UC_WB_ALetter,
  UC_WB_MidLetter,
  UC_WB_MidNum,
  UC_WB_Numeric,
  UC_WB_ExtendNumLet,
};

enum unicode_line_break {
  UC_LB_XX = 0,
  UC_LB_BK,
  UC_LB_CR,
  UC_LB_LF,
  UC_LB_CM,
  UC_LB_SG,
  UC_LB_GL,
  UC_LB_CB,
  UC_LB_SP,
  UC_LB_ZW,
  UC_LB_NL,
  UC_LB_WJ,
  UC_LB_JL,
  UC_LB_JV,
  UC_LB_JT,
  UC_LB_H2,
  UC_LB_H3,
  UC_LB_OP,
  UC_LB_CL,
  UC_LB_QU,
  UC_LB_NS,
  UC_LB_EX,
  UC_LB_SY,
  UC_LB_IS,
  UC_LB_PR,
  UC_LB_PO,
  UC_LB_NU,
  UC_LB_AL,
  UC_LB_ID,
  UC_LB_IN,
  UC_LB_HY,
  UC_LB_BB,
  UC_LB_BA,
  UC_LB_SA,
  UC_LB_AI,
  UC_LB_B2,
};

enum unicode_general_category {
  UC_GC_Cn = 0,
  UC_GC_Cc,
  UC_GC_Cf,
  UC_GC_Co,
  UC_GC_Cs,
  UC_GC_Ll,
  UC_GC_Lm,
  UC_GC_Lo,
  UC_GC_Lt,
  UC_GC_Lu,
  UC_GC_Mc,
  UC_GC_Me,
  UC_GC_Mn,
  UC_GC_Nd,
  UC_GC_Nl,
  UC_GC_No,
  UC_GC_Pc,
  UC_GC_Pd,
  UC_GC_Pe,
  UC_GC_Pf,
  UC_GC_Pi,
  UC_GC_Po,
  UC_GC_Ps,
  UC_GC_Sc,
  UC_GC_Sk,
  UC_GC_Sm,
  UC_GC_So,
  UC_GC_Sp,
  UC_GC_Zl,
  UC_GC_Zp,
  UC_GC_Zs,
};

enum unicode_script {
  UC_SCR_Common = 0,
  UC_SCR_Latin,
  UC_SCR_Greek,
  UC_SCR_Cyrillic,
  UC_SCR_Armenian,
  UC_SCR_Hebrew,
  UC_SCR_Arabic,
  UC_SCR_Syriac,
  UC_SCR_Thaana,
  UC_SCR_Devanagari,
  UC_SCR_Bengali,
  UC_SCR_Gurmukhi,
  UC_SCR_Gujarati,
  UC_SCR_Oriya,
  UC_SCR_Tamil,
  UC_SCR_Telugu,
  UC_SCR_Kannada,
  UC_SCR_Malayalam,
  UC_SCR_Sinhala,
  UC_SCR_Thai,
  UC_SCR_Lao,
  UC_SCR_Tibetan,
  UC_SCR_Myanmar,
  UC_SCR_Georgian,
  UC_SCR_Hangul,
  UC_SCR_Ethiopic,
  UC_SCR_Cherokee,
  UC_SCR_Canadian_Aboriginal,
  UC_SCR_Ogham,
  UC_SCR_Runic,
  UC_SCR_Khmer,
  UC_SCR_Mongolian,
  UC_SCR_Hiragana,
  UC_SCR_Katakana,
  UC_SCR_Bopomofo,
  UC_SCR_Han,
  UC_SCR_Yi,
  UC_SCR_Old_Italic,
  UC_SCR_Gothic,
  UC_SCR_Deseret,
  UC_SCR_Inherited,
  UC_SCR_Tagalog,
  UC_SCR_Hanunoo,
  UC_SCR_Buhid,
  UC_SCR_Tagbanwa,
  UC_SCR_Limbu,
  UC_SCR_Tai_Le,
  UC_SCR_Linear_B,
  UC_SCR_Ugaritic,
  UC_SCR_Shavian,
  UC_SCR_Osmanya,
  UC_SCR_Cypriot,
  UC_SCR_Braille,
  UC_SCR_Buginese,
  UC_SCR_Coptic,
  UC_SCR_New_Tai_Lue,
  UC_SCR_Glagolitic,
  UC_SCR_Tifinagh,
  UC_SCR_Syloti_Nagri,
  UC_SCR_Old_Persian,
  UC_SCR_Kharoshthi,
};

enum unicode_block {
  UC_BLK_No_Block = 0,
  UC_BLK_Basic_Latin,
  UC_BLK_Latin_1_Supplement,
  UC_BLK_Latin_Extended_A,
  UC_BLK_Latin_Extended_B,
  UC_BLK_IPA_Extensions,
  UC_BLK_Spacing_Modifier_Letters,
  UC_BLK_Combining_Diacritical_Marks,
  UC_BLK_Greek_and_Coptic,
  UC_BLK_Cyrillic,
  UC_BLK_Cyrillic_Supplement,
  UC_BLK_Armenian,
  UC_BLK_Hebrew,
  UC_BLK_Arabic,
  UC_BLK_Syriac,
  UC_BLK_Arabic_Supplement,
  UC_BLK_Thaana,
  UC_BLK_Devanagari,
  UC_BLK_Bengali,
  UC_BLK_Gurmukhi,
  UC_BLK_Gujarati,
  UC_BLK_Oriya,
  UC_BLK_Tamil,
  UC_BLK_Telugu,
  UC_BLK_Kannada,
  UC_BLK_Malayalam,
  UC_BLK_Sinhala,
  UC_BLK_Thai,
  UC_BLK_Lao,
  UC_BLK_Tibetan,
  UC_BLK_Myanmar,
  UC_BLK_Georgian,
  UC_BLK_Hangul_Jamo,
  UC_BLK_Ethiopic,
  UC_BLK_Ethiopic_Supplement,
  UC_BLK_Cherokee,
  UC_BLK_Unified_Canadian_Aboriginal_Syllabics,
  UC_BLK_Ogham,
  UC_BLK_Runic,
  UC_BLK_Tagalog,
  UC_BLK_Hanunoo,
  UC_BLK_Buhid,
  UC_BLK_Tagbanwa,
  UC_BLK_Khmer,
  UC_BLK_Mongolian,
  UC_BLK_Limbu,
  UC_BLK_Tai_Le,
  UC_BLK_New_Tai_Lue,
  UC_BLK_Khmer_Symbols,
  UC_BLK_Buginese,
  UC_BLK_Phonetic_Extensions,
  UC_BLK_Phonetic_Extensions_Supplement,
  UC_BLK_Combining_Diacritical_Marks_Supplement,
  UC_BLK_Latin_Extended_Additional,
  UC_BLK_Greek_Extended,
  UC_BLK_General_Punctuation,
  UC_BLK_Superscripts_and_Subscripts,
  UC_BLK_Currency_Symbols,
  UC_BLK_Combining_Diacritical_Marks_for_Symbols,
  UC_BLK_Letterlike_Symbols,
  UC_BLK_Number_Forms,
  UC_BLK_Arrows,
  UC_BLK_Mathematical_Operators,
  UC_BLK_Miscellaneous_Technical,
  UC_BLK_Control_Pictures,
  UC_BLK_Optical_Character_Recognition,
  UC_BLK_Enclosed_Alphanumerics,
  UC_BLK_Box_Drawing,
  UC_BLK_Block_Elements,
  UC_BLK_Geometric_Shapes,
  UC_BLK_Miscellaneous_Symbols,
  UC_BLK_Dingbats,
  UC_BLK_Miscellaneous_Mathematical_Symbols_A,
  UC_BLK_Supplemental_Arrows_A,
  UC_BLK_Braille_Patterns,
  UC_BLK_Supplemental_Arrows_B,
  UC_BLK_Miscellaneous_Mathematical_Symbols_B,
  UC_BLK_Supplemental_Mathematical_Operators,
  UC_BLK_Miscellaneous_Symbols_and_Arrows,
  UC_BLK_Glagolitic,
  UC_BLK_Coptic,
  UC_BLK_Georgian_Supplement,
  UC_BLK_Tifinagh,
  UC_BLK_Ethiopic_Extended,
  UC_BLK_Supplemental_Punctuation,
  UC_BLK_CJK_Radicals_Supplement,
  UC_BLK_Kangxi_Radicals,
  UC_BLK_Ideographic_Description_Characters,
  UC_BLK_CJK_Symbols_and_Punctuation,
  UC_BLK_Hiragana,
  UC_BLK_Katakana,
  UC_BLK_Bopomofo,
  UC_BLK_Hangul_Compatibility_Jamo,
  UC_BLK_Kanbun,
  UC_BLK_Bopomofo_Extended,
  UC_BLK_CJK_Strokes,
  UC_BLK_Katakana_Phonetic_Extensions,
  UC_BLK_Enclosed_CJK_Letters_and_Months,
  UC_BLK_CJK_Compatibility,
  UC_BLK_CJK_Unified_Ideographs_Extension_A,
  UC_BLK_Yijing_Hexagram_Symbols,
  UC_BLK_CJK_Unified_Ideographs,
  UC_BLK_Yi_Syllables,
  UC_BLK_Yi_Radicals,
  UC_BLK_Modifier_Tone_Letters,
  UC_BLK_Syloti_Nagri,
  UC_BLK_Hangul_Syllables,
  UC_BLK_High_Surrogates,
  UC_BLK_High_Private_Use_Surrogates,
  UC_BLK_Low_Surrogates,
  UC_BLK_Private_Use_Area,
  UC_BLK_CJK_Compatibility_Ideographs,
  UC_BLK_Alphabetic_Presentation_Forms,
  UC_BLK_Arabic_Presentation_Forms_A,
  UC_BLK_Variation_Selectors,
  UC_BLK_Vertical_Forms,
  UC_BLK_Combining_Half_Marks,
  UC_BLK_CJK_Compatibility_Forms,
  UC_BLK_Small_Form_Variants,
  UC_BLK_Arabic_Presentation_Forms_B,
  UC_BLK_Halfwidth_and_Fullwidth_Forms,
  UC_BLK_Specials,
  UC_BLK_Linear_B_Syllabary,
  UC_BLK_Linear_B_Ideograms,
  UC_BLK_Aegean_Numbers,
  UC_BLK_Ancient_Greek_Numbers,
  UC_BLK_Old_Italic,
  UC_BLK_Gothic,
  UC_BLK_Ugaritic,
  UC_BLK_Old_Persian,
  UC_BLK_Deseret,
  UC_BLK_Shavian,
  UC_BLK_Osmanya,
  UC_BLK_Cypriot_Syllabary,
  UC_BLK_Kharoshthi,
  UC_BLK_Byzantine_Musical_Symbols,
  UC_BLK_Musical_Symbols,
  UC_BLK_Ancient_Greek_Musical_Notation,
  UC_BLK_Tai_Xuan_Jing_Symbols,
  UC_BLK_Mathematical_Alphanumeric_Symbols,
  UC_BLK_CJK_Unified_Ideographs_Extension_B,
  UC_BLK_CJK_Compatibility_Ideographs_Supplement,
  UC_BLK_Tags,
  UC_BLK_Variation_Selectors_Supplement,
  UC_BLK_Supplementary_Private_Use_Area_A,
  UC_BLK_Supplementary_Private_Use_Area_B,
};

#define UC_FLAG(x) (UINT64_C(1) << (x))

#define UC_FL_COMPOSITION_EXCLUSION     UC_FLAG(0)
#define UC_FL_ALPHABETIC                UC_FLAG(1)
#define UC_FL_DEFAULT_IGNORABLE_CODE_POINT  UC_FLAG(2)
#define UC_FL_LOWERCASE                 UC_FLAG(3)
#define UC_FL_GRAPHEME_BASE             UC_FLAG(4)
#define UC_FL_GRAPHEME_EXTEND           UC_FLAG(5)
#define UC_FL_ID_START                  UC_FLAG(6)
#define UC_FL_ID_CONTINUE               UC_FLAG(7)
#define UC_FL_MATH                      UC_FLAG(8)
#define UC_FL_UPPERCASE                 UC_FLAG(9)
#define UC_FL_XID_START                 UC_FLAG(10)
#define UC_FL_XID_CONTINUE              UC_FLAG(11)
#define UC_FL_HEX_DIGIT                 UC_FLAG(12)
#define UC_FL_BIDI_CONTROL              UC_FLAG(13)
#define UC_FL_DASH                      UC_FLAG(14)
#define UC_FL_DEPRECATED                UC_FLAG(15)
#define UC_FL_DIACRITIC                 UC_FLAG(16)
#define UC_FL_EXTENDER                  UC_FLAG(17)
#define UC_FL_GRAPHEME_LINK             UC_FLAG(18)
#define UC_FL_IDEOGRAPHIC               UC_FLAG(19)
#define UC_FL_IDS_BINARY_OPERATOR       UC_FLAG(20)
#define UC_FL_IDS_TRINARY_OPERATOR      UC_FLAG(21)
#define UC_FL_JOIN_CONTROL              UC_FLAG(22)
#define UC_FL_LOGICAL_ORDER_EXCEPTION   UC_FLAG(23)
#define UC_FL_NONCHARACTER_CODE_POINT   UC_FLAG(24)
#define UC_FL_PATTERN_SYNTAX            UC_FLAG(25)
#define UC_FL_PATTERN_WHITE_SPACE       UC_FLAG(26)
#define UC_FL_QUOTATION_MARK            UC_FLAG(27)
#define UC_FL_RADICAL                   UC_FLAG(28)
#define UC_FL_SOFT_DOTTED               UC_FLAG(29)
#define UC_FL_STERM                     UC_FLAG(30)
#define UC_FL_TERMINAL_PUNCTUATION	UC_FLAG(31)
#define UC_FL_UNIFIED_IDEOGRAPH		UC_FLAG(32)
#define UC_FL_VARIATION_SELECTOR	UC_FLAG(33)
#define UC_FL_WHITE_SPACE		UC_FLAG(34)
#define UC_FL_BIDI_MIRRORED		UC_FLAG(35)

struct unicode_character_data {
  uint64_t fl;			/* Flags */
  const char *name;
  const char *bidi_mirroring_glyph;
  const char *uppercase_mapping;
  const char *lowercase_mapping;
  const char *titlecase_mapping;
  int32_t ucs;			/* Actual codepoint */
  int32_t simple_uppercase;
  int32_t simple_lowercase;
  int32_t simple_titlecase;
  /* Numeric value = num/den * 10^exp */
  uint8_t numeric_value_num;
  uint8_t numeric_value_den;
  uint8_t numeric_value_exp;
  uint8_t age_ma, age_mi;
  enum unicode_general_category         general_category;
  enum unicode_block			block;
  enum unicode_script                   script;
  enum unicode_joining_type      	joining_type;
  enum unicode_joining_group     	joining_group;
  enum unicode_east_asian_width         east_asian_width;
  enum unicode_hangul_syllable_type     hangul_syllable_type;
  enum unicode_numeric_type             numeric_type;
  enum unicode_combining_class	        combining_class;
  enum unicode_bidi_class	    	bidi_class;
  enum unicode_grapheme_cluster_break	grapheme_cluster_break;
  enum unicode_sentence_break		sentence_break;
  enum unicode_word_break		word_break;
  enum unicode_line_break		line_break;
};

struct unicode_character_data *unicode_character_data(int32_t);

#endif /* UCD_H */
