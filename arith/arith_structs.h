/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: arith_sturcts.h
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Interface of Structs module to hold all the structs needed
 *             for the compression and decompression as well as their 
 *             getters and setters
 *
 */

#ifndef ARITH_STRUCTS_INCLUDED
#define ARITH_STRUCTS_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>

/*STRUCT DEFINITIONS */
typedef struct Comp_Vid_Pixel *Comp_Vid_Pixel;
typedef struct Blocked_Comp_Vid_Pixel *Blocked_Comp_Vid_Pixel;
typedef struct Scaled_Pixel *Scaled_Pixel;

/* COMPONENT VIDEO FUNCTIONS */
float get_comp_vid_y(Comp_Vid_Pixel cv_pixel);
float get_comp_vid_pb(Comp_Vid_Pixel cv_pixel);
float get_comp_vid_pr(Comp_Vid_Pixel cv_pixel);

void set_comp_vid_y(Comp_Vid_Pixel cv_pixel, float y);
void set_comp_vid_pb(Comp_Vid_Pixel cv_pixel, float pb);
void set_comp_vid_pr(Comp_Vid_Pixel cv_pixel, float pr);

size_t get_comp_vid_size();

/* BLOCKED COMPONENT VIDEO FUNCTIONS */
float get_blocked_a(Blocked_Comp_Vid_Pixel blocked_pixel);
float get_blocked_b(Blocked_Comp_Vid_Pixel blocked_pixel);
float get_blocked_c(Blocked_Comp_Vid_Pixel blocked_pixel);
float get_blocked_d(Blocked_Comp_Vid_Pixel blocked_pixel);
float get_blocked_avg_pb(Blocked_Comp_Vid_Pixel blocked_pixel);
float get_blocked_avg_pr(Blocked_Comp_Vid_Pixel blocked_pixel);

void set_blocked_a(Blocked_Comp_Vid_Pixel blocked_pixel, float a);
void set_blocked_b(Blocked_Comp_Vid_Pixel blocked_pixel, float b);
void set_blocked_c(Blocked_Comp_Vid_Pixel blocked_pixel, float c);
void set_blocked_d(Blocked_Comp_Vid_Pixel blocked_pixel, float d);
void set_blocked_avg_pb(Blocked_Comp_Vid_Pixel blocked_pixel, float avg_pb);
void set_blocked_avg_pr(Blocked_Comp_Vid_Pixel blocked_pixel, float avg_pr);

size_t get_blocked_size();

/*SCALED PIXEL FUNCTIONS */
unsigned get_scaled_a(Scaled_Pixel scaled_pixel);
int get_scaled_b(Scaled_Pixel scaled_pixel);
int get_scaled_c(Scaled_Pixel scaled_pixel);
int get_scaled_d(Scaled_Pixel scaled_pixel);
unsigned get_scaled_pb(Scaled_Pixel scaled_pixel);
unsigned get_scaled_pr(Scaled_Pixel scaled_pixel);

void set_scaled_a(Scaled_Pixel scaled_pixel, unsigned a);
void set_scaled_b(Scaled_Pixel scaled_pixel, int b);
void set_scaled_c(Scaled_Pixel scaled_pixel, int c);
void set_scaled_d(Scaled_Pixel scaled_pixel, int d);
void set_scaled_pb(Scaled_Pixel scaled_pixel, unsigned pb);
void set_scaled_pr(Scaled_Pixel scaled_pixel, unsigned pr);

size_t get_scaled_size();


#endif