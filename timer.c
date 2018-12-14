

/*
时基结构体IM_Base_InitTypeDef中有5个成员，TIM6和TIM7（基础定时器）寄存器只有Prescaler和Period，
所以使用TIM6和TIM7的时候只需要初始化这两个
typedef struct
{
  uint32_t Prescaler;         //分频率，最终传入PSC寄存器（驱动CNT计数器的时钟：CLK_cnt = Fck_int / (psc + 1) = 180M / (psc + 1)）：0x0000U - 0xFFFFU，都有
  uint32_t CounterMode;       // 计数模式：向上、向下、中间，TIMx,x[6,7]没有
  uint32_t Period;            //计数满值(自动重装载寄存器ARR的值，累计period+1个周期后产生中断)：0x0000U - 0xFFFFU，都有
  uint32_t ClockDivision;     //时钟分频，通常默认不分频（时钟分频因子，配置死区时间用），TIMx,x[6,7]没有
  uint32_t RepetitionCounter; //重复计数器的值，0x00 - 0xFF，只有TIMx,x[1,8]才有                              
} TIM_Base_InitTypeDef;
*/
/*
输出比较结构体TIM_OC_InitTypeDef
typedef struct
{
  uint32_t OCMode;       //输出模式，可以是PWM输出模式
  uint32_t Pulse;        //占空比大小，该值可以根据时基结构体中Period的数值来设置，不可大于Period的数值，对应占空比0%-100%
  uint32_t OCPolarity;   //输出通道电平极性配置
  uint32_t OCNPolarity;  //输出通道空闲电平极性配置，只有TIMx,x[1,8]才有
  uint32_t OCFastMode;   //只有PWM1 and PWM2才有
  uint32_t OCIdleState;  //只有TIMx,x[1,8]才有  
  uint32_t OCNIdleState; //只有TIMx,x[1,8]才有
} TIM_OC_InitTypeDef;  
*/


/*************************************************************************************************/
/****************************************PWM相关模板代码*******************************************/
/*************************************************************************************************/

#define PWM_FREQUENCE 50 //默认PWM频率：F = APB / ((ARR + 1) * (PSC + 1))
#define PWM_RESOLUTION 10000  //自动重装载寄存器Period的值，使用的时候记得-1
#define PWM_DEFAULT_DUTY 5000 //默认占空比，50%

#define TIM_PSC_APB1 ((APB1_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION) //APB1分频率，使用的时候记得-1
#define APB1_TIMER_CLOCKS 90000000 //APB1主频


/*开启，关闭PWM输出*/
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
/*
修改PWM占空比0.0-1.0
*/
void pwm_set_duty(TIM_HandleTypeDef *tim, uint32_t tim_channel, float duty)
{	
    switch(tim_channel)
    {	
		case TIM_CHANNEL_1: tim->Instance->CCR1 = (PWM_RESOLUTION*duty) - 1;break;
		case TIM_CHANNEL_2: tim->Instance->CCR2 = (PWM_RESOLUTION*duty) - 1;break;
		case TIM_CHANNEL_3: tim->Instance->CCR3 = (PWM_RESOLUTION*duty) - 1;break;
		case TIM_CHANNEL_4: tim->Instance->CCR4 = (PWM_RESOLUTION*duty) - 1;break;
	}
}
/*
修改PWM频率0-90,000,000Hz
*/
void pwm_set_frequence(TIM_HandleTypeDef *tim, float frequence)
{	
    __IO uint32_t tim_psc_apb1;
    tim_psc_apb1 = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
    tim->Instance->PSC = tim_psc_apb1;
}


#define SUBDIVISION (200)
/*
步进电机控制
param[in]:电机号
param[in]:细分数
param[in]:转速
param[in]:方向 FORWARD REVERSE
*/

void step_rotat_speed(TIM_HandleTypeDef *motor, uint16_t subd, float r_speed, uint8_t dir)
{
  float frequence;
  __IO uint32_t tim_psc_apb1;
	
	if(dir == FORWARD)
	{
		HAL_GPIO_WritePin(STEP_DIRECTION_GPIO_Port, STEP_DIRECTION_Pin, GPIO_PIN_SET);//正向旋转，刹车
	}
	else
	{
		HAL_GPIO_WritePin(STEP_DIRECTION_GPIO_Port, STEP_DIRECTION_Pin, GPIO_PIN_RESET);
	}
  frequence = r_speed * subd;
	if(r_speed)
	{
		tim_psc_apb1 = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
	}
	else
	{
		tim_psc_apb1 = (APB1_TIMER_CLOCKS / 0.1) / PWM_RESOLUTION -1;
	}
  
  motor->Instance->PSC = tim_psc_apb1;
}

/*
步进电机控制
param[in]:电机号
param[in]:细分数
param[in]:转速
param[in]:方向
param[in]:位置（角度）
*/
void step_rotat_speed_pos(TIM_HandleTypeDef *motor, uint16_t subd, float r_speed, uint8_t dir, double pos)
{
  float frequence;
  __IO uint32_t tim_psc_apb1;
	
	if(dir == FORWARD)
	{
		HAL_GPIO_WritePin(STEP_DIRECTION_GPIO_Port, STEP_DIRECTION_Pin, GPIO_PIN_SET);//正向旋转，刹车
	}
	else
	{
		HAL_GPIO_WritePin(STEP_DIRECTION_GPIO_Port, STEP_DIRECTION_Pin, GPIO_PIN_RESET);
	}
  frequence = r_speed * subd;
	if(r_speed)
	{
		tim_psc_apb1 = (APB1_TIMER_CLOCKS / frequence) / PWM_RESOLUTION -1;
	}
	else
	{
		tim_psc_apb1 = (APB1_TIMER_CLOCKS / 0.1) / PWM_RESOLUTION -1;
	}
  
  motor->Instance->PSC = tim_psc_apb1;
}



/*高级定时器用这个，TIMx,x[1,8]*/
// #define TIM_PSC_APB2 ((APB2_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1) //APB2分频率
// #define APB2_TIMER_CLOCKS 180000000 //APB2主频

/*
PWM信号 周期和占空比计算
ARR：自动重装载寄存器的值
CCR：
计数器时钟：CLK_cnt = Fck_int / (psc + 1) = 180M / (psc + 1)
PWM信号周期：T = (ARR + 1) * (1 / CLK_cnt) = (ARR + 1) * (PSC + 1) / 72M
占空比：P = CCR / (ARR + 1)
*/


/*-------------------------------------以下部分为步进电机控制相关--------------------------------*/
/*
控制步进电机需要的资源有
一个控制使能的GPIO
一个控制方向的GPIO
PWM信号频率计算公式： F = APB / ((ARR + 1) * (PSC + 1))
步进电机转速计算： speed = （PWM信号频率）F/（细分*单圈步数）
*/
#define LEFT 0
#define RIGHT 1

#define Pulse_width 20

#define T1_FREQ 1000000     //定时器频率
#define FSPR    200         //步进电机单圈步数
#define SPR     (FSPR*100)  //100细分的单圈步数
