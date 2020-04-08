/*
 *         Assignment: Project 4: Arith
 *
 *         Filename: rgb_to_comp_vid.h
 *
 *         Authors: Isabelle Lai & Andrea Foo
 *
 *         Due Date: March 4, 2020
 *
 *         Summary: Interface file for module of conversion of rgb pixels to component video format
 *                  and its corresponding decompression step from component video to scaled
 *                  rgb format. 
 *
 */

#ifndef RGB_TO_COMP_VID_INCLUDED
#define RGB_TO_COMP_VID_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnm.h>
#include "arith_structs.h"

Pnm_ppm cv_image_compress(Pnm_ppm original);
Pnm_ppm cv_image_decompress(Pnm_ppm original);


#endif