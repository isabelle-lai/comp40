/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: ppmdiff.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Implementation for ppmdiff
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"

float compare_images(Pnm_ppm image1, Pnm_ppm image2);
float compute_E(Pnm_ppm image1, Pnm_ppm image2, int width, int height);
float computation_helper(unsigned value1, unsigned value2, 
                         unsigned denominator1, unsigned denominator2);

int main (int argc, char *argv[]) 
{
    char *filename1;
    char *filename2;
    if (argc == 3) {
        filename1 = argv[1];
        filename2 = argv[2];
        if (strcmp(filename1, "-") == 0 && strcmp(filename2, "-") == 0) {
            fprintf(stderr, "Please give at least one filename");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Please input the right number of arguments");
        exit(EXIT_FAILURE);
    }


    FILE *source1 = NULL;
    FILE *source2 = NULL;
    if (strcmp(filename1, "-") == 0) {
        source1 = stdin;
    } else {
        
        source1 = fopen(filename1, "r");
    }
    if (strcmp(filename2, "-") == 0) {
        source2 = stdin;
    } else {
        source2 = fopen(filename2, "r");
    }

    (void) source1;
    (void) source2;

    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);

    Pnm_ppm image1 = Pnm_ppmread(source1, methods);
    Pnm_ppm image2 = Pnm_ppmread(source2, methods);

    float result = compare_images(image1, image2);
    printf("%.4f\n", result);

    exit(EXIT_SUCCESS);
}

float compare_images(Pnm_ppm image1, Pnm_ppm image2)
{
    int width1 = (int)image1 -> width;
    int width2 = (int)image2 -> width;
    int height1 = (int)image1 -> height;
    int height2 = (int)image2 -> height;
    int diff_width = abs(width1 - width2);
    int diff_height = abs(height1 - height2);
    if((diff_width > 1) || (diff_height > 1)) {
        fprintf(stderr, "The dimension difference is greater than one\n");
        fprintf(stdout, "%.1f\n", 1.0);
        exit(EXIT_SUCCESS);
    }

    int smallest_width, smallest_height;
    if(width1 <= width2) {
        smallest_width = width1;
    }
    else {
        smallest_width = width2;
    }
    if(height1 <= height2) {
        smallest_height = height1;
    }
    else {
        smallest_height = height2;
    }

    float result = compute_E(image1, image2, smallest_width, smallest_height);
    return result;

}

float compute_E(Pnm_ppm image1, Pnm_ppm image2, int width, int height)
{
    A2Methods_UArray2 uarray1 = image1 -> pixels;
    A2Methods_UArray2 uarray2 = image2 -> pixels;
    A2Methods_T methods = (const A2Methods_T) image1 -> methods;
    unsigned denominator1 = image1 -> denominator;
    unsigned denominator2 = image2 -> denominator;
    float pixel_sum = 0.0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Pnm_rgb pixel1 = (Pnm_rgb) methods -> at(uarray1, i, j);
            Pnm_rgb pixel2 = (Pnm_rgb) methods -> at(uarray2, i, j);
            float red_value = computation_helper(pixel1 -> red, pixel2 -> red, 
                                                 denominator1, denominator2);
            float blue_value = computation_helper(pixel1 -> blue, 
                                                  pixel2 -> blue, denominator1, 
                                                  denominator2);
            float green_value = computation_helper(pixel1 -> green, 
                                                   pixel2 -> green, 
                                                   denominator1, denominator2);
            pixel_sum += red_value + blue_value + green_value;
        }
    }

    float result_E = sqrt(pixel_sum / (3 * width * height));
    return result_E;
}


float computation_helper(unsigned value1, unsigned value2, 
                         unsigned denominator1, unsigned denominator2)
{
    float difference = ((float) value1 / (float) denominator1) - 
                       ((float) value2 / (float) denominator2);
    return difference * difference;
}