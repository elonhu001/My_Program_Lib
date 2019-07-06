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

#include "lib_mecanum.h"
#include "sys_config.h"

//入口参数：前后方向速度vx      左右方向速度vy     旋转速度vw
//输出：四个轮子的转速  r0  r1  r2  r3

void mecanum_calculate(mecanum_t* mecanum)
{
    mecanum->v_r0 = -mecanum->v_x - mecanum->v_y - mecanum->v_w * (WHEEL_BASE + WHEEL_TRACK);
    mecanum->v_r1 = -mecanum->v_x - mecanum->v_y - mecanum->v_w * (WHEEL_BASE + WHEEL_TRACK);
    mecanum->v_r2 = -mecanum->v_x - mecanum->v_y - mecanum->v_w * (WHEEL_BASE + WHEEL_TRACK);
    mecanum->v_r3 = -mecanum->v_x - mecanum->v_y - mecanum->v_w * (WHEEL_BASE + WHEEL_TRACK);
}
