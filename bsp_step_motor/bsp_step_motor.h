/**
  ******************************************************************************
  * @file    bsp_step_motor.h
  * @author  elon hu
  * @tiem    2018-12-11
  * @brief   header file of step motor driver.
  ******************************************************************************
  */ 
#ifndef __BSP_STEP_MOTOR_H
#define __BSP_STEP_MOTOR_H
#include "sys_config.h"

#define PWM_FREQUENCE 50                                                /*<! the frequncy of pwm :PWM_FREQUENCE= APB / ((ARR + 1) * (PSC + 1)) */
#define PWM_RESOLUTION 10000                                            /*<! the reload value, period, remember -1 when use it to initialize timer */
#define PWM_DEFAULT_DUTY (PWM_RESOLUTION/2)                             /*<! the default duty is 50% */

#define TIM_PSC_APB1 ((APB1_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION) /*<! the prescaler of timer */
#define APB1_TIMER_CLOCKS 90000000                                      /*<! the clock frequncy of timer */

#define SUBDIVISION (400)//20000//the pulses for a circle

typedef struct
{
	TIM_HandleTypeDef* timer; /*<! timer that motor used */
	uint32_t channel;         /*<! channel that motor used*/
	GPIO_TypeDef* dir_gpiox;      /*<! direction gpio that motor used */
	uint16_t dir_pinx;
	GPIO_TypeDef* drv_en_gpiox;      /*<! driver enable gpio that motor used */
	uint16_t drv_en_pinx;
	
	uint32_t apb_timer_clocks;
	uint32_t pwm_resolution;
	uint16_t subdivision;     /*<! the needed pulses for one circle */
	uint32_t pulse_cnt;
	uint8_t rotate_ok_flag;
	uint8_t step_pos;
	uint32_t laps;
	void (*f_forward) (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void (*f_back) (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void (*f_enable_driver) (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void (*f_disable_driver) (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
}step_motor_t;

void step_motor_init( step_motor_t* step_motor, 
											TIM_HandleTypeDef* tim, 
											uint32_t chan, 
											GPIO_TypeDef* dir_gpio, 
											uint16_t dir_pin,
											GPIO_TypeDef* drv_en_gpio,      
											uint16_t drv_en_pin,
											uint32_t apb_freq, 
											uint32_t pwm_reso, 
											uint16_t subd);
void step_motor_forward(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void step_motor_back(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void enable_driver(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void disable_driver(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void step_motor_ctrl(step_motor_t* step_motor, float r_speed, uint32_t lap);
void step_motor_pulse_cnt(step_motor_t* step_motor);

#endif


