/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: blocked_comp_vid.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Implementation file for module of conversion of an image from 
 *             component video format to 2x2 blocked component video and 
 *             the corresponding decompression step which reverts it back
 *             to four pixels instead of 1 blocked pixel. 
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include <assert.h>
#include "blocked_comp_vid.h"

/* FUNCTION PROTOTYPES */

static float avg_four(float val1, float val2, float val3, float val4);

/* APPLY FUNCTIONS */
static void cv_to_blocked_apply(int col, int row, 
                                A2Methods_UArray2 blocked_image, 
                                void *elem, void *cl);

static void blocked_to_cv_apply(int col, int row, 
                                A2Methods_UArray2 blocked_image, 
                                void *elem, void *cl);


/* Function: blocked_cv_image_compress
 *
 * Input: Pnm_ppm original (the original component video image)
 *
 * Output: Returns compressed blocked component video format of the image. 
 *
 * Purpose: Reads in an image in component video format, and calls helper 
 *          functions to average Pb and Pr values and perform DCT on y values
 *          for 2x2 pixel blocks.
 * 
 * Effects: Checked runtime error if original image is NULL or new memory could
 *          not be allocated. 
 */
Pnm_ppm blocked_cv_image_compress(Pnm_ppm original)
{
    assert(original != NULL);

    /* Allocate memory for the new compressed image */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    /* Set values of each item in the new image */
    new_image -> denominator = 1;
    new_image -> width = (original -> width) / 2;
    new_image -> height = (original -> height) / 2;
    new_image -> methods = original -> methods;
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> 
                          new(new_image -> width, new_image -> height, 
                              get_blocked_size());

    /* Map over new image, taking corresponding pixel from original image, 
     * compressing, and storing in new image */
    original -> methods -> map_default(new_image -> pixels, 
                                       cv_to_blocked_apply, original);        
    
    return new_image;
}

/* Function: blocked_cv_image_decompress
 *
 * Input: Pnm_ppm original (the image in blocked component video format to
 *        decompress)
 *
 * Output: Returns decompressed component video format of the image. 
 *
 * Purpose: Reads in an image in blocked component video format, and calls 
 *          helper functions to reverse averaging of Pb and Pr values and 
 *          reverses DCT on a,b,c,d values for each pixel, placing values
 *          in corresponding new pixels in new image.
 * 
 * Effects: Checked runtime error if original image is NULL or new memory 
 *          could not be allocated. 
 */
Pnm_ppm blocked_cv_image_decompress(Pnm_ppm original)
{
    assert(original != NULL);

    /* Allocate memory for the new decompressed image */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    /* Set values of each item in the new image */
    new_image -> denominator = 1;
    new_image -> width = (original -> width) * 2;
    new_image -> height = (original -> height) * 2;
    new_image -> methods = original -> methods;
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> 
                          new(new_image -> width, new_image -> height, 
                              get_comp_vid_size());

    /* Map over new image, taking corresponding pixel from original image, 
     * decompressing, and storing in new image */
    new_image -> methods -> map_default(original -> pixels, 
                                        blocked_to_cv_apply, new_image);  
    return new_image;
}


/* Function: cv_to_blocked_apply
 *
 * Input: int col, int row, A2Methods_UArray2 blocked_image, void *elem, 
 *        void *cl (the image in component video format that is being 
 *        compressed)
 *
 * Output: N/A/A
 *
 * Purpose: Apply function for transforming each component video pixel to 
 *          blocked component video pixel format. Gets Pb, Pr, and y values for
 *          each 2x2 pixel block in the original component video image, 
 *          computes averages and DCT, and updates pixel in new image to store 
 *          computed values.
 * 
 * Effects: Checked runtime error if component video image is NULL. Updates 
 *          pixel in blocked image to store compressed values.       
 */
static void cv_to_blocked_apply(int col, int row, 
                                A2Methods_UArray2 blocked_image, 
                                void *elem, void *cl)
{
    (void) blocked_image;
    Blocked_Comp_Vid_Pixel blocked_pixel = (Blocked_Comp_Vid_Pixel) elem;
    Pnm_ppm cv_image = (Pnm_ppm) cl;
    assert(cv_image != NULL);

    Comp_Vid_Pixel cv_pixel1, cv_pixel2, cv_pixel3, cv_pixel4;

    /* Get four pixels in current 2x2 block from original component video 
     * image */
    cv_pixel1 = cv_image -> methods -> at(cv_image -> pixels, col * 2, 
                                          row * 2);
    cv_pixel2 = cv_image -> methods -> at(cv_image -> pixels, (col * 2) + 1, 
                                          row * 2);
    cv_pixel3 = cv_image -> methods -> at(cv_image -> pixels, col * 2, 
                                          (row * 2) + 1);
    cv_pixel4 = cv_image -> methods -> at(cv_image -> pixels, (col * 2) + 1, 
                                          (row * 2) + 1);

    /* Get pb values for each component video pixel */
    float pb1 = get_comp_vid_pb(cv_pixel1);
    float pb2 = get_comp_vid_pb(cv_pixel2);
    float pb3 = get_comp_vid_pb(cv_pixel3);
    float pb4 = get_comp_vid_pb(cv_pixel4);

    /* Get pr values for each component video pixel */
    float pr1 = get_comp_vid_pr(cv_pixel1);
    float pr2 = get_comp_vid_pr(cv_pixel2);
    float pr3 = get_comp_vid_pr(cv_pixel3);
    float pr4 = get_comp_vid_pr(cv_pixel4);

    /* Get average pb and pr values and store in new blocked pixel */
    set_blocked_avg_pb(blocked_pixel, avg_four(pb1, pb2, pb3, pb4));
    set_blocked_avg_pr(blocked_pixel, avg_four(pr1, pr2, pr3, pr4));

    /* Get y values for each component video pixel */
    float y1 = get_comp_vid_y(cv_pixel1); 
    float y2 = get_comp_vid_y(cv_pixel2); 
    float y3 = get_comp_vid_y(cv_pixel3); 
    float y4 = get_comp_vid_y(cv_pixel4);

    /* Perform DCT with four y values and store in new blocked pixel */
    set_blocked_a(blocked_pixel, (y4 + y3 + y2 + y1) / 4.0);
    set_blocked_b(blocked_pixel, (y4 + y3 - y2 - y1) / 4.0);
    set_blocked_c(blocked_pixel, (y4 - y3 + y2 - y1) / 4.0);
    set_blocked_d(blocked_pixel, (y4 - y3 - y2 + y1) / 4.0);
}

/* Function: blocked_to_cv_apply
 *
 * Input: int col, int row, A2Methods_UArray2 blocked_image, void *elem, 
 *        void *cl (the image in component video format that is being 
 *        decompressed to)
 *
 * Output: N/A 
 *
 * Purpose: Apply function for transforming each blocked component video 
 *          pixel to component video pixel format. Gets average Pb, Pr, and 
 *          a,b,c,d values for each 2x2 pixel block in the compressed blocked 
 *          component video image, and updates pixel in new image to store 
 *          correctly computed values for Pb, Pr, and y.
 * 
 * Effects: Checked runtime error if component video image is NULL. Updates 
 *          pixel in comp vid image to store decompressed values.      
 */
static void blocked_to_cv_apply(int col, int row, 
                                A2Methods_UArray2 blocked_image, 
                                void *elem, void *cl)
{
    (void) blocked_image;
    Blocked_Comp_Vid_Pixel blocked_pixel = (Blocked_Comp_Vid_Pixel) elem;
    Pnm_ppm cv_image = (Pnm_ppm) cl;
    assert(cv_image != NULL);
    Comp_Vid_Pixel cv_pixel1, cv_pixel2, cv_pixel3, cv_pixel4;

    /* Get four pixels in current 2x2 block of component video image we are 
     * decompressing to*/
    cv_pixel1 = cv_image -> methods -> at(cv_image -> pixels, col * 2, 
                                          row * 2);
    cv_pixel2 = cv_image -> methods -> at(cv_image -> pixels, (col * 2) + 1, 
                                          row * 2);
    cv_pixel3 = cv_image -> methods -> at(cv_image -> pixels, col * 2, 
                                          (row * 2) + 1);
    cv_pixel4 = cv_image -> methods -> at(cv_image -> pixels, (col * 2) + 1, 
                                          (row * 2) + 1);
    

    /* Set pb and pr values of each component video pixel to be the avg pb and 
     * pr from the blocked pixel */
    set_comp_vid_pb(cv_pixel1, get_blocked_avg_pb(blocked_pixel));
    set_comp_vid_pr(cv_pixel1, get_blocked_avg_pr(blocked_pixel));
    set_comp_vid_pb(cv_pixel2, get_blocked_avg_pb(blocked_pixel));
    set_comp_vid_pr(cv_pixel2, get_blocked_avg_pr(blocked_pixel));
    set_comp_vid_pb(cv_pixel3, get_blocked_avg_pb(blocked_pixel));
    set_comp_vid_pr(cv_pixel3, get_blocked_avg_pr(blocked_pixel));
    set_comp_vid_pb(cv_pixel4, get_blocked_avg_pb(blocked_pixel));
    set_comp_vid_pr(cv_pixel4, get_blocked_avg_pr(blocked_pixel));

    /* Get a, b, c, d stored in blocked_pixel */
    float a = get_blocked_a(blocked_pixel);
    float b = get_blocked_b(blocked_pixel);
    float c = get_blocked_c(blocked_pixel);
    float d = get_blocked_d(blocked_pixel);

    /* Reverse DCT to get y values for each of the four component video 
     * pixels */
    set_comp_vid_y(cv_pixel1, a - b - c + d);
    set_comp_vid_y(cv_pixel2, a - b + c - d);
    set_comp_vid_y(cv_pixel3, a + b - c - d);
    set_comp_vid_y(cv_pixel4, a + b + c + d);
}

/* Function: avg_four
 *
 * Input: four float values
 * 
 * Output: the average of the four arguments in float format
 *
 * Purpose: Averages four floats and returns the average in float format
 * 
 * Effects: N/A  
 */
static float avg_four(float val1, float val2, float val3, float val4) 
{
    return ((val1 + val2 + val3 + val4) / 4.0);
}