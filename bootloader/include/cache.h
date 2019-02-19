#ifndef __CACHE_H__
#define __CACHE_H__

#include "microlib.h"

/**
 * Invalidate the cache line relevant to the provided address.
 */
void __invalidate_cache_line(const void * addr);


/**
 * Invalides any cache lines that store data relevant to a given regsion.
 */
void __invalidate_cache_region(const void * addr, size_t length);

#endif
