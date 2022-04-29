#include "crypto.h"

// Define the inline function to complete the task of assigning bit from pt to state_bs
// Aim to unroll the loop in the enslice function below to improve the speed
static inline void present_enslice(uint8_t blockNum, uint8_t ptByte, uint8_t ptBit, uint8_t bsPos,
                                   bs_reg_t state_bs[CRYPTO_IN_SIZE_BIT], uint8_t pt[CRYPTO_IN_SIZE * BITSLICE_WIDTH]) {
    // blockNum: which bit in the element of state_bs, and which block in pt(32 in total)
    // ptBit: which bit in the element of pt
    // ptByte: base uint8_t element in pt (8 in total) used to locate the position
    // 1 ptByte -> 32 uint8_t elements from 32 blocks of pt,  1 ptBit -> 8 bits in each of 32 uint8_t elements
    // ptPos(Position of pt): ptByte + blockNum * 8, bsPos(Position of state_bs)
    // Assign one bit from pt[ptPos] to state_bs[bsPos]
    uint8_t ptPos;
    bs_reg_t bitTmp;
    ptPos = ptByte + blockNum * 8;
    bitTmp = (uint32_t)(pt[ptPos] >> ptBit & 0x1);
    state_bs[bsPos] &= ~(0x1 << blockNum);
    state_bs[bsPos] |= bitTmp << blockNum;
}

/**
 * Bring normal buffer into bitsliced form
 * @param pt Input: state_bs in normal form
 * @param state_bs Output: Bitsliced state
 */
static void enslice(uint8_t pt[CRYPTO_IN_SIZE * BITSLICE_WIDTH], bs_reg_t state_bs[CRYPTO_IN_SIZE_BIT])
{
    uint8_t ptByte, ptBit, bsPos;
    // Expand 64 bits (one block in pt, CRYPTO_IN_SIZE) to state_bs[64],
    // Each element of state_bs has 32 bits (bs_reg_t) that integrate the bits of the same position of the 32 blocks in pt
    // Based on 8 ptByte(one block, type: uint8_t) in pt to locate the position, iterate 8 times corresponding to 8 blocks in state_bs
    // 32 blocks of size 8: 32 * 8 byte  -->  8 blocks of size 32: 8 * 32 byte
    for (ptByte = 0; ptByte < CRYPTO_IN_SIZE; ptByte++) {
        // Inner loop to iterate all bits (8 in total) of one uint8_t element
        // Correspond to 8 bs_reg_t elements of one block in state_bs
        // 1 ptByte -> 32 uint8_t elements from 32 blocks of pt,  1 ptBit -> 8 bits in each uint8_t elements
        for (ptBit = 0; ptBit < 8; ptBit++) {
            // Choose enumeration to assign 32 bits from each block of pt to one bs_reg_t(uint32_t) variable
            bsPos = ptByte * 8 + ptBit; // Calculate the corresponding position in state_bs
            present_enslice(0, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(1, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(2, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(3, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(4, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(5, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(6, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(7, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(8, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(9, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(10, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(11, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(12, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(13, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(14, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(15, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(16, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(17, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(18, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(19, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(20, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(21, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(22, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(23, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(24, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(25, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(26, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(27, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(28, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(29, ptByte, ptBit, bsPos, state_bs, pt);
            present_enslice(30, ptByte, ptBit, bsPos, state_bs, pt); present_enslice(31, ptByte, ptBit, bsPos, state_bs, pt);
        }
    }
}

/**
 * Bring bitsliced buffer into normal form
 * @param state_bs Input: Bitsliced state
 * @param pt Output: state_bs in normal form
 */
static void unslice(bs_reg_t state_bs[CRYPTO_IN_SIZE_BIT], uint8_t pt[CRYPTO_IN_SIZE * BITSLICE_WIDTH])
{
    uint8_t ptByte, ptBit, blockNum, bitTmp, ptTmp;

    // Change the bitsliced form back to the normal form
    // Similar to enslice function, iterate 8 times based on 8 elements of one block in pt to locate the position
    // 8 blocks of size 32: 8 * 32 byte  -->  32 blocks of size 8: 32 * 8 byte
    for (ptByte = 0; ptByte < CRYPTO_IN_SIZE; ptByte++) {
        // Correspond to 32 elements in 32 blocks of pt
        for (blockNum = 0; blockNum < 32; blockNum++) {
            ptTmp = 0;
            // Correspond to 8 bits in each uint8_t element in pt
            // Position in state_bs: ptByte * 8 + ptBit
            // After assigning 8 bits into ptTmp, then use ptTmp to put 8 bits into pt[ptPos] altogether
            for (ptBit = 0; ptBit < 8; ptBit++) {
                bitTmp = (uint8_t)(state_bs[ptByte * 8 + ptBit] >> blockNum & 0x1);
                ptTmp &= ~(0x1 << ptBit);
                ptTmp |= bitTmp << ptBit;
            }
            // Each element's position in pt: ptByte + blockNum * 8
            pt[ptByte + blockNum * 8] = ptTmp;
        }
    }
}

static void add_round_key(bs_reg_t state[CRYPTO_IN_SIZE_BIT], uint8_t roundkey[CRYPTO_IN_SIZE])
{
    uint8_t bsBlock, bsElem;
    bs_reg_t keyExp;

    // State has a total of 64 uint32_t elements which integrates 32 blocks of pt
    // To avoid calculate position of roundkey, divide into 8 bitsliced blocks loop and bitsliced elements loop in each block (64 in total)
    // 1 bsBlock -> 32 bytes in 32 blocks of pt,  1 bsElem -> 32 bits in 32 pt bytes
    // bsBlock locates which position of roundkey
    for (bsBlock = 0; bsBlock < 8; bsBlock++) {
        // bsElem locates which position of bit in this roundkey
        for (bsElem = 0; bsElem < 8; bsElem++) {
            keyExp = roundkey[bsBlock] >> bsElem & 0x1;
            // Add round key to 32 bits altogether performing 32 PRESENT executions in parallel
            // Invert and add one in the bitTmp to assign the value of bit0 to all 32 bits in keyExp
            // Then XOR 32 bits altogether directly using uint32_t keyExp
            keyExp = ~keyExp + 0x1;
            state[bsBlock * 8 + bsElem] ^= keyExp;
        }
    }
}

// Define the inline function to carry out sbox of minimized boolean expressions and pbox
// Merge sbox and pbox by directly putting the sbox result into the permuted position
// Aim to unroll the loop in the sbox_and_pbox function below to improve the speed
static inline void present_sbox_pbox(uint8_t bsPos, bs_reg_t state[CRYPTO_IN_SIZE_BIT], bs_reg_t state_out[CRYPTO_IN_SIZE_BIT]) {
    uint8_t perPos;
    bs_reg_t tmp0 = state[bsPos], tmp1 = state[bsPos + 1], tmp2 = state[bsPos + 2], tmp3 = state[bsPos + 3], c;

    // Calculate the permuted position
    perPos = bsPos / 4 + bsPos % 4 * 16;
    // Put the sbox result into the permuted position
    state_out[perPos] = tmp0 ^ (tmp1 & tmp2) ^ tmp2 ^ tmp3;

    perPos = (bsPos + 1) / 4 + (bsPos + 1) % 4 * 16;
    state_out[perPos] = (tmp1 & ~(tmp0 & (tmp2 ^ tmp3) ^ tmp3)) ^ (tmp3 & ~(tmp0 & tmp2 ^ tmp2));

    perPos = (bsPos + 2) / 4 + (bsPos + 2) % 4 * 16;
    c = ~tmp2;  // x2 + 1 = c
    state_out[perPos] = (tmp1 & (tmp0 ^ (tmp0 & tmp3) ^ tmp3)) ^ (tmp0 & tmp3 & c) ^ c ^ tmp3;

    perPos = (bsPos + 3) / 4 + (bsPos + 3) % 4 * 16;
    state_out[perPos] = (tmp1 & tmp0 & (tmp2 ^ tmp3)) ^ (tmp2 & (tmp3 & tmp0 ^ tmp1) ^ tmp0 ^ tmp1 ^ ~tmp3);
}

// Merge the sbox and pbox layer
static void sbox_and_pbox(bs_reg_t state[CRYPTO_IN_SIZE_BIT])
{
    uint8_t bsPos;
    bs_reg_t state_out[CRYPTO_IN_SIZE_BIT] = { 0 };

    // Use enumeration instead of loop to perform sbox and pbox operations (4 elements each time, 16 times in total)
    present_sbox_pbox(0, state, state_out); present_sbox_pbox(4, state, state_out);
    present_sbox_pbox(8, state, state_out); present_sbox_pbox(12, state, state_out);
    present_sbox_pbox(16, state, state_out); present_sbox_pbox(20, state, state_out);
    present_sbox_pbox(24, state, state_out); present_sbox_pbox(28, state, state_out);
    present_sbox_pbox(32, state, state_out); present_sbox_pbox(36, state, state_out);
    present_sbox_pbox(40, state, state_out); present_sbox_pbox(44, state, state_out);
    present_sbox_pbox(48, state, state_out); present_sbox_pbox(52, state, state_out);
    present_sbox_pbox(56, state, state_out); present_sbox_pbox(60, state, state_out);

    // After finishing sbox and pbox, put the results back to state
    for (bsPos = 0; bsPos < CRYPTO_IN_SIZE_BIT; bsPos++) {
        state[bsPos] = state_out[bsPos];
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

    // Encrypt the bitsliced state to perform 32 PRESENT executions in parallel
    for (round = 1; round <= 31; round++) {
        add_round_key(state, key + 2);
        sbox_and_pbox(state);
        update_round_key(key, round);
    }
    add_round_key(state, key + 2);

	// Convert back to normal form
	unslice(state, pt);
}