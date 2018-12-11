/**
  ******************************************************************************
  * @file    bsp_step_motor.c
  * @author  elon hu
  * @tiem    2018-12-11
  * @brief   step motor driver.
  *          This file provides firmware functions to manage the functionalities
  *          of the step motor.
  ******************************************************************************
  */ 

#include "bsp_step_motor.h"
#include "sys_config.h"

/**
	* @brief  initialize step motor.
	* @param[in\out]  step_motor: the structure of the motor to be used.
	* @param[in]      tim: timer that motor used.
	* @param[in]      chan: channel that motor used.
	* @param[in]      gpio: gpio that motor used.
	* @param[in]      pin: gpio that motor used.
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
											GPIO_TypeDef* gpio, 
											uint16_t pin, 
											uint32_t apb_freq, 
											uint32_t pwm_freq, 
											uint16_t subd)
{
	step_motor->timer = tim;
	step_motor->channel = chan;
	step_motor->gpiox = gpio;
	step_motor->pinx = pin;
	
	step_motor->apb_timer_clocks = apb_freq;
	step_motor->pwm_frequncy = pwm_freq;
	step_motor->subdivision = subd;
	
	step_motor->f_forward = step_motor_forward;
	step_motor->f_back = step_motor_back;
}

/**
  * @brief  step motor forward, this function should be modifyed according to actual situation.
	* @param[in] GPIOx: gpio that motor direction used.
	* @param[in] GPIO_Pin: gpio that motor direction used.
  * @retval None
	* @auther elon hu
	* @time   2018-12-11
  */
void step_motor_forward(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	/*user can change here by self*/
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/**
  * @brief  step motor forward, this function should be modifyed according to actual situation.
	* @param[in] GPIOx: gpio that motor direction used.
	* @param[in] GPIO_Pin: gpio that motor direction used.
  * @retval None
	* @auther elon hu
	* @time   2018-12-11
  */
void step_motor_back(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	/*user can change here by self*/
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
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
				step_motor->f_forward(step_motor->gpiox, step_motor->pinx);
				step_motor->timer->Instance->PSC = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
				HAL_TIM_PWM_Start(step_motor->timer,step_motor->channel);
			}
			else if(r_speed < 0)
			{
				step_motor->f_back(step_motor->gpiox, step_motor->pinx);
				step_motor->timer->Instance->PSC = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
				HAL_TIM_PWM_Start(step_motor->timer,step_motor->channel);
			}
			else if(r_speed == 0)
			{
				HAL_TIM_PWM_Stop(step_motor->timer,step_motor->channel);
			}
		}
	}
	else if(lap == 0)//rotate always
	{
		/*frequnce number pulses a second*/
		frequence = r_speed * step_motor->subdivision;
		if(r_speed > 0)
		{
			step_motor->f_forward(step_motor->gpiox, step_motor->pinx);
			step_motor->timer->Instance->PSC = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
			HAL_TIM_PWM_Start(step_motor->timer,step_motor->channel);
		}
		else if(r_speed < 0)
		{
			step_motor->f_back(step_motor->gpiox, step_motor->pinx);
			step_motor->timer->Instance->PSC = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
			HAL_TIM_PWM_Start(step_motor->timer,step_motor->channel);
		}
		else if(r_speed == 0)
		{
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
	if(__HAL_TIM_GET_IT_SOURCE(step_motor->timer ,TIM_IT_UPDATE )!=RESET )
		{
			step_motor->pulse_cnt++;
		}
		__HAL_TIM_CLEAR_IT(step_motor->timer ,TIM_IT_UPDATE );
		
		if(step_motor->pulse_cnt == step_motor->laps * step_motor->subdivision)
		{
			step_motor->rotate_ok_flag = 1;
			step_motor->pulse_cnt = 0;
			HAL_TIM_PWM_Stop(step_motor->timer,step_motor->channel);
		}
}

