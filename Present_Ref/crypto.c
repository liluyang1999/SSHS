#include "crypto.h"

static void add_round_key(uint8_t pt[CRYPTO_IN_SIZE], uint8_t roundkey[CRYPTO_IN_SIZE])
{
    uint8_t i;

    // Perform XOR operation on each element of plaintext block using corresponding round key
    for (i = 0; i < CRYPTO_IN_SIZE; i++) {
        pt[i] ^= roundkey[i];
    }
}

static const uint8_t sbox[16] = {
        0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2,
};

static void sbox_layer(uint8_t s[CRYPTO_IN_SIZE])
{
    uint8_t un, ln, i;

    // Perform SBox lookup to replace the previous high and low 4 bits respectively for each element of block
    for (i = 0; i < CRYPTO_IN_SIZE; i++) {
        ln = s[i] & 0xf;
        un = (s[i] >> 4) & 0xf;
        s[i] = (sbox[un] << 4) | sbox[ln];
    }
}

static void pbox_layer(uint8_t s[CRYPTO_IN_SIZE])
{
    uint8_t i, j, tmp, perPos, bytePos, bitPos;
    uint8_t out[CRYPTO_IN_SIZE] = { 0 };

    /*
     * Perform the permutation on 64 bits
     * Store the permutation result on a newly-allocated space
     * To improve the speed, use outer and inner loop to avoid the calculation of bit position in original state
     * bytePos: which position to store after permutation
     * bitPos: which bit of this state to store
     */
    // Outer loop iterates over each element (8 in total) of block
    for (i = 0; i < CRYPTO_IN_SIZE; i++) {
        // Inner loop iterates over each bit (8 in total) of this element
        for (j = 0; j < 8; j++) {
            tmp = (s[i] >> j) & 0x1;
            perPos = (i * 8 + j) / 4 + (i * 8 + j) % 4 * 16;
            bytePos = perPos / 8;
            bitPos = perPos % 8;
            out[bytePos] &= ~((uint8_t)0x1 << bitPos);
            out[bytePos] |= tmp << bitPos;
        }
    }

    // After finishing permutation, put the result back to the original space
    for (i = 0; i < CRYPTO_IN_SIZE; i++) {
        s[i] = out[i];
    }
}

static void update_round_key(uint8_t key[CRYPTO_KEY_SIZE], const uint8_t r)
{
    //
    // There is no need to edit this code
    //
    uint8_t tmp = 0;
    const uint8_t tmp2 = key[2];
    const uint8_t tmp1 = key[1];
    const uint8_t tmp0 = key[0];

    // rotate right by 19 bit
    key[0] = key[2] >> 3 | key[3] << 5;
    key[1] = key[3] >> 3 | key[4] << 5;
    key[2] = key[4] >> 3 | key[5] << 5;
    key[3] = key[5] >> 3 | key[6] << 5;
    key[4] = key[6] >> 3 | key[7] << 5;
    key[5] = key[7] >> 3 | key[8] << 5;
    key[6] = key[8] >> 3 | key[9] << 5;
    key[7] = key[9] >> 3 | tmp0 << 5;
    key[8] = tmp0 >> 3   | tmp1 << 5;
    key[9] = tmp1 >> 3   | tmp2 << 5;

    // perform sbox lookup on MSbits
    tmp = sbox[key[9] >> 4];
    key[9] &= 0x0F;
    key[9] |= tmp << 4;

    // XOR round counter k19 ... k15
    key[1] ^= r << 7;
    key[2] ^= r >> 1;
}

void crypto_func(uint8_t pt[CRYPTO_IN_SIZE], uint8_t key[CRYPTO_KEY_SIZE])
{
    //
    // There is no need to edit this code
    //

    uint8_t i = 0;

    for(i = 1; i <= 31; i++)
    {
        add_round_key(pt, key + 2);
        sbox_layer(pt);
        pbox_layer(pt);
        update_round_key(key, i);
    }

    add_round_key(pt, key + 2);
}
