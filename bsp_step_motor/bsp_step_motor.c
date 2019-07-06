/**
  ******************************************************************************
  * @file    bsp_step_motor.c
  * @author  elon hu
  * @date    2018-12-11
  * @brief   step motor driver.
  *          This file provides firmware functions to manage the functionalities
  *          of the step motor.
	* @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
	* step1: define a structure for a step motor
	* -@-    step_motor_t step_motor_break;
	* step2: put the function step_motor_pulse_cnt() into the corresponding time 
	* 			 global interrupt function of PWM wave. The interrupt function will be 
	*				 find in stm32f4xx_it.c file.
	* -@-    void TIM3_IRQHandler(void)
	*	-@-		 {
	*	-@-				step_motor_pulse_cnt(&step_motor_up_down);
	*	-@-		 }
	* step3: initiallize for the step motor.
	*	-@-    step_motor_init(&step_motor_break, &htim2, TIM_CHANNEL_2, GPIOC,  \
	*        GPIO_PIN_0, GPIOB, GPIO_PIN_14, APB1_TIMER_CLOCKS, PWM_RESOLUTION, SUBDIVISION);
	* step4: use the control function.
	*	-@-    step_motor_ctrl(&step_motor_break, -2, 0);
  ******************************************************************************
  */ 

#include "bsp_step_motor.h"
#include "sys_config.h"

/**
	* @brief  initialize step motor.
	* @param[in\out]  step_motor: the structure of the motor to be used.
	* @param[in]      tim: timer that motor used.
	* @param[in]      chan: channel that motor used.
	* @param[in]      gpio: gpio that motor used for direction.
	* @param[in]      pin: gpio that motor used for direction.
	* @param[in]      apb_freq: the main frequncy of timer that be used.
	* @param[in]      pwm_freq: the default pwm frequncy to be set.
	* @param[in]      subd: the subdivision that motor used.
	* @retval None
	* @auther elon hu
	* @time   2018-12-11
	*/
void step_motor_init( step_motor_t* step_motor, 
											TIM_HandleTypeDef* tim, 
											uint32_t chan, 
											GPIO_TypeDef* dir_gpio, 
											uint16_t dir_pin,
											GPIO_TypeDef* drv_en_gpio,      
											uint16_t drv_en_pin,
											uint32_t apb_freq, 
											uint32_t pwm_reso, 
											uint16_t subd)
{
	step_motor->timer = tim;
	step_motor->channel = chan;
	step_motor->dir_gpiox = dir_gpio;
	step_motor->dir_pinx = dir_pin;
	step_motor->drv_en_gpiox = drv_en_gpio;
	step_motor->drv_en_pinx = drv_en_pin;
	
	step_motor->apb_timer_clocks = apb_freq;
	step_motor->pwm_resolution = pwm_reso;
	step_motor->subdivision = subd;
	
	step_motor->f_forward = step_motor_forward;
	step_motor->f_back = step_motor_back;
	step_motor->f_enable_driver = enable_driver;
	step_motor->f_disable_driver = disable_driver;
}

/**
  * @brief  step motor forward, this function should be modifyed according to actual situation.
	* @param  None
  * @retval None
	* @auther elon hu
	* @time   2018-12-11
  */
void step_motor_forward(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  step motor forward, this function should be modifyed according to actual situation.
	* @param  None
  * @retval None
	* @auther elon hu
	* @time   2018-12-11
  */
void step_motor_back(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/**
  * @brief  step motor driver enable.
	* @param  None
  * @retval None
	* @auther elon hu
	* @time   2018-12-11
  */
void enable_driver(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  step motor driver disable.
	* @param  None
  * @retval None
	* @auther elon hu
	* @time   2018-12-11
  */
void disable_driver(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/**
  * @brief  step motor control.
	* @param[in\out]  step_motor: the structure of the motor to be used.
	* @param[in]      r_speed: rotate speed of motor.
										this parameter should be in -10.0~10.0f
	* @param[in]      lab: laps of motor rotate.
										this parameter can be one of the following values:
										@arg 0: rotate always.
										@arg >0:rotate for a specific laps.
	* @retval None
	* @auther elon hu
	* @time   2018-12-11
	*/
void step_motor_ctrl(step_motor_t* step_motor, float r_speed, uint32_t lap)
{
  float frequence;
	if(lap > 0)//rotate laps circles
	{
		step_motor->laps = lap;
		if(step_motor->rotate_ok_flag == 0)
		{
			/*frequnce number pulses a second*/
			frequence = r_speed * step_motor->subdivision;
			if(r_speed > 0)
			{
				/*frequnce number pulses a second*/
				frequence = r_speed * step_motor->subdivision;
				step_motor->f_forward(step_motor->dir_gpiox, step_motor->dir_pinx);
				step_motor->timer->Instance->PSC = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
				step_motor->f_enable_driver(step_motor->drv_en_gpiox, step_motor->drv_en_pinx);
				HAL_TIM_PWM_Start_IT(step_motor->timer,step_motor->channel);
			}
			else if(r_speed < 0)
			{
				/*frequnce number pulses a second*/
				frequence = -r_speed * step_motor->subdivision;
				step_motor->f_back(step_motor->dir_gpiox, step_motor->dir_pinx);
				step_motor->timer->Instance->PSC = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
				step_motor->f_enable_driver(step_motor->drv_en_gpiox, step_motor->drv_en_pinx);
				HAL_TIM_PWM_Start_IT(step_motor->timer,step_motor->channel);
			}
			else if(r_speed == 0)
			{
				step_motor->f_disable_driver(step_motor->drv_en_gpiox, step_motor->drv_en_pinx);
				HAL_TIM_PWM_Stop_IT(step_motor->timer,step_motor->channel);
			}
		}
		else
		{
				step_motor->f_disable_driver(step_motor->drv_en_gpiox, step_motor->drv_en_pinx);
				HAL_TIM_PWM_Stop_IT(step_motor->timer,step_motor->channel);
		}
	}
	else if(lap == 0)//rotate always
	{
		
		if(r_speed > 0)
		{
			/*frequnce number pulses a second*/
			frequence = r_speed * step_motor->subdivision;
			step_motor->f_forward(step_motor->dir_gpiox, step_motor->dir_pinx);
			step_motor->timer->Instance->PSC = (step_motor->apb_timer_clocks / frequence) / step_motor->pwm_resolution - 1;
			step_motor->f_enable_driver(step_motor->drv_en_gpiox, step_motor->drv_en_pinx);
			HAL_TIM_PWM_Start(step_motor->timer,step_motor->channel);
		}
		else if(r_speed < 0)
		{
			/*frequnce number pulses a second*/
			frequence = -r_speed * step_motor->subdivision;
			step_motor->f_back(step_motor->dir_gpiox, step_motor->dir_pinx);
			step_motor->timer->Instance->PSC = (step_motor->apb_timer_clocks / frequence) / step_motor->pwm_resolution - 1;
			step_motor->f_enable_driver(step_motor->drv_en_gpiox, step_motor->drv_en_pinx);
			HAL_TIM_PWM_Start(step_motor->timer,step_motor->channel);
		}
		else if(r_speed == 0)
		{
			step_motor->f_disable_driver(step_motor->drv_en_gpiox, step_motor->drv_en_pinx);
			HAL_TIM_PWM_Stop(step_motor->timer,step_motor->channel);
		}
	}
}

/*put this into interrupt function*/
/**
  * @brief step motor pulses count.
					 this function should be called by the corresponding timer intrrupt function
	* @param[in\out]  step_motor: the structure of the motor to be used.
	* @retval None
	* @auther elon hu
	* @time   2018-12-11
	*/
void step_motor_pulse_cnt(step_motor_t* step_motor)
{
	if(__HAL_TIM_GET_IT_SOURCE(step_motor->timer ,TIM_IT_CC2 )!=RESET )
		{
			step_motor->pulse_cnt++;
		}
		__HAL_TIM_CLEAR_IT(step_motor->timer ,TIM_IT_CC2 );
		
		if(step_motor->pulse_cnt == step_motor->laps * step_motor->subdivision)
		{
			step_motor->rotate_ok_flag = 1;
			step_motor->pulse_cnt = 0;
			HAL_TIM_PWM_Stop_IT(step_motor->timer,step_motor->channel);
		}
}

