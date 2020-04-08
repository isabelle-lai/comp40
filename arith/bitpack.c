/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: bitpack.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Implementation file for bitpack interface which contains
 *             functions that check if values can fit in given widths, and 
 *             gets/sets both signed and unsigned values in a 64 bit unsigned
 *             word.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <except.h>
#include <assert.h>
#include <bitpack.h>

/* CONSTANTS */
const unsigned TOTAL_BITS = 64;

/* EXCEPTION */
Except_T Bitpack_Overflow = {"Overflow packing bits"};

/* HELPER SHIFT FUNCTION PROTOTYPES */
static uint64_t u_shift_left(uint64_t old_num, unsigned shift);
static int64_t s_shift_left(int64_t old_num, unsigned shift);
static uint64_t u_shift_right(uint64_t old_num, unsigned shift);
static int64_t s_shift_right(int64_t old_num, unsigned shift);


/* Function: Bitpack_fitsu
 *
 * Input: uint64_t n (value to check fit of), 
 *        unsigned width (number of bits to check if n fits in)
 * 
 * Output: whether n fits in width or not
 *
 * Purpose: checks if unsigned value n fits in the given width bits
 * 
 * Effects: N/A  
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{  
    /* unsigned values fit in width bits if value is less than or equal 
     * to 2^width - 1 */
    if (((u_shift_left((uint64_t)1, width) - 1) >= n)) {
        return true;
    } else {
        return false;
    }
}

/* Function: Bitpack_fitss
 *
 * Input: int64_t n (value to check fit of), 
 *        unsigned width (number of bits to check if n fits in)
 * 
 * Output: whether n fits in width or not
 *
 * Purpose: checks if signed value n fits in the given width bits
 * 
 * Effects: N/A  
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    /* signed values fit in width bits if value is within the range of 
       2^(width - 1) - 1 and -(2^(width - 1)) */
    if (((s_shift_left((int64_t)1, (width - 1)) - 1) >= n) && 
        ((-1) * (s_shift_left((int64_t)1, (width - 1))) <= n)) {
        return true;
    } else {
        return false;
    }
}

/* Function: Bitpack_getu
 *
 * Input: uint64_t word (word to get value from), 
 *        unsigned width (width in mumber of bits of value), 
 *        unsigned lsb (least significant bit in word of value to get)
 * 
 * Output: unsigned value at given width and lsb in given word
 *
 * Purpose: gets the value of size width bits as an unsigned type at the given
 *          lsb in the word
 * 
 * Effects: Checked runtime error if width or (width + lsb) is greater than 
 *          total bits in the word
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    /* check that width is in correct range */
    assert((width <= TOTAL_BITS));
    assert((width + lsb) <= TOTAL_BITS);

    /* only zero is defined for a width of zero */
    if (width == 0) {
        return 0;
    }
    
    /* create mask with all 1s corresponding to the correct place in the word */
    uint64_t mask = ~0;
    mask = u_shift_right(mask, TOTAL_BITS - width); 
    mask = u_shift_left(mask, lsb);

    /* extract value using mask */
    uint64_t new_word = (word & mask);

    /* shift value to least significant bits and return */
    return u_shift_right(new_word, lsb);
}


/* Function: Bitpack_gets
 *
 * Input: uint64_t word (word to get value from), 
 *        unsigned width (width in mumber of bits of value), 
 *        unsigned lsb (least significant bit in word of value to get)
 * 
 * Output: signed value at given width and lsb in given word
 *
 * Purpose: gets the value of size width bits as a signed type at the given
 *          lsb in the word
 * 
 * Effects: Checked runtime error if width or (width + lsb) is greater than 
 *          total bits in the word
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    /* check that width is in correct range */
    assert((width <= TOTAL_BITS));
    assert((width + lsb) <= TOTAL_BITS);

    /* only zero is defined for a width of zero */
    if (width == 0) {
        return 0;
    }

    /* create mask with all 1s corresponding to the correct place in the word */
    uint64_t mask = ~0;
    mask = u_shift_right(mask, TOTAL_BITS - width); 
    mask = u_shift_left(mask, lsb);

    /* extract value using mask */
    int64_t new_word = (word & mask);

    /* shift value to least significant bits */
    new_word = s_shift_right(new_word, lsb);

    /* if first bit in value is 1, the value must be a negative number,
     * so adjust greater significant bits to be 1s accordingly */
    int64_t temp = s_shift_right(new_word, width - 1);
    if (temp == 1) {
        uint64_t mask2 = ~0;
        mask2 = u_shift_left(mask2, width);
        new_word = (new_word | mask2);
    }
    return new_word;
}

/* Function: Bitpack_newu
 *
 * Input: uint64_t word (word to update value in), 
 *        unsigned width (width in mumber of bits of value), 
 *        unsigned lsb (least significant bit in word of value to get), 
 *        uint64_t value (value being updated to)
 * 
 * Output: the word updated with the value of size width at lsb
 *
 * Purpose: replaces the entry of size width bits as an unsigned type at the 
 *          given lsb in the word with the bits in value
 * 
 * Effects: Checked runtime error if width or (width + lsb) is greater than
 *          total bits in the word.Bitpack Overflow error if given value does 
 *          not fit in given width.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
    /* check that width is in correct range */
    assert(width <= TOTAL_BITS && (width + lsb <= TOTAL_BITS));

    /* check that value fits in width number of bits */
    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);
    }

    /* nothing will be updated if width is 0 */
    if (width == 0) {
        return word;
    }

    /* create masks to clear original value */
    uint64_t mask_l = ~0;
    mask_l = u_shift_left(mask_l, width + lsb);
    uint64_t mask_r = ~0;
    mask_r = u_shift_right(mask_r, TOTAL_BITS - lsb);
    
    /* use final mask to clear original value */
    uint64_t mask_final = mask_l | mask_r;
    word = mask_final & word;

    /* shift value to correct position in word */
    value = u_shift_left(value, lsb);

    return (word | value);
    
}

/* Function: Bitpack_news
 *
 * Input: uint64_t word (word to update value in), 
 *        unsigned width (width in mumber of bits of value), 
 *        unsigned lsb (least significant bit in word of value to get), 
 *        int64_t value (value being updated to)
 * 
 * Output: the word updated with the value of size width at lsb
 *
 * Purpose: replaces the entry of size width bits as a signed type at the 
 *          given lsb in the word with the bits in value
 * 
 * Effects: Checked runtime error if width or (width + lsb) is greater than 
 *          total bits in the word. Bitpack Overflow error if given value does
 *          not fit in given width.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{
    /* check that width is in correct range */
    assert(width <= TOTAL_BITS && (width + lsb <= TOTAL_BITS));

    /* check that value fits in width number of bits */
    if (!Bitpack_fitss(value, width)) {
        RAISE(Bitpack_Overflow);
    }
    
    /* nothing will be updated if width is 0 */
    if (width == 0) {
        return word;
    }
 
    /* create masks to clear original value */
    uint64_t mask_l = ~0;
    mask_l = u_shift_left(mask_l, width + lsb);
    uint64_t mask_r = ~0;
    mask_r = u_shift_right(mask_r, TOTAL_BITS - lsb);
    
    /* use final mask to clear original value */
    uint64_t mask_final = mask_l | mask_r;
    word = mask_final & word;

    /* shift value to correct position in word */
    value = u_shift_left(value, lsb);

    /* if value is signed and has 1s in greater significant bits, 
       clear extra 1s before updating word */
    mask_l = ~mask_l;
    value = mask_l & value;

    return (word | value);
}

/* Function: u_shift_left
 *
 * Input: uint64_t old_num (number to shift), 
 *        unsigned shift (bits to shift by)
 * 
 * Output: the shifted number as a uint64_t
 *
 * Purpose: shifts the given unsigned number to the left by shift bits
 * 
 * Effects: Checked runtime error if shift is greater than total bits in number
 */
static uint64_t u_shift_left(uint64_t old_num, unsigned shift)
{
    /* shifting by total number of bits should yield 0 */
    if (shift == TOTAL_BITS) {
        return 0;
    }
    assert(shift < TOTAL_BITS);
    return (uint64_t)(old_num << shift);
}

/* Function: s_shift_left
 *
 * Input: int64_t old_num (number to shift), 
 *        unsigned shift (bits to shift by)
 * 
 * Output: the shifted number as a int64_t
 *
 * Purpose: shifts the given signed number to the left by shift bits
 * 
 * Effects: Checked runtime error if shift is greater than total bits in number
 */
static int64_t s_shift_left(int64_t old_num, unsigned shift)
{
    //TODO: is this right or what???
    /* shifting by total number of bits should yield 0 */
    if (shift == TOTAL_BITS) {
        return 0;
    }
    assert(shift < TOTAL_BITS);

    return (int64_t)(old_num << shift);
}

/* Function: u_shift_right
 *
 * Input: uint64_t old_num (number to shift), 
 *        unsigned shift (bits to shift by)
 * 
 * Output: the shifted number as a uint64_t
 *
 * Purpose: shifts the given unsigned number to the right by shift bits
 * 
 * Effects: Checked runtime error if shift is greater than total bits in number
 */
static uint64_t u_shift_right(uint64_t old_num, unsigned shift)
{
    /* shifting by total number of bits should yield 0 */
    if (shift == TOTAL_BITS) {
        return 0;
    }
    assert(shift < TOTAL_BITS);
    return (uint64_t)(old_num >> shift);
}

/* Function: s_shift_right
 *
 * Input: uint64_t old_num (number to shift), 
 *        unsigned shift (bits to shift by)
 * 
 * Output: the shifted number as a uint64_t
 *
 * Purpose: shifts the given signed number to the right by shift bits
 * 
 * Effects: Checked runtime error if shift is greater than total bits in number
 */
static int64_t s_shift_right(int64_t old_num, unsigned shift)
{
    /* shifting by total number of bits should yield 0 */
    if (shift == TOTAL_BITS) {
        return ~0;
    }
    assert(shift < TOTAL_BITS);
    return (int64_t)(old_num >> shift);
}