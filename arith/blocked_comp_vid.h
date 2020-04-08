/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: blocked_comp_vid.h
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Interface file for module of conversion of image from component
 *             video format to blocked component video format with 4 pixels per
 *             block and corresponding decompression step that revertsit back 
 *             to 4 pixels instead of 1 block
 *
 */

#ifndef BLOCKED_COMP_VID_INCLUDED
#define BLOCKED_COMP_VID_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pnm.h"
#include "arith_structs.h"

Pnm_ppm blocked_cv_image_compress(Pnm_ppm original);
Pnm_ppm  blocked_cv_image_decompress(Pnm_ppm original);

#endif