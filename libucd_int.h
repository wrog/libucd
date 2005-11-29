/*
 * libucd_int.h
 *
 * Internal header file for libucd
 */

#ifndef LIBUCD_INT_H
#define LIBUCD_INT_H

#include <stdlib.h>
#include "ucd.h"
#include "int24.h"

extern const char _libucd_hangul_jamo_l[][4];
extern const char _libucd_hangul_jamo_v[][4];
extern const char _libucd_hangul_jamo_t[][4];

struct _libucd_property_array {
  int32_t  ucd;			/* Wasteful but fast (used in search) */
  uint8_t  general_category;
  uint8_t  script;
  uint8_t  numeric_value_num;
  uint8_t  numeric_value_den_exp; /* bit 7 = 1 if exponent */
  uint64_t flags_block;		/* Block index is high byte */
  int24    simple_uppercase;
  int24    simple_lowercase;
  int24    simple_titlecase;
  uint8_t  age;			/* (major << 5) + minor */
  uint8_t  pad[2];		/* Do something useful here... */
  unsigned arabic_joining_type  :3;
  unsigned arabic_joining_group :6;
  unsigned east_asian_width     :3;
  unsigned hangul_syllable_type :3;
  unsigned line_break           :5;
  unsigned numeric_type         :2;
  unsigned combining_class      :5;
  unsigned bidi_class           :5;
};

#endif
