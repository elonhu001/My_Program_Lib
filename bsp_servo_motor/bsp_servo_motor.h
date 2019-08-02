 #ifndef __BSP_SERVO_MOTOR_H
 #define __BSP_SERVO_MOTOR_H
 typedef struct
{
	float   angle_aim;
	float   angle_curr;
    float   accuracy;
	uint8_t servo_ready_flag;
	TIM_HandleTypeDef *tim;
	uint32_t tim_channel;
}servo_t;
#define PWM_FREQUENCE 50
#define PWM_RESOLUTION 10000
#define APB1_TIMER_CLOCKS 72000000
#define PWM_DEFAULT_DUTY 5000
#define TIM_PSC_APB1 (APB1_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION

void pwm_set_duty(TIM_HandleTypeDef *tim,uint32_t tim_channel,float duty);
float map(float in_val, float in_min, float in_max, float out_min, float out_max);
void servo_init(servo_t *servo, TIM_HandleTypeDef *timer, uint32_t channel, float angle);
void servo_ctrl(servo_t *servo, float ang, float accu);

 #endif



