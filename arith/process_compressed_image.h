/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: processed_compressed_image.h
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Interface file for module of printing compressed image to binary
 *             text in standard output and reading a compressed image binary 
 *             textfile and printing out the decompressed rgb pixel image
 *
 */

#ifndef PROCESSED_COMPRESSED_IMAGE_INCLUDED
#define PROCESSED_COMPRESSED_IMAGE_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnm.h>

void print_compressed_image(Pnm_ppm image);
Pnm_ppm read_compressed_image(FILE *fp);

#endif