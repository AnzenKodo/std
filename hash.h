/********************************************************************************
 * hash.c: Provides different hash function
 * Code samples & inspiration taken from:
 *  - https://github.com/nothings/stb/blob/master/stb_ds.h by Sean Barrett
 *  - http://www.cse.yorku.ca/~oz/hash.html
 * -----------------------------------------------------------------------------
 * MIT License
 *
 * Copyright 2024 AnzenKodo
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
********************************************************************************/

#ifndef LIBS_HASH
#define LIBS_HASH

#include <stddef.h>
#include <assert.h>

#define HASH_SIZE_T_BITS           ((sizeof (size_t)) * 8)
#define HASH_ROTATE_LEFT(val, n)   (((val) << (n)) | ((val) >> (HASH_SIZE_T_BITS - (n))))
#define HASH_ROTATE_RIGHT(val, n)  (((val) >> (n)) | ((val) << (HASH_SIZE_T_BITS - (n))))

size_t hash_str(const char *s) {
    size_t key = 0;
    assert(s);
    while (*s) {
        key = key * 37 + *s++;
    }
    return key & 0x7FFFFFFF;
}

size_t hash_stb(char *str, size_t seed) {
  size_t hash = seed;
  while (*str)
     hash = HASH_ROTATE_LEFT(hash, 9) + (unsigned char) *str++;

  // Thomas Wang 64-to-32 bit mix function, hopefully also works in 32 bits
  hash ^= seed;
  hash = (~hash) + (hash << 18);
  hash ^= hash ^ HASH_ROTATE_RIGHT(hash,31);
  hash = hash * 21;
  hash ^= hash ^ HASH_ROTATE_RIGHT(hash,11);
  hash += (hash << 6);
  hash ^= HASH_ROTATE_RIGHT(hash,22);
  return hash+seed;
}

unsigned long hash_djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

static unsigned long hash_sdbm(str)
unsigned char *str;
{
    unsigned long hash = 0;
    int c;
    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
    return hash;
}

unsigned long hash_2lose(unsigned char *str) {
	unsigned int hash = 0;
	int c;

	while ((c = *str++))
	    hash += c;

	return hash;
}

unsigned int hash_adler32(const char* s)
{
    unsigned int a = 1;
    unsigned int b = 0;
    const unsigned int MODADLER = 65521;

    size_t i = 0;
    while (s[i] != '\0')
    {
        a = (a + s[i]) % MODADLER;
        b = (b + a) % MODADLER;
        i++;
    }
    return (b << 16) | a;
}

unsigned int hash_crc32(const char* s)
{
    unsigned int crc = 0xffffffff;
    size_t i = 0;
    while (s[i] != '\0')
    {
        unsigned char byte = s[i];
        crc = crc ^ byte;
        for (unsigned char j = 8; j > 0; --j)
        {
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }

        i++;
    }
    return crc ^ 0xffffffff;
}

unsigned char xor8(const char* s)
{
    unsigned char hash = 0;
    size_t i = 0;
    while (s[i] != '\0')
    {
        hash = (hash + s[i]) & 0xff;
        i++;
    }
    return (((hash ^ 0xff) + 1) & 0xff);
}

unsigned int hash_int_jenkins(unsigned int a) {
    a = (a + 0x7ed55d16) + (a << 12);
    a = (a ^ 0xc761c23c) ^ (a >> 19);
    a = (a + 0x165667b1) + (a << 5);
    a = (a + 0xd3a2646c) ^ (a << 9);
    a = (a + 0xfd7046c5) + (a << 3);
    a = (a ^ 0xb55a4f09) ^ (a >> 16);
    return a;
}

unsigned int hash_int_wang(unsigned int a) {
    a = ~a + (a << 15);           /*  a = (a << 15) - a - 1; */
    a = a ^ (a >> 12);
    a = a + (a << 2);
    a = a ^ (a >> 4);
    a = a * 2057;                 /*  a = (a + (a << 3)) + (a << 11); */
    a = a ^ (a >> 16);
    return a;
}

#endif // LIBS_HASH