#include "crypto.h"

/**
 * Bring normal buffer into bitsliced form
 * @param pt Input: state_bs in normal form
 * @param state_bs Output: Bitsliced state
 */
static void enslice(const uint8_t pt[CRYPTO_IN_SIZE * BITSLICE_WIDTH], bs_reg_t state_bs[CRYPTO_IN_SIZE_BIT])
{
    uint8_t bytePos, bsPos, bitPos;
    bs_reg_t bsRegTmp, bitTmp;
    uint8_t i, j;

    for (i = 0; i < CRYPTO_IN_SIZE; i++) {
        for (j = 0; j < 8; j++) {
            bsRegTmp = 0;
            for (bitPos = 0; bitPos < 32; bitPos++) {
                bytePos = i + bitPos * 8;
                bitTmp = pt[bytePos] >> j & 0x1;
                bsRegTmp &= ~(0x1 << bitPos);
                bsRegTmp |= bitTmp << bitPos;
            }
            bsPos = i * 8 + j;
            state_bs[bsPos] = bsRegTmp;
        }
    }
}

/**
 * Bring bitsliced buffer into normal form
 * @param state_bs Input: Bitsliced state
 * @param pt Output: state_bs in normal form
 */
static void unslice(const bs_reg_t state_bs[CRYPTO_IN_SIZE_BIT], uint8_t pt[CRYPTO_IN_SIZE * BITSLICE_WIDTH])
{
    uint8_t bitPos, blockNum, i;
    uint8_t bitTmp, ptTmp;

    for (i = 0; i < CRYPTO_IN_SIZE; i++) {
        for (blockNum = 0; blockNum < 32; blockNum++) {
            ptTmp = 0;
            for (bitPos = 0; bitPos < 8; bitPos++) {
                bitTmp = (uint8_t)(state_bs[i * 8 + bitPos] >> blockNum & 0x1);
                ptTmp &= ~(0x1 << bitPos);
                ptTmp |= bitTmp << bitPos;
            }
            pt[i + blockNum * 8] = ptTmp;
        }
    }
}

static void add_round_key(bs_reg_t state[CRYPTO_IN_SIZE_BIT], uint8_t roundkey[CRYPTO_IN_SIZE])
{
    uint8_t i, j, bsPos;
    bs_reg_t bitTmp;
    for (i = 0; i < CRYPTO_IN_SIZE; i++) {
        for (j = 0; j < 8; j++) {
            bsPos = i * 8 + j;
            bitTmp = roundkey[i] >> j & 0x1;
            bitTmp = ~(bitTmp) + 1;
            state[bsPos] ^= bitTmp;
        }
    }
}

static void sbox_layer(bs_reg_t state[CRYPTO_IN_SIZE_BIT])
{
    uint8_t bsPos;
    bs_reg_t state_out[CRYPTO_IN_SIZE_BIT] = { 0 };
    for (bsPos = 0; bsPos < CRYPTO_IN_SIZE_BIT; bsPos += 4) {
        state_out[bsPos] = state[bsPos] ^ (state[bsPos + 1] & state[bsPos + 2]) ^ state[bsPos + 2] ^ state[bsPos + 3];
        state_out[bsPos + 1] = (state[bsPos] & state[bsPos + 2] & state[bsPos + 1]) ^ (state[bsPos] & state[bsPos + 3] & state[bsPos + 1]) ^ (state[bsPos + 3] & state[bsPos + 1]) ^ state[bsPos + 1] ^ (state[bsPos] & state[bsPos + 2] & state[bsPos + 3]) ^
                     (state[bsPos + 2] & state[bsPos + 3]) ^ state[bsPos + 3];
        state_out[bsPos + 2] = ~((state[bsPos] & state[bsPos + 1]) ^ (state[bsPos] & state[bsPos + 3] & state[bsPos + 1]) ^ (state[bsPos + 3] & state[bsPos + 1]) ^
                state[bsPos + 2] ^ (state[bsPos] & state[bsPos + 3]) ^ (state[bsPos] & state[bsPos + 2] & state[bsPos + 3]) ^ state[bsPos + 3]);
        state_out[bsPos + 3] = ~((state[bsPos + 1] & state[bsPos + 2] & state[bsPos]) ^ (state[bsPos + 1] & state[bsPos + 3] & state[bsPos]) ^
                       (state[bsPos + 2] & state[bsPos + 3] & state[bsPos]) ^ state[bsPos] ^ state[bsPos + 1] ^ (state[bsPos + 1] & state[bsPos + 2]) ^
                state[bsPos + 3]);
    }
    for (bsPos = 0; bsPos < CRYPTO_IN_SIZE_BIT; bsPos++) {
        state[bsPos] = state_out[bsPos];
    }
}

static void pbox_layer(bs_reg_t state[CRYPTO_IN_SIZE_BIT])
{
    uint8_t i, out;
    bs_reg_t state_out[CRYPTO_IN_SIZE_BIT];
    for (i = 0; i < CRYPTO_IN_SIZE_BIT; i++) {
        out = i / 4 + (i % 4) * 16;
        state_out[out] = state[i];
    }
    for (i = 0; i < CRYPTO_IN_SIZE_BIT; i++) {
        state[i] = state_out[i];
    }
}

/**
 * Perform next key schedule step
 * @param key Key register to be updated
 * @param r Round counter
 * @warning For correct function, has to be called with incremented r each time
 * @note You are free to change or optimize this function
 */
static void update_round_key(uint8_t key[CRYPTO_KEY_SIZE], const uint8_t r)
{
	//
	// There is no need to edit this code - but you can do so if you want to
	// optimise further
	//

	const uint8_t sbox[16] = {
		0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2,
	};

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

void crypto_func(uint8_t pt[CRYPTO_IN_SIZE * BITSLICE_WIDTH], uint8_t key[CRYPTO_KEY_SIZE])
{
	// State buffer and additional backbuffer of same size (you can remove the backbuffer if you do not need it)
	bs_reg_t state[CRYPTO_IN_SIZE_BIT];

    uint8_t round;

    // Bring into bitslicing form
	enslice(pt, state);

    for (round = 1; round <= 31; round++) {
        add_round_key(state, key + 2);
        sbox_layer(state);
        pbox_layer(state);
        update_round_key(key, round);
    }
    add_round_key(state, key + 2);

	// Convert back to normal form
	unslice(state, pt);
}