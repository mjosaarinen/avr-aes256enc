// speed.c
// 04-Sep-16  Markku-Juhani O. Saarinen <mjos@iki.fi>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "ser_print.h"
#include "aes256enc.h"

/* === suitable as a "boot-up" self test code === */

static int aes256enc_test()
{
    // Test triplet lifted from FIPS-197

    const uint8_t test_pt[16] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    const uint8_t test_ct[16] = {
        0x8E, 0xA2, 0xB7, 0xCA, 0x51, 0x67, 0x45, 0xBF,
        0xEA, 0xFC, 0x49, 0x90, 0x4B, 0x49, 0x60, 0x89 };

    const uint8_t test_ky[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

    int i;
    uint8_t v[16], ek[AES256_EXPKEY_LEN];


    aes256_exp_key(test_ky, ek);        // expand key

    for (i = 0; i < 16; i++)
        v[i] = test_pt[i];

    aes256_encrypt_ecb(ek, v);          // encrypt

    for (i = 0; i < 16; i++) {
        if (v[i] != test_ct[i])
            return -1;                  // FAIL!
    }

    return 0;
}

/* === speed-testing code === */

uint64_t tick_ticks;
static uint8_t tick_init_done = 0;

// intialize timers

static void tick_init(void)
{
    tick_ticks = 0;
#ifdef __AVR_ATmega128__
    TCCR1B = (1 << CS12);
    TIMSK |= (1 << TOIE1);
#else
    TCCR0B = (1 << CS00);
    TCCR1B = (1 << CS12);
    TIMSK1 |= (1 << TOIE1);
#endif
    TCNT0 = 0;
    TCNT1 = 0;
    sei(); // Enable global interrupts
    tick_init_done = 1;
}

// interrupt handler on TIMER1 overflow

ISR(TIMER1_OVF_vect)
{
    tick_ticks += (1UL << 24);
}

unsigned long long tick_cycles(void)
{
    if (!tick_init_done)
        tick_init();

    return tick_ticks | (((uint64_t) TCNT1) << 8) | ((uint64_t) TCNT0);
}


int main(void)
{
    int i, run;
    uint64_t t;
    uint8_t ek[AES256_EXPKEY_LEN], v[16];

    ser_print("AES-256 Self-Test: ");
    if (aes256enc_test())
        ser_print("FAIL!\n");
    else
        ser_print("OK\n");

    for (i = 0; i < 16; i++) {
        v[i] = i ^ (i << 4);
        ek[i] = i;
        ek[i + 16] = i + 16;
    }
    aes256_exp_key(ek, ek);

    tick_init();

    for (run = 0; run < 10; run++) {

        ser_print("Run #");
        ser_hex8(run);
        ser_print(" ");

        t = tick_cycles();
        for (i = 1 << run; i > 0; i--) {
            aes256_encrypt_ecb(ek, v);
            aes256_encrypt_ecb(ek, v);
            aes256_encrypt_ecb(ek, v);
            aes256_encrypt_ecb(ek, v);
        }
        t = tick_cycles() - t;
        t >>= run + 2;

        ser_dec64(t);
        ser_print(" ticks / block\n");
    }

    ser_end();

    return 0;
}

