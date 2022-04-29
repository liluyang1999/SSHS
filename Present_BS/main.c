#include <stdio.h>
#include <stdint.h>
#include "crypto.h"

extern uint8_t pt[8 * 32];
extern uint8_t key[10];
extern uint8_t ct[8 * 3];

int main() {
    crypto_func(pt, key);
    printf("\nCorrect:\n");
    for (int i = 0; i < sizeof(ct) / 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%.2X ", ct[i * 8 + j]);
        }
        printf("\n");
    }

    printf("\nResult:\n");
    for (int i = 0; i < sizeof(ct) / 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%.2X ", pt[i * 8 + j]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}

uint8_t key[10] = {0x3c, 0xf4, 0x00, 0xd8, 0x28, 0xf1, 0x08, 0x7a, 0x60, 0x26,};

uint8_t ct[8 * 3] = {0x5f, 0x46, 0x65, 0xf3, 0x2d, 0x76, 0xfe, 0x9a,
                    0xb7, 0xf4, 0x2d, 0x2e, 0xe9, 0x48, 0x66, 0x23,
                    0xd0, 0x44, 0x6a, 0x0a, 0xc9, 0x13, 0x35, 0xd4, };

uint8_t pt[8 * 32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                     0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,};


