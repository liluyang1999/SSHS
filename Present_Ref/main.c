#include <stdio.h>
#include <stdint.h>
#include "crypto.h"

uint8_t pt[8] = {0x45, 0x84, 0x22, 0x7B, 0x38, 0xC1, 0x79, 0x55};
uint8_t key[10] = {0x3c, 0xf4, 0x00, 0xd8, 0x28, 0xf1, 0x08, 0x7a, 0x60, 0x26};
uint8_t ct[8] = {0xd0, 0x44, 0x6a, 0x0a, 0xc9, 0x13, 0x35, 0xd4};

int main() {
    crypto_func(pt, key);
    printf("Correct: ");
    for (int i = 0; i < sizeof(ct); i++) {
        printf("%.2x ", ct[i]);
    }
    printf("\nResult:  ");
    for (int i = 0; i < sizeof(pt); i++) {
        printf("%.2x ", pt[i]);
    }
    printf("\n");
    return 0;
}




