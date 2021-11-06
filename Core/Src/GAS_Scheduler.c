/*
 * GAS_Scheduler.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Suprhimp
 */

#include "GAS_Scheduler.h"
#include "stdio.h"
#include "tim.h"

extern uint8_t canRx0Data[8];
//extern volatile uint32_t ValueOfADC[8];

uint8_t pin_state;

#define False 0
#define True 1

volatile int gTimerCnt;
volatile uint8_t pwmChangeFlag;
Scheduler_task gTask;

void GAS_Scheduler_init(void);
void GAS_Scheduler_taskCounter_1ms(void);
void GAS_Scheduler_taskCounter_10ms(void);
void GAS_Scheduler_taskCounter_100ms(void);
void GAS_Scheduler_taskCounter_1000ms(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void GAS_Schduler(void);

void GAS_Scheduler_init(void)
{

	SysTick_Config(SystemCoreClock/1000);

//********CAN initialization********
	GAS_Can_init();

//********PWM initialization********
	GAS_CCTL_outputInit();
//	GAS_PWM_outputInit();

//********ADC initialization*******
	GAS_Vadc_init();
}



void GAS_Scheduler_taskCounter_1ms(void)
{
	GAS_Vadc_getValue();

//	GAS_Can_sendMessage();
//	GAS_PWM_changeOutput_ch1(ValueOfADC[0]);
}

void GAS_Scheduler_taskCounter_10ms(void)
{
	GAS_Can_sendMessage();
	GAS_CCTL_Control();
}

void GAS_Scheduler_taskCounter_100ms(void)
{
	HAL_GPIO_TogglePin(GPIOB, BlueLED_Pin);
}
void GAS_Scheduler_taskCounter_1000ms(void)
{
//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
//		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
}

void HAL_SYSTICK_Callback(void){
	gTimerCnt++;
			gTask.flag_1ms=True;
			if(gTimerCnt % 10 == 0)
			{
				gTask.flag_10ms=True;
			}
			if(gTimerCnt % 100 == 0)
			{
				gTask.flag_100ms=True;
			}

			if(gTimerCnt == 1000)
			{
				gTask.flag_1000ms=True;
				gTimerCnt = 0;
			}
}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	gTimerCnt++;
//
//	gTask.flag_1ms=True;
//
//	if(gTimerCnt % 10 == 0)
//	{
//		gTask.flag_10ms=True;
//	}
//
//	if(gTimerCnt % 100 == 0)
//	{
//		gTask.flag_100ms=True;
//	}
//
//	if(gTimerCnt == 1000)
//	{
//		gTask.flag_1000ms=True;
//		gTimerCnt = 0;
//	}
//}

void GAS_Scheduler(void)
{


	if(gTask.flag_1ms == True)
	{
		gTask.flag_1ms = False;
		GAS_Scheduler_taskCounter_1ms();
	}

	if(gTask.flag_10ms == True)
	{
		gTask.flag_10ms = False;
		GAS_Scheduler_taskCounter_10ms();
	}

	if(gTask.flag_100ms == True)
	{
		gTask.flag_100ms = False;
		GAS_Scheduler_taskCounter_100ms();
	}

	if(gTask.flag_1000ms == True)
	{
		gTask.flag_1000ms = False;
		GAS_Scheduler_taskCounter_1000ms();
	}

//	GAS_Can_recieveMessage(&hcan);
//	HAL_CAN_IRQHandler(&hcan);
//	  pin_state=HAL_GPIO_ReadPin(GPIO_PIN_8);
}
