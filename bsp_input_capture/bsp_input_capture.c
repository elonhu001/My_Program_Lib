/**
  ******************************************************************************
  * @file    bsp_input_capture.c
  * @author  elon hu
  * @tiem    2019-7-3
  * @brief   input capture driver.
  *          This file provides firmware functions to manage the functionalities
  *          of the input capture.
  *          首先明确一下输入捕获的操作流程，先进行上升沿捕获，然后进行下降沿捕获，最后
  *          再次进行上升沿捕获，这样就捕获了一个周期的波形，可以获得高电平时间和周期时
  *          间。
  *          该份驱动库分的捕获步骤为0~4五个部分，其中第0和4个部分在主函数中运行，1、2
  *          、3部分在输入捕获中断回调中运行。
  ******************************************************************************
  */ 

typedef struct
{
    uint32_t ch;
    uint32_t capt_buf[3];
    uint8_t  capt_cnt;

    float    cycle_time;
    float    high_time;
    float    duty;
    float    duty_last;
    float    duty_llast;
    float    duty_err;
}time_capt_channel_t;

typedef struct
{
    TIM_HandleTypeDef tim[3];
    time_capt_channel_t channels[4];
}time_capt_t;

