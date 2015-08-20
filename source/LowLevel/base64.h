#ifndef _BASE64_H_
#define _BASE64_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char *base64_encode(const unsigned char *src, size_t len, size_t *out_len);
unsigned char *base64_decode(const unsigned char *src, size_t len, size_t *out_len);

#endif // _BASE64_H_
