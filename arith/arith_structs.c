/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: arith_structs.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Implementation of Structs module to hold all the structs needed
 *             for the compression and decompression as well as their getters
 *             and setters
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include "arith_structs.h"

/* STRUCT DEFINITIONS */

/* Struct: Comp_Vid_Pixel
 *
 * Purpose: store y,pb, and pr values of each pixel
 */
struct Comp_Vid_Pixel {
    float y, pb, pr;
};

/* Struct: Blocked_Comp_Vid_Pixel
 *
 * Purpose: store floats a,b,c,d,avg_pb, and avg_pr values of each pixel
 */
struct Blocked_Comp_Vid_Pixel 
{
    float a, b, c, d, avg_pb, avg_pr;
};

/* Struct: Scaled_Pixel
 *
 * Purpose: store unsigned a,int b,int c, int d, unsigned avg_pb, and 
 *          unsigned avg_pr values of each pixel
 */
struct Scaled_Pixel{
    unsigned a; 
    int b, c, d;
    unsigned pb, pr;
};

/* GETTERS FOR COMP VID PIXELS */
/* Function: get_comp_vid_y
 *
 * Input: Comp_Vid_Pixel cv_pixel
 *
 * Output: y value of cv_pixel
 *
 * Purpose: gets y value of cv_pixel
 */
float get_comp_vid_y(Comp_Vid_Pixel cv_pixel)
{
    return cv_pixel -> y;
}

/* Function: get_comp_vid_pb
 *
 * Input: Comp_Vid_Pixel cv_pixel
 *
 * Output: pb value of cv_pixel
 *
 * Purpose: gets pb value of cv_pixel
 */
float get_comp_vid_pb(Comp_Vid_Pixel cv_pixel)
{
     return cv_pixel -> pb;
}


/* Function: get_comp_vid_pr
 *
 * Input: Comp_Vid_Pixel cv_pixel
 *
 * Output: pr value of cv_pixel
 *
 * Purpose: gets pr value of cv_pixel
 */
float get_comp_vid_pr(Comp_Vid_Pixel cv_pixel)
{
     return cv_pixel -> pr;
}


/* SETTERS FOR COMP VID PIXELS*/
/* Function: set_comp_vid_y
 *
 * Input: Comp_Vid_Pixel cv_pixel, float y
 *
 * Output: no output
 *
 * Purpose: sets y value for cv pixel 
 */
void set_comp_vid_y(Comp_Vid_Pixel cv_pixel, float y)
{
    cv_pixel -> y = y;
}

/* Function: set_comp_vid_pb
 *
 * Input: Comp_Vid_Pixel cv_pixel, float pb
 *
 * Output: no output
 *
 * Purpose: sets pb value for cv pixel 
 */
void set_comp_vid_pb(Comp_Vid_Pixel cv_pixel, float pb)
{
    cv_pixel -> pb = pb;
}


/* Function: set_comp_vid_pr
 *
 * Input: Comp_Vid_Pixel cv_pixel, float pr
 *
 * Output: no output
 *
 * Purpose: sets pr value for cv pixel 
 */
void set_comp_vid_pr(Comp_Vid_Pixel cv_pixel, float pr)
{
    cv_pixel -> pr = pr;
}

/* Function: get_comp_vid_size
 *
 * Input: no input
 *
 * Output: size of component video pixel 
 *
 * Purpose: returns size of each component video pixel 
 */
size_t get_comp_vid_size() {
    return sizeof(struct Comp_Vid_Pixel);
}

/* GETTERS FOR BLOCKED PIXELS */
/* Function: get_blocked_a
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel
 *
 * Output: a value of blocked pixel
 *
 * Purpose: returns a value of blocked pixel 
 */
float get_blocked_a(Blocked_Comp_Vid_Pixel blocked_pixel)
{
    return blocked_pixel -> a;
}


/* Function: get_blocked_b
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel
 *
 * Output: b value of blocked pixel
 *
 * Purpose: returns b value of blocked pixel 
 */
float get_blocked_b(Blocked_Comp_Vid_Pixel blocked_pixel)
{
    return blocked_pixel -> b;
}


/* Function: get_blocked_c
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel
 *
 * Output: c value of blocked pixel
 *
 * Purpose: returns c value of blocked pixel 
 */
float get_blocked_c(Blocked_Comp_Vid_Pixel blocked_pixel)
{
    return blocked_pixel -> c;
}


/* Function: get_blocked_d
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel
 *
 * Output: d value of blocked pixel
 *
 * Purpose: returns d value of blocked pixel 
 */
float get_blocked_d(Blocked_Comp_Vid_Pixel blocked_pixel)
{
    return blocked_pixel -> d;
}


/* Function: get_blocked_avg_pb
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel
 *
 * Output: avg_pb value of blocked pixel
 *
 * Purpose: returns avg_pb value of blocked pixel 
 */
float get_blocked_avg_pb(Blocked_Comp_Vid_Pixel blocked_pixel)
{
    return blocked_pixel -> avg_pb;
}


/* Function: get_blocked_avg_pr
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel
 *
 * Output: avg_pr value of blocked pixel
 *
 * Purpose: returns avg_pr value of blocked pixel 
 */
float get_blocked_avg_pr(Blocked_Comp_Vid_Pixel blocked_pixel)
{
    return blocked_pixel -> avg_pr;
}

/* SETTERS FOR BLOCKED PIXELS */
/* Function: set_blocked_a
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel, float a
 *
 * Output: no output
 *
 * Purpose: sets a value of blocked pixel 
 */
void set_blocked_a(Blocked_Comp_Vid_Pixel blocked_pixel, float a)
{
    blocked_pixel -> a = a;
}


/* Function: set_blocked_b
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel, float b
 *
 * Output: no output
 *
 * Purpose: set b value of blocked pixel 
 */
void set_blocked_b(Blocked_Comp_Vid_Pixel blocked_pixel, float b)
{
    blocked_pixel -> b = b;
}


/* Function: set_blocked_c
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel, float c
 *
 * Output: no output
 *
 * Purpose: set c value of blocked pixel 
 */
void set_blocked_c(Blocked_Comp_Vid_Pixel blocked_pixel, float c)
{
    blocked_pixel -> c = c;
}


/* Function: set_blocked_d
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel, float d
 *
 * Output: no output
 *
 * Purpose: set d value of blocked pixel 
 */
void set_blocked_d(Blocked_Comp_Vid_Pixel blocked_pixel, float d) 
{
    blocked_pixel -> d = d;
}


/* Function: set_blocked_avg_pb
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel, float avg_pb
 *
 * Output: no output
 *
 * Purpose: set avg_pb value of blocked pixel 
 */
void set_blocked_avg_pb(Blocked_Comp_Vid_Pixel blocked_pixel, float avg_pb)
{
    blocked_pixel -> avg_pb = avg_pb;
}

/* Function: set_blocked_avg_pr
 *
 * Input: Blocked_Comp_Vid_Pixel blocked_pixel, float avg_pr
 *
 * Output: no output
 *
 * Purpose: set avg_pr value of blocked pixel 
 */
void set_blocked_avg_pr(Blocked_Comp_Vid_Pixel blocked_pixel, float avg_pr)
{
    blocked_pixel -> avg_pr = avg_pr;
}

/* Function: get_blocked_size
 *
 * Input: no input
 *
 * Output: size of blocked pixel
 *
 * Purpose: returns size of blocked pixel
 */
size_t get_blocked_size() {
    return sizeof(struct Blocked_Comp_Vid_Pixel);
}

/* GETTERS FOR SCALED PIXELS */
/* Function: get_scaled_a
 *
 * Input:  Scaled_Pixel scaled_pixel
 *
 * Output: a value of scaled pixel
 *
 * Purpose: returns a value of scaled pixel
 */
unsigned get_scaled_a(Scaled_Pixel scaled_pixel)
{
    return scaled_pixel -> a;
}

/* Function: get_scaled_b
 *
 * Input:  Scaled_Pixel scaled_pixel
 *
 * Output: b value of scaled pixel
 *
 * Purpose: returns b value of scaled pixel
 */
int get_scaled_b(Scaled_Pixel scaled_pixel)
{
    return scaled_pixel -> b;
}

/* Function: get_scaled_c
 *
 * Input:  Scaled_Pixel scaled_pixel
 *
 * Output: c value of scaled pixel
 *
 * Purpose: returns c value of scaled pixel
 */
int get_scaled_c(Scaled_Pixel scaled_pixel)
{
    return scaled_pixel -> c;
}

/* Function: get_scaled_d
 *
 * Input:  Scaled_Pixel scaled_pixel
 *
 * Output: d value of scaled pixel
 *
 * Purpose: returns d value of scaled pixel
 */
int get_scaled_d(Scaled_Pixel scaled_pixel)
{
    return scaled_pixel -> d;
}

/* Function: get_scaled_pb
 *
 * Input:  Scaled_Pixel scaled_pixel
 *
 * Output: pb value of scaled pixel
 *
 * Purpose: returns pb value of scaled pixel
 */
unsigned get_scaled_pb(Scaled_Pixel scaled_pixel)
{
    return scaled_pixel -> pb;
}


/* Function: get_scaled_pr
 *
 * Input:  Scaled_Pixel scaled_pixel
 *
 * Output: pr value of scaled pixel
 *
 * Purpose: returns pr value of scaled pixel
 */
unsigned get_scaled_pr(Scaled_Pixel scaled_pixel)
{
    return scaled_pixel -> pr;
}

/* SCALED PIXEL SETTERS */
/* Function: set_scaled_a
 *
 * Input:  Scaled_Pixel scaled_pixel, unsigned a
 *
 * Output: no output
 *
 * Purpose: sets a value for scaled pixel
 */
void set_scaled_a(Scaled_Pixel scaled_pixel, unsigned a)
{
    scaled_pixel -> a = a;
}

/* Function: set_scaled_b
 *
 * Input:  Scaled_Pixel scaled_pixel, unsigned b
 *
 * Output: no output
 *
 * Purpose: sets b value for scaled pixel
 */
void set_scaled_b(Scaled_Pixel scaled_pixel, int b)
{
    scaled_pixel -> b = b;
}

/* Function: set_scaled_c
 *
 * Input:  Scaled_Pixel scaled_pixel, unsigned c
 *
 * Output: no output
 *
 * Purpose: sets c value for scaled pixel
 */
void set_scaled_c(Scaled_Pixel scaled_pixel, int c)
{
    scaled_pixel -> c = c; 
}


/* Function: set_scaled_d
 *
 * Input:  Scaled_Pixel scaled_pixel, unsigned d
 *
 * Output: no output
 *
 * Purpose: sets d value for scaled pixel
 */
void set_scaled_d(Scaled_Pixel scaled_pixel, int d)
{
    scaled_pixel -> d = d;
}

/* Function: set_scaled_pb
 *
 * Input:  Scaled_Pixel scaled_pixel, unsigned pb
 *
 * Output: no output
 *
 * Purpose: sets pb value for scaled pixel
 */
void set_scaled_pb(Scaled_Pixel scaled_pixel, unsigned pb)
{
    scaled_pixel -> pb = pb;
}


/* Function: set_scaled_pr
 *
 * Input:  Scaled_Pixel scaled_pixel, unsigned pr
 *
 * Output: no output
 *
 * Purpose: sets pr value for scaled pixel
 */
void set_scaled_pr(Scaled_Pixel scaled_pixel, unsigned pr)
{
    scaled_pixel -> pr = pr;
}


/* Function: get_scaled_size
 *
 * Input: no input
 *
 * Output: size of scaled pixel
 *
 * Purpose: returns size of scaled pixel
 */
size_t get_scaled_size()
{
    return sizeof(struct Scaled_Pixel);
}


