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
//how to use
time_capt_t tim2_capt;
get_capture_param(&htim2, &tim2_capt);

#include "bsp_input_capture.h"
#include "sys_config.h"

/**
    * @brief          steps of get timer capture data.
    * @param[in]      htim: timer that used for input capture.
    * @param[in/out]  time_capt: structure of a timer whitch storaged the figure of input capture.
    * @retval None
    * @auther elon hu
    * @time   2018-7-3
    */
void get_capture_param(TIM_HandleTypeDef *htim, time_capt_t *time_capt)
{
    uint8_t i;
    for(i = 0; i < 4; i++)
    {
        switch(time_capt->channels[i].capt_cnt)
        {
            case 0:
            {
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, time_capt->channels[i].ch, TIM_INPUTCHANNELPOLARITY_RISING);//检测上升沿
                HAL_TIM_IC_Start_IT(htim,  time_capt->channels[i].ch);
                time_capt->channels[i].capt_cnt++;
            }break;

            case 4:
            {
                time_capt->channels[i].cycle_time = (time_capt->channels[i].capt_buf[2] - time_capt->channels[i].capt_buf[0]) / 10000.0;
                time_capt->channels[i].high_time = (time_capt->channels[i].capt_buf[1] - time_capt->channels[i].capt_buf[0]) / 10000.0;						
                time_capt->channels[i].duty_last = time_capt->channels[i].duty;		
                time_capt->channels[i].duty_llast = time_capt->channels[i].duty_last;								
                time_capt->channels[i].duty = (time_capt->channels[i].high_time / time_capt->channels[i].cycle_time) * 100;
                time_capt->channels[i].duty_err = time_capt->channels[i].duty - time_capt->channels[i].duty_last;				

                /*限幅滤波*/
                if((time_capt->channels[i].duty > 93.30f) || (time_capt->channels[i].duty < 88.470f))
                {
                time_capt->channels[i].duty = time_capt->channels[i].duty_last;
                }

                /*误差限幅滤波，对摇杆数据做处理*/
                if(htim == &htim2)
                {
                if((time_capt->channels[i].duty_err > 2.0f) && (time_capt->channels[i].duty_err < -2.0f)) //85.0f
                {
                time_capt->channels[i].duty = time_capt->channels[i].duty_last;
                }						
                }

                time_capt->channels[i].capt_cnt = 0;                
            }break;
        }        
    }     
}

/**
    * @brief          steps of get timer capture data, used in timer itterupt.
    * @param[in]      htim: timer that used for input capture.
    * @param[in/out]  time_capt_channel: spacific timer channal of input capture figure structure.
    * @retval None
    * @auther elon hu
    * @time   2018-7-3
    */
static void capture_record(TIM_HandleTypeDef *htim, time_capt_channel_t *time_capt_channel)
{
    switch(time_capt_channel->capt_cnt)
    {
        case 1:
        {
            time_capt_channel->capt_buf[0] = __HAL_TIM_GET_COMPARE(htim, time_capt_channel->ch);
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, time_capt_channel->ch, TIM_INPUTCHANNELPOLARITY_RISING);//检测上升沿
            time_capt_channel->capt_cnt++;             
        } break;
        
        case 2:
        {
            time_capt_channel->capt_buf[1] = __HAL_TIM_GET_COMPARE(htim, time_capt_channel->ch);
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, time_capt_channel->ch, TIM_INPUTCHANNELPOLARITY_FALLING);//检测下降沿
            time_capt_channel->capt_cnt++;             
        }break;
        
        case 3:
        {
            time_capt_channel->capt_buf[2] = __HAL_TIM_GET_COMPARE(htim, time_capt_channel->ch);
            HAL_TIM_IC_Stop_IT(htim, time_capt_channel->ch);
            time_capt_channel->capt_cnt++;             
        }break;
        default: break;
    } 
}
/**
    * @brief  served for input capture, be called in timer intterupt.
    * @param[in]      htim: timer that used for input capture.
    * @param[in/out]  time_capt: structure of a timer whitch storaged the figure of input capture.
    * @retval None
    * @auther elon hu
    * @time   2018-7-3
    */
static void capture_handle(TIM_HandleTypeDef *htim, time_capt_t *time_capt)
{
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        capture_record(htim, &time_capt->channels[0]);
    }
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        capture_record(htim, &time_capt->channels[1]);
    }
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        capture_record(htim, &time_capt->channels[2]);
    }
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
        capture_record(htim, &time_capt->channels[3]);
    }
}
/**
    * @brief  timer intterupt callback function.
    * @param[in]      htim: timer that used for input capture.
    * @retval None
    * @auther elon hu
    * @time   2018-7-3
    */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
    {
        capture_handle(htim, &tim2_capt);
    }
    /* user can choose to use witch timer depend on specific situation */
    /*
    if(htim->Instance == TIM3)
    {
        capture_handle(htim, &tim3_capt);
    }
    if(htim->Instance == TIM5)
    {
        capture_handle(htim, &tim5_capt);
    }    
    */
}