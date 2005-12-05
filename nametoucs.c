#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "libucd_int.h"
#include "gen/nametoucs_hash.h"

static uint32_t prehash(const char *str)
{
  uint32_t hash = PHASHSALT;
  const uint8_t *p = (const uint8_t *)str;

  while ( *p )
    hash = (hash ^ *p++) + ((hash << 27)+(hash >> 5));

  return hash;
}

/* This returns a candidate UCS for a given name. */
int32_t _libucd_ucs_to_name(const char *name)
{
  int32_t ucs;
  uint32_t hash;

  if ( !strncmp(name, "CJK UNIFIED IDEOGRAPH-", 22) ) {
    char *q;

    ucs = strtol(name+22, &q, 16);
    if ( *q || q < name+26 || q > name+27 )
      return -1;
  } else {
    hash = _libucd_nametoucs_hash(prehash(name));
    if ( hash > PHASHNKEYS )
      return -1;
    ucs = getint24(_libucd_nametoucs_tab[hash].ucs);
  }

  return ucs;
}

    
    
