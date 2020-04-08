/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: scaled_comp_vid.h
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Interface file for module of conversion of image from blocked 
 *             component video format to scaled component video format by 
 *             quantizing values from blocked format pixels and the
 *             corresponding decompression step that reverts it back to a 
 *             blocked component video format
 *
 */

#ifndef SCALED_COMP_VID_INCLUDED
#define SCALED_COMP_VID_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnm.h>
#include "arith_structs.h"

Pnm_ppm scaled_cv_image_compress(Pnm_ppm original);
Pnm_ppm  scaled_cv_image_decompress(Pnm_ppm original);

#endif