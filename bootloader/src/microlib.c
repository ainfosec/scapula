/**
 * Microlib:
 * A tiny support library providing only the needed stdlib equivalents for the
 * bareflank bootloader
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

#include <microlib.h>

/**
 * Quick (and not particularly performant) implementation of the standard
 * library's memcpy.
 */
void * memcpy(void * dest, const void * src, size_t n)
{
    const char * src_byte = src;
    char * dest_byte = dest;

    size_t i = 0;

    for(i = 0; i < n; ++i)
        dest_byte[i] = src_byte[i];

    return dest;
}

/**
 * Prints a single character (synchronously) via serial.
 *
 * @param c The character to be printed
 */
extern void _putc(char c);
void putc(char c, void *stream)
{
    // If we're about to send a newline, prefix it with a carriage return.
    // This makes our putc behave like a normal console putc.
    if(c == '\n')
        putc('\r', stream);

    _putc(c);
}

/**
 * Prints a string (synchronously) via serial.
 *
 * @param s The string to be printed; must be null terminated.
 */
int puts(const char * s)
{
    while(*s) {
        putc(*s, stdin);
        ++s;
    }

    return 0;
}

/**
 * Determines the length of a string, scanning at most max characters.
 */
size_t strnlen(const char *s, size_t max)
{
    size_t n = 0;

    while(*s) {
        ++n;
        ++s;

        if(n == max)
            return n;
    }

    return n;
}

/**
 * Fills a given block with a byte value.
 */
void * memset(void *b, int c, size_t len)
{
    unsigned char *p = b;
    int i;

    for(i = 0; i < len; ++i)
        p[i] = c;

    return b;
}
