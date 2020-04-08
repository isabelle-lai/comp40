/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: pack_word.h
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Interface file for packing each scaled pixel into a 32 bit word
 *             to form an image of 32 bit words and the corresponding 
 *             decompression step that reverts the pixel back to a scaled pixel
 *                  
 *
 */

#ifndef PACK_WORD_INCLUDED
#define PACK_WORD_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnm.h>
#include "arith_structs.h"

Pnm_ppm pack_words(Pnm_ppm original);
Pnm_ppm  unpack_words(Pnm_ppm original);

#endif