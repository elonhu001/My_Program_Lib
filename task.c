/*以下是freertos.c文件部分模板内容*/
#include "task_name_task.h"

osThreadId task_name_TaskHandle;

osThreadDef(task_name_task, start_task_name_task, osPriorityNormal, 0, 128);
task_name_TaskHandle = osThreadCreate(osThread(task_name_task), NULL);

//通常在DefaultTask中添加led_blink()作为系统运行提示灯

/*以下是.C文件部分模板内容*/
#include "task_name_task.h"
#include "cmsis_os.h"//RTOS标准库
#include "sys_config.h"//用于工程系统配置
/*C标准库，根据情况使用*/
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"


void start_task_name_task(void const * argument)
{
  uint32_t task_name_wake_time = osKernelSysTick();
  while(1)
  {

    osDelayUntil(&task_name_wake_time, TASK_NAME_TASK_PERIOD);
  }
}

/*以下是.H文件部分模板内容*/
#ifndef __TASK_NAME_TASK_H
#define __TASK_NAME_TASK_H

#include "stm32f4xx_hal.h"
#include "sys_config.h"

#define TASK_NAME_TASK_PERIOD (5)//任务运行周期ms


#endif