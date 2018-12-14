/*
本文件包含了一些跟gpio相关的常用代码操作
*/

/*以下是.C文件部分模板内容*/
/*
param[in]:闪烁次数
param[in]:一次闪烁中间间隔时间
param[in]:两次闪烁之间的间隔时间
*/
void led_blink(uint8_t time, uint16_t time1, uint16_t time2)
{
	uint8_t i;
	for(i = 0; i < time; i++)
	{
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
		osDelay(time1);
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
		osDelay(time1);
	}
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
	osDelay(time2);
}

/*以下是.H文件部分模板内容*/
//引脚定义，这部分CubeMX可以配置
/*******************************************************/
#define LED1_PIN                  GPIO_Pin_10                 
#define LED1_GPIO_PORT            GPIOH                      

#define LED2_PIN                  GPIO_Pin_11                 
#define LED2_GPIO_PORT            GPIOH                      

#define LED3_PIN                  GPIO_Pin_12                 
#define LED3_GPIO_PORT            GPIOH                       

#define LED4_PIN                  GPIO_Pin_11                 
#define LED4_GPIO_PORT            GPIOD                       
/************************************************************/

/*
控制LED亮灭的宏，这里是低电平有效，ON=0,OFF=1
*/
    #define ON  0
    #define OFF 1
/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	if (a)	\
        GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);\
        else		\
        GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN)

#define LED2(a)	if (a)	\
        GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);\
        else		\
        GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN)

#define LED3(a)	if (a)	\
        GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);\
        else		\
        GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN)
        
#define LED4(a)	if (a)	\
        GPIO_SetBits(LED4_GPIO_PORT,LED4_PIN);\
        else		\
        GPIO_ResetBits(LED4_GPIO_PORT,LED4_PIN)

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}		    //输出高电平	
#define digitalLo(p,i)			{p->BSRRH=i;}		    //输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转电平

/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF		digitalHi(LED1_GPIO_PORT,LED1_PIN)
#define LED1_ON			digitalLo(LED1_GPIO_PORT,LED1_PIN)

#define LED2_TOGGLE		digitalToggle(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF		digitalHi(LED2_GPIO_PORT,LED2_PIN)
#define LED2_ON			digitalLo(LED2_GPIO_PORT,LED2_PIN)

#define LED3_TOGGLE		digitalToggle(LED3_GPIO_PORT,LED3_PIN)
#define LED3_OFF		digitalHi(LED3_GPIO_PORT,LED3_PIN)
#define LED3_ON			digitalLo(LED3_GPIO_PORT,LED3_PIN)

#define LED4_TOGGLE		digitalToggle(LED4_GPIO_PORT,LED4_PIN)
#define LED4_OFF		digitalHi(LED4_GPIO_PORT,LED4_PIN)
#define LED4_ON			digitalLo(LED4_GPIO_PORT,LED4_PIN)

/* RGB—LED控制 */
//红
#define LED_RED  \
LED1_ON;\
LED2_OFF\
LED3_OFF

//绿
#define LED_GREEN		\
LED1_OFF;\
LED2_ON\
LED3_OFF

//蓝
#define LED_BLUE	\
LED1_OFF;\
LED2_OFF\
LED3_ON


//黄（红+绿）				
#define LED_YELLOW	\
LED1_ON;\
LED2_ON\
LED3_OFF
//紫（红+蓝）
#define LED_PURPLE	\
LED1_ON;\
LED2_OFF\
LED3_ON

//青（绿+蓝）
#define LED_CYAN \
LED1_OFF;\
LED2_ON\
LED3_ON

//白（红+绿+蓝）
#define LED_WHITE	\
LED1_ON;\
LED2_ON\
LED3_ON

//黑（全部关闭）
#define LED_RGBOFF	\
LED1_OFF;\
LED2_OFF\
LED3_OFF




