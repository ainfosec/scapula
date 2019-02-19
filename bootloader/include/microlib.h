/**
 * Microlib:
 * simple support library providing simple stdlib equivalents for
 * discharge
 *
 * Copyright (C) Assured Information Security, Inc.
 *      Author: ktemkin <temkink@ainfosec.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a 
 *  copy of this software and associated documentation files (the "Software"), 
 *  to deal in the Software without restriction, including without limitation 
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *  and/or sell copies of the Software, and to permit persons to whom the 
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in 
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 *  DEALINGS IN THE SOFTWARE.
 */

#include <stddef.h>

#ifndef __MICROLIB_H__
#define __MICROLIB_H__

static const int SUCCESS = 0;

static const int true = 1;
static const int false = 0;

#define BOOTLOADER_PRINT(X, ...) bootloader_printf((X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_INFO(X, ...) bootloader_printf(("[BOOTLOADER] " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_SUBINFO(X, ...) bootloader_printf(("[BOOTLOADER]     " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_DEBUG(X, ...) bootloader_printf(("[BOOTLOADER DEBUG] " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_ALERT(X, ...) bootloader_printf(("[BOOTLOADER ALERT] " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_ERROR(X, ...) bootloader_printf(("[BOOTLOADER ERROR] " X"\n"), ##__VA_ARGS__)

#define stdin 0


/**
* Min and max macros.
*/
#define max(a,b) \
 ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
   _a > _b ? _a : _b; })
#define min(a,b) \
 ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
   _a < _b ? _a : _b; })

void * memcpy(void * dest, const void * src, size_t n);
void * memmove(void *dst0, const void *src0, register size_t length);

void putc(char c, void *stream);
int puts(const char * s);
size_t strnlen(const char *s, size_t max);
void * memchr(const void *s, int c, size_t n);
void * memset(void *b, int c, size_t len);

int bootloader_printf(const char *fmt, ...);

#endif
