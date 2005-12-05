#include <stdlib.h>
#include "libucd_int.h"
#ifdef HAVE_PTHREAD_H
# include <pthread.h>
#endif

#define CACHE_SIZE 512
static struct unicode_character_data *_libucd_cache[CACHE_SIZE];

#ifdef HAVE_PTHREAD_H
static pthread_mutex_t _libucd_cache_mutex = PTHREAD_MUTEX_INITIALIZER;

static void lock_cache(void)
{
  pthread_mutex_lock(&_libucd_cache_mutex);
}
static void unlock_cache(void)
{
  pthread_mutex_unlock(&_libucd_cache_mutex);
}

#else

/* Single-threaded execution only */
static void lock_cache(void)
{
}
static void unlock_cache(void)
{
}
#endif

#if defined(HAVE_PTHREAD_H) && (defined(__i386__) || defined(__x86_64__))

/* Specially optimized versions for i386 and x86-64 */

struct unicode_character_data *
unicode_character_get(struct unicode_character_data *ucd)
{
  struct libucd_private *pvt = (struct libucd_private *)(ucd+1);
  asm volatile("lock ; incl %0" : "+m" (pvt->usage_ctr));
  return ucd;
}

void
unicode_character_put(struct unicode_character_data *ucd)
{
  struct libucd_private *pvt = (struct libucd_private *)(ucd+1);
  unsigned char zero;
  
  asm volatile("lock ; decl %0 ; setz %1"
	       : "+m" (pvt->usage_ctr), "=r" (zero));
  if ( zero )
    free(ucd);
}

#else

# ifdef HAVE_PTHREAD_H
static void lock(struct libucd_private *pvt)
{
  pthread_mutex_lock(&pvt->mutex);
}
static void unlock(struct libucd_private *pvt)
{
  pthread_mutex_unlock(&pvt->mutex);
}
# else
static void lock(struct libucd_private *pvt)
{
}
static void unlock(struct libucd_private *pvt)
{
}
# endif

struct unicode_character_data *
unicode_character_get(struct unicode_character_data *ucd)
{
  struct libucd_private *pvt = (struct libucd_private *)(ucd+1);
  lock(pvt);
  pvt->usage_ctr++;
  unlock(pvt);
  return ucd;
}

void
unicode_character_put(struct unicode_character_data *ucd)
{
  struct libucd_private *pvt = (struct libucd_private *)(ucd+1);
  unsigned int cnt;
  lock(pvt);
  cnt = --pvt->usage_ctr;
  unlock(pvt);
  if ( !cnt )
    free(ucd);
}

#endif

struct unicode_character_data *
unicode_character_data(int32_t ucs)
{
  struct unicode_character_data *ucd, *prev_ucd;
  unsigned int bucket;
  
  if ( ucs < 0 || ucs > 0x10ffff )
    return NULL;

  bucket = (uint32_t)ucs % CACHE_SIZE;

  lock_cache();
  ucd = _libucd_cache[bucket];
  if ( ucd && ucd->ucs == ucs ) {
    ucd = unicode_character_get(ucd);
    unlock_cache();
    return ucd;
  }
  unlock_cache();

  ucd = unicode_character_data_raw(ucs);
  
  lock_cache();
  prev_ucd = _libucd_cache[bucket];
  _libucd_cache[bucket] = ucd;
  unlock_cache();

  if ( prev_ucd )
    unicode_character_put(prev_ucd);
  
  return ucd;
}
