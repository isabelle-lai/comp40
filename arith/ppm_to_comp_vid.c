/*
 *         Assignment: Project 4: Arith
 *
 *         Filename: rgb_to_comp_vid.h
 *
 *         Authors: Isabelle Lai & Andrea Foo
 *
 *         Due Date: March 4, 2020
 *
 *         Summary: Implementation file for module of conversion of rgb int to component video format
 *                  and the corresponding decompression step which converts it scaled rgb int pixel format
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include <assert.h>
#include <pnm.h>
#include "rgb_to_comp_vid.h"

/* CONSTANTS */
const unsigned DENOMINATOR = 65535;


/* STRUCT DEFINITIONS */
/* Struct: RGB_Float_Pixel
 *
 * Purpose: To store floating points of rgb values of each pixel
 */
typedef struct RGB_Float_Pixel {
    float red, green, blue;
} *RGB_Float_Pixel;


/* FUNCTION PROTOTYPES */

static Pnm_ppm trim(Pnm_ppm image);
static void copy_pixel(int col, int row, A2Methods_UArray2 image, void *elem, void *cl);

/* RGB INT AND FLOAT */
static void convert_rgb_to_float(Pnm_rgb int_pixel, RGB_Float_Pixel float_pixel, unsigned denominator);
static void convert_float_to_rgb(Pnm_rgb int_pixel, RGB_Float_Pixel float_pixel, unsigned denominator); 
static float bound_float(float num);

/* RGB FLOAT AND COMPONENT VIDEO */
static void float_to_comp_vid_pixel(RGB_Float_Pixel float_pixel, Comp_Vid_Pixel cv_pixel);
static void comp_vid_pixel_to_float(RGB_Float_Pixel float_pixel, Comp_Vid_Pixel cv_pixel);

/* APPLY FUNCTIONS */
static void rgb_to_cv_apply(int col, int row, A2Methods_UArray2 cv_image, void *elem, void *cl);
static void cv_to_rgb_apply(int col, int row, A2Methods_UArray2 cv_image, void *elem, void *cl);


/* Function: cv_image_compress
 *
 * Input: Pnm_ppm original
 *
 * Output: Returns compressed component video format of the pnm_ppm image. 
 *
 * Purpose: Reads in original pnmppm (in rgb pixels), trims it, and then converts the rgb_pixels to component video pixels. 
 * 
 * Effects: Checked runtime error if image provided is null, malloc fails or created new image is null. Frees memory 
 *          associated with the trimmed image. 
 */
Pnm_ppm cv_image_compress(Pnm_ppm original)
{
    assert(original != NULL);
    Pnm_ppm trimmed_image = trim(original);

    /* 
     * creates new image with the same attributes as original image except for the pixels, which will be created using
     * the map function and converted to comp_vid pixels using an apply function 
     */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    new_image -> denominator = original -> denominator;
    new_image -> width = trimmed_image -> width;
    new_image -> height = trimmed_image -> height;
    new_image -> methods = trimmed_image -> methods;
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> new(new_image -> width, new_image -> height, get_comp_vid_size());
    new_image -> methods -> map_default(new_image -> pixels, rgb_to_cv_apply, trimmed_image);

    assert(trimmed_image != NULL);   
    Pnm_ppmfree(&trimmed_image); 

    return new_image;
}

/* Function: cv_image_decompress
 *
 * Input: Pnm_ppm original
 *
 * Output: Returns decompressed rgb_pixels format of the pnm_ppm image. 
 *
 * Purpose: Reads in original pnmppm that is in component video form and converts it into rgb_pixel (scaled integers) format. 
 * 
 * Effects: Checked runtime error if image provided is null or malloc fails. 
 */
Pnm_ppm cv_image_decompress(Pnm_ppm original)
{
    assert(original != NULL);
    /* 
     * creates new image with the same attributes as original image except for the pixels, which will be created using
     * the map function and converted to rgb_pixels using an apply function 
     */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    new_image -> width = original -> width;
    new_image -> height = original -> height;
    new_image -> methods = original -> methods;
    new_image -> denominator = DENOMINATOR;
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> new(new_image -> width, new_image -> height, sizeof(struct Pnm_rgb));
    new_image -> methods -> map_default(original -> pixels, cv_to_rgb_apply, new_image);    
    return new_image;
}


/* Function: trim
 *
 * Input: Pnm_ppm image
 *
 * Output: Returns trimmed image
 *
 * Purpose: Trims an image to make sure that the width and height are even numbers 
 * 
 * Effects: Checked runtime error if image provided is null or malloc fails. Copies pixels from original image
 *          to the trimmed image based on new height and width using map and apply
 */
static Pnm_ppm trim(Pnm_ppm image)
{
    assert(image != NULL);
    Pnm_ppm trimmed_image = malloc(sizeof(*trimmed_image));
    assert(trimmed_image != NULL);

    /* 
     * Checks to make sure that the width and height are even numbers if it is, it will decrease the width
     * or height by 1
     */
    if (image -> width % 2 == 1) {
        trimmed_image -> width = image -> width - 1;
    } else {
        trimmed_image -> width = image -> width;

    }
    if (image -> height % 2 == 1) {
        trimmed_image -> height = image -> height - 1;
    } else {
        trimmed_image -> height = image -> height;
    }

    trimmed_image -> denominator = image -> denominator;
    trimmed_image -> methods = image -> methods;
    
    trimmed_image -> pixels = (A2Methods_UArray2) trimmed_image -> methods -> new(trimmed_image -> width, trimmed_image -> height, sizeof(struct Pnm_rgb));
    trimmed_image -> methods -> map_default(trimmed_image -> pixels, copy_pixel, image);

    return trimmed_image; 
}

/* Function: copy_pixel
 *
 * Input: Pnm_ppm image
 *
 * Output: N/A
 *
 * Purpose: Apply function for trimming to copy all the original pixels at the original image to the new image
 * 
 * Effects: current pixel of new image is set to the value of the current pixel at the origunal image. 
 */
static void copy_pixel(int col, int row, A2Methods_UArray2 image, void *elem, void *cl) 
{
    (void) image;
    Pnm_rgb new_pixel = (Pnm_rgb) elem;
    Pnm_ppm original_image = (Pnm_ppm) cl;
    assert(original_image != NULL);
    Pnm_rgb curr_pixel = (Pnm_rgb) original_image -> methods -> at(original_image -> pixels, col, row);
    *new_pixel = *curr_pixel;
}

/* Function: ppm_to_cv_apply
 *
 * Input: int col, int row, A2Methods_UArray2 cv_image, void *elem, void *cl
 *
 * Output: N/A
 *
 * Purpose: Apply function for transforming each rgb pixel to component video
 * 
 * Effects: Takes original image's current rgb pixel and converts it to float rgb pixel and then
 *          to component video pixel and set it to the current pixel of new image
 */
static void rgb_to_cv_apply(int col, int row, A2Methods_UArray2 cv_image, void *elem, void *cl)
{
    (void) cv_image;
    Pnm_ppm rgb_image = (Pnm_ppm) cl;
    assert (rgb_image != NULL);
    Pnm_rgb rgb_pixel = rgb_image -> methods -> at(rgb_image -> pixels, col, row);
    int denominator = rgb_image -> denominator;

    /* Intermediate step to convert RGB scaled integers to RGB Floats */
    RGB_Float_Pixel float_pixel = malloc(sizeof(*float_pixel));
    assert(float_pixel != NULL);
    convert_rgb_to_float(rgb_pixel, float_pixel, denominator);

    /*Convert the RGB Floats to component video pixels */
    Comp_Vid_Pixel cv_pixel = (Comp_Vid_Pixel) elem;
    float_to_comp_vid_pixel(float_pixel, cv_pixel);

    /* Free intermediate float pixel to prevent memory leaks */
    assert(float_pixel != NULL);
    free(float_pixel);
}

/* Function: cv_to_rgb_apply
 *
 * Input: int col, int row, A2Methods_UArray2 cv_image, void *elem, void *cl
 *
 * Output: N/A
 *
 * Purpose: Apply function for transforming each component video in original image to rgb pixel format and putting it in
 *          the new image
 * 
 * Effects: Takes original image's current cv pixel and convert it to float rgb pixel and then 
 *          to scaled int rgb pixel and set it to the current pixel of new image
 */
static void cv_to_rgb_apply(int col, int row, A2Methods_UArray2 cv_image, void *elem, void *cl)
{
    (void) cv_image;
    Pnm_ppm rgb_image = (Pnm_ppm) cl;
    assert (rgb_image != NULL);
    Pnm_rgb rgb_pixel = rgb_image -> methods -> at(rgb_image -> pixels, col, row);
    Comp_Vid_Pixel cv_pixel = (Comp_Vid_Pixel) elem;

    /* Intermediate step to convert Component Video pixels to RGB Floats */
    RGB_Float_Pixel float_pixel = malloc(sizeof(*float_pixel));
    assert(float_pixel != NULL);
    comp_vid_pixel_to_float(float_pixel, cv_pixel);

    /*Convert the RGB Floats to RGB pixels */
    convert_float_to_rgb(rgb_pixel, float_pixel, DENOMINATOR);

    /* Free intermediate float pixel to prevent memory leaks */
    assert(float_pixel != NULL);
    free(float_pixel);
}



/* Function: convert_rgb_to_float
 *
 * Input: Pnm_rgb int_pixel, RGB_Float_Pixel float_pixel, unsigned denominator
 *
 * Output: N/A
 *
 * Purpose: Converts rgb scaled integers to rgb floats
 * 
 * Effects: Float pixel values are updated with converted values from int pixel
 */
static void convert_rgb_to_float(Pnm_rgb int_pixel, RGB_Float_Pixel float_pixel, unsigned denominator)
{    
    float_pixel -> red = (float) (int_pixel -> red) / (float) denominator;
    float_pixel -> green = (float) (int_pixel -> green) / (float) denominator;
    float_pixel -> blue = (float)(int_pixel -> blue) / (float) denominator;
}



/* Function: convert_float_to_rgb
 *
 * Input: Pnm_rgb int_pixel, RGB_Float_Pixel float_pixel, unsigned denominator
 *
 * Output: N/A
 *
 * Purpose: Converts floating point rgb values to scaled integers
 * 
 * Effects: RGB pixels are updated with converted values from float pixels
 */
static void convert_float_to_rgb(Pnm_rgb int_pixel, RGB_Float_Pixel float_pixel, unsigned denominator) 
{
    /* check to make sure that floats are within bounds of 0 and 1*/
    float red = bound_float(float_pixel -> red);
    float green = bound_float(float_pixel -> green);
    float blue = bound_float(float_pixel -> blue);

    /* Conversion of float to int */
    int_pixel -> red = red * denominator; 
    int_pixel -> green = green * denominator; 
    int_pixel -> blue = blue * denominator; 
}


/* Function: bound_float
 *
 * Input: float num
 *
 * Output: returns the bound float
 *
 * Purpose: Ensures that the float is within its boundaries between 0 and 1
 */
static float bound_float(float num) 
{
    if (num < 0) {
        num = 0.0;  
    }
    else if (num > 1) {
        num = 1.0;
    }
    return num;
}

/* Function: float_to_comp_vid_pixel
 *
 * Input: RGB_Float_Pixel float_pixel, Comp_Vid_Pixel cv_pixel
 *
 * Output: No output
 *
 * Purpose: Converts float pixels to component video pixels and update value
 * 
 * Effects: Updates the cv pixel values based on float pixel values
 */
static void float_to_comp_vid_pixel(RGB_Float_Pixel float_pixel, Comp_Vid_Pixel cv_pixel)
{
    float red = float_pixel -> red;
    float green = float_pixel -> green;
    float blue = float_pixel -> blue;

    /* equations given in spec */
    float y = 0.299 * red + 0.587 * green + 0.114 * blue;
    float pb = -0.168736 * red - 0.331264 * green + 0.5 * blue;
    float pr = 0.5 * red - 0.418688 * green - 0.081312 * blue;    

    /* Uses setter functions from arith_structs */
    set_comp_vid_y(cv_pixel, y);
    set_comp_vid_pb(cv_pixel, pb);
    set_comp_vid_pr(cv_pixel, pr);
}


/* Function: comp_vid_pixel_to_float
 *
 * Input: RGB_Float_Pixel float_pixel, Comp_Vid_Pixel cv_pixel
 *
 * Output: No output, but updates the rgb float pixel values
 *
 * Purpose: Converts component video pixels to float pixels and updates value
 * 
 * Effects: Updates float pixel values based on cv pixel values
 */
static void comp_vid_pixel_to_float(RGB_Float_Pixel float_pixel, Comp_Vid_Pixel cv_pixel)
{
    /* Uses getter functions from arith_structs */
    float y = get_comp_vid_y(cv_pixel);
    float pb = get_comp_vid_pb(cv_pixel);
    float pr = get_comp_vid_pr(cv_pixel);

    /* Quantization equations given in spec */
    float_pixel -> red = 1.0 * y + 0.0 * pb + 1.402 * pr;
    float_pixel -> green = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
    float_pixel -> blue = 1.0 * y + 1.772 * pb + 0.0 * pr;
}