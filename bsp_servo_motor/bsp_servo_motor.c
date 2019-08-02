#include "bsp_servo_motor.h"
//  htim1.Init.Prescaler = TIM_PSC_APB1-1;
//  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim1.Init.Period = PWM_RESOLUTION-1;


void pwm_set_duty(TIM_HandleTypeDef *tim,uint32_t tim_channel,float duty)
{
	switch(tim_channel)
	{	
		case TIM_CHANNEL_1: tim->Instance->CCR1 = (PWM_RESOLUTION*duty*0.01) - 1;break;
		case TIM_CHANNEL_2: tim->Instance->CCR2 = (PWM_RESOLUTION*duty*0.01) - 1;break;
		case TIM_CHANNEL_3: tim->Instance->CCR3 = (PWM_RESOLUTION*duty*0.01) - 1;break;
		case TIM_CHANNEL_4: tim->Instance->CCR4 = (PWM_RESOLUTION*duty*0.01) - 1;break;
	}
}

float map(float in_val, float in_min, float in_max, float out_min, float out_max)
{
	float k, b, out_val;
	k = (out_max - out_min) / (in_max - in_min);
	b = out_max - (k * in_max);
	out_val = k * in_val + b;
	return out_val;
}


void servo_init(servo_t *servo, TIM_HandleTypeDef *timer, uint32_t channel, float angle)
{
    float dut;
    servo->angle_aim = servo->angle_curr = angle;
    servo->tim = timer;
    servo->tim_channel = channel;
    dut = map(servo->angle_curr, 0, 180, 2.5, 12.5);
    PWM_SetDuty(&servo->tim, servo->tim_channel, dut); 
}

void servo_ctrl(servo_t *servo, float ang, float accu)
{
	float dut;
    servo->angle_aim = ang;
    servo->accuracy = accu;
	if(servo->angle_aim > servo->angle_curr)
	{
		servo->angle_curr += servo->accuracy;
		if(servo->angle_aim < servo->angle_curr)
		{
			servo->angle_curr = servo->angle_aim;
			servo->servo_ready_flag = 1;
		}
		dut = map(servo->angle_curr, 0, 180, 2.5, 12.5);
		PWM_SetDuty(&servo->tim, servo->tim_channel, dut); 
	}
	else if(servo->angle_aim < servo->angle_curr)
	{
		servo->angle_curr -= servo->accuracy;
		if(servo->angle_aim > servo->angle_curr)
		{
			servo->angle_curr = servo->angle_aim;
			servo->servo_ready_flag = 1;
		}
		dut = map(servo->angle_curr, 0, 180, 2.5, 12.5);
		PWM_SetDuty(&servo->tim, servo->tim_channel, dut); 
	}
	else if(servo->angle_aim == servo->angle_curr)
	{
		dut = map(servo->angle_curr, 0, 180, 2.5, 12.5);
		PWM_SetDuty(&servo->tim, servo->tim_channel, dut); 
	}
	else{}
}