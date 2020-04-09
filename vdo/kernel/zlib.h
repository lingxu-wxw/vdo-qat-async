#ifndef ZLIB_H
#define ZLIB_H

#include <linux/types.h>
#include <linux/zlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int zlib_compress_level(void *dest, size_t *destLen, const void *source, size_t sourceLen, int level);
int zlib_uncompress(void *dest, size_t *destLen, const void *source, size_t sourceLen);
int zlib_init(void);
void zlib_fini(void);

#endif /* ZLIB_H */
