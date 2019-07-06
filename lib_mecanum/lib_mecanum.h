/**
  ******************************************************************************
  * @file    lib_mecanum.c
  * @author  elon hu
  * @tiem    2019-7-6
  * @brief   A library for calculating mecanum wheel.
  *          The installation form of mecanum wheel is O-rectangular, the arrangement
  *          of the roller is like the chart below, whatch from the above
  *          \1    /0
  *          /2    \3
  ******************************************************************************
  */ 

#include "sys_config.h"

#define WHEEL_BASE  (300)
#define WHEEL_TRACK (400)
typedef struct 
{
    float v_x;
    float v_y;
    float v_w;
    float v_r0;
    float v_r1;
    float v_r2;
    float v_r3;
}mecanum_t;