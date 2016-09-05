// ser_print.h
// 04-Sep-15  Markku-Juhani O. Saarinen <mjos@iki.fi>
// bits and pieces originally from various public domain sources

#ifndef SER_PRINT_H
#define SER_PRINT_H

#include <stddef.h>
#include <stdint.h>

void ser_init(void);
void ser_write(uint8_t c);
void ser_print(const char *s);
void ser_dec64(uint64_t x);
void ser_hex8(uint8_t x);
void ser_hex16(uint16_t x);
void ser_end();

#endif
