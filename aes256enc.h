// aes256enc.h
// 06-Sep-16  Markku-Juhani O. Saarinen <mjos@iki.fi>

#ifndef AES256ENC_H
#define AES256ENC_H

#include <stdint.h>

// expand key. ek[] needs 240 bytes of storage
#define AES256_EXPKEY_LEN 240

void aes256_exp_key(const uint8_t key[32], uint8_t ek[]);

// encrypt a block
void aes256_encrypt_ecb(const uint8_t ek[], uint8_t v[16]);

#endif

