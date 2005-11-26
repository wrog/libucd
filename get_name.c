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
 * get_name.c
 *
 * Generate the character name from the codepoint number
 */

#include "libucd_int.h"

/*
 * libucd_hangul_name()
 *
 * Take a character in the range of the Hangul characters, and compute
 * its formal name.  Returns the length, or 0 if invalid.
 */

size_t _libucd_hangul_name(char *buf, size_t n, int32_t codepoint)
{
  /* See the Unicode Standard, version 4.1, section 3.12 */
  const int32_t SBase = 0xAC00;
  const int32_t LCount = 19;
  const int32_t VCount = 21;
  const int32_t TCount = 28;
  const int32_t NCount = VCount * TCount; /* 588 */
  const int32_t SCount = NCount * LCount; /* 11172 */

  int32_t SIndex, L, V, T;
  
  SIndex = codepoint - SBase;
  if ( SIndex < 0 || SIndex >= SCount )
    return 0;

  L = SIndex/NCount;
  V = (SIndex % NCount)/TCount;
  T = SIndex % TCount;
  
  return snprintf(buf, n, "HANGUL SYLLABLE %s%s%s",
		  libucd_hangul_jamo_l[L],
		  libucd_hangul_jamo_v[V],
		  libucd_hangul_jamo_t[T]);
}

/*
 * libucd_cjk_name()
 *
 * Naming for CJK unified ideographs
 */

size_t _libucd_cjk_name(char *buf, size_t n, int32_t codepoint)
{
  return snprintf(buf, n, "CJK UNIFIED IDEOGRAPH-%04X", codepoint);
}

