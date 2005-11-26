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
  UC_GCB_XX = 0,		/* Other */
  UC_GCB_CN,			/* Control */
  UC_GCB_CR,			/* CR */
  UC_GCB_EX,			/* Extend */
  UC_GCB_L,			/* L */
  UC_GCB_LF,			/* LF */
  UC_GCB_LV,			/* LV */
  UC_GCB_LVT,			/* LVT */
  UC_GCB_T,			/* T */
  UC_GCB_V,			/* V */
};

enum unicode_hangul_syllable_type {
  UC_HST_NA = 0,		/* Not applicable */
  UC_HST_L,			/* Leading_Jamo */
  UC_HST_LV,			/* LV syllable */
  UC_HST_LVT,			/* LVT syllable */
  UC_HST_T,			/* Trailing_Jamo */
  UC_HST_V,			/* Vowel_Jamo */
};

enum unicode_arabic_joining_group {
  UC_AJC_None = 0,
  UC_AJG_Ain,
  UC_AJC_Alaph,
  UC_AJC_Alef,
  UC_AJC_Beh,
  UC_AJC_Beth,
  UC_AJC_Dal,
  UC_AJC_Dalath_Rish,
  UC_AJC_E,
  UC_AJC_Fe,
  UC_AJC_Feh,
  UC_AJC_Final_Semkath,
  UC_AJC_Gaf,
  UC_AJC_Gamal,
  UC_AJC_Hah,
  UC_AJC_Hamza_On_Heh_Goal,
  UC_AJC_He,
  UC_AJC_Heh,
  UC_AJC_Heh_Goal,
  UC_AJC_Heth,
  UC_AJC_Kaf,
  UC_AJC_Kaph,
  UC_AJC_Khaph,
  UC_AJC_Knotted_Heh,
  UC_AJC_Lam,
  UC_AJC_Lamadh,
  UC_AJC_Meem,
  UC_AJC_Mim,
  UC_AJC_Noon,
  UC_AJC_Nun,
  UC_AJC_Pe,
  UC_AJC_Qaf,
  UC_AJC_Qaph,
  UC_AJC_Reh,
  UC_AJC_Reversed_Pe,
  UC_AJC_Sad,
  UC_AJC_Sadhe,
  UC_AJC_Seen,
  UC_AJC_Semkath,
  UC_AJC_Shin,
  UC_AJC_Swash_Kaf,
  UC_AJC_Syriac_Waw,
  UC_AJC_Tah,
  UC_AJC_Taw,
  UC_AJC_Teh_Marbuta,
  UC_AJC_Teth,
  UC_AJC_Waw,
  UC_AJC_Yeh,
  UC_AJC_Yeh_Barree,
  UC_AJC_Yeh_With_Tail,
  UC_AJC_Yudh,
  UC_AJC_Yudh_He,
  UC_AJC_Zain,
  UC_AJC_Zhain,
};

enum unicode_arabic_joining_type {
  UC_AJT_None,
};

enum unicode_ternary {
  UC_FALSE = 0,
  UC_TRUE = 1,
  UC_MAYBE = 2
};

enum unicode_numeric_type {
  UC_NT_None,			/* Not numeric */
  UC_NT_Nu,			/* Numeric */
  UC_NT_Di,			/* Digit */
  UC_NT_De,			/* Decimal digit */
};

enum unicode_sentence_break {
  UC_SB_XX = 0,
  UC_SB_AT,
  UC_SB_CL,
  UC_SB_FO,
  UC_SB_LE,
  UC_SB_LO,
  UC_SB_NU,
  UC_SB_SE,
  UC_SB_SP,
  UC_SB_ST,
  UC_SB_UP,
};

enum unicode_word_break {
  UC_WB_XX = 0,
  UC_WB_EX,
  UC_WB_FO,
  UC_WB_KA,
  UC_WB_LE,
  UC_WB_ML,
  UC_WB_MN,
  UC_WB_NU,
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
  UC_GC_XX = 0,
  UC_GC_Cn,
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
  UC_GC_Zl,
  UC_GC_Sp,
  UC_GC_Zs,
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
  const char *block;
  const char *uppercase_mapping;
  const char *lowercase_mapping;
  const char *titlecase_mapping;
  int32_t ucs;			/* Actual codepoint */
  int32_t simple_uppercase;
  int32_t simple_lowercase;
  int32_t simple_titlecase;
  int16_t numeric_value_num;
  int16_t numeric_value_den;
  enum unicode_general_category         general_category;
  enum unicode_script                   script;
  enum unicode_arabic_joining_type      joining_type;
  enum unicode_arabic_joining_group     joining_group;
  enum unicode_east_asian_width         east_asian_width;
  enum unicode_hangul_syllable_type     hangul_syllable_type;
  enum unicode_line_break               line_break;
  enum unicode_numeric_type             numeric_type;
  enum unicode_combining_class	        combining_class;
  enum unicode_bidi_class	    	bidi_class;
};

#endif /* UCD_H */
