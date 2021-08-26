/*
 * GAS_PWM.c
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 */

#include <GAS_CCTL.h>
#include "stdio.h"

TC_msg_t TC_msg;
stm32_msg_t stm32;
CoolingADC_t CoolingADC;




void GAS_CCTL_outputInit(void);
void GAS_CCTL_change_ExternalFAN(uint8_t Duty);
void GAS_CCTL_change_WaterPump(uint8_t Duty);
void GAS_CCTL_change_RadiatorFAN(uint8_t Duty);
static void GAS_CCTL_onAll(void);
static void GAS_CCTL_offAll(void);
void GAS_CCTL_Control(void);

void GAS_CCTL_outputInit(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

}

static void GAS_CCTL_onAll(void){
	HAL_GPIO_WritePin(GPIOB,ExternalFan_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Radiator0_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Radiator1_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,WaterPump0_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Waterpump1_INH_Pin, GPIO_PIN_SET);
}

static void GAS_CCTL_offAll(void){
	HAL_GPIO_WritePin(GPIOB,ExternalFan_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Radiator0_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Radiator1_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,WaterPump0_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Waterpump1_INH_Pin, GPIO_PIN_SET);
}


void GAS_CCTL_Control(void){

	if (TC_msg.B.killAll)GAS_CCTL_offAll();

	if(TC_msg.B.OnAll)GAS_CCTL_onAll();

	uint8_t defaultDuty = 30;
//	TIM1 -> CCR1 = defaultDuty + TC_msg.B.ExternalFan_DutyOrder;// + CoolingADC.External_IS * Pe;
//	TIM8 -> CCR1 = defaultDuty + TC_msg.B.Radiator0_DutyOrder;// + CoolingADC.Radiator_T * Pr;
//	TIM8 -> CCR2 = defaultDuty + TC_msg.B.WaterPump0_DutyOrder;// + CoolingADC.WaterPump_P * Pw;
//	TIM8 -> CCR3 = defaultDuty + TC_msg.B.Radiator1_DutyOrder;// + CoolingADC.Radiator_T * Pr;
//	TIM8 -> CCR4 = defaultDuty + TC_msg.B.WaterPump1_DutyOrder;// + CoolingADC.WaterPump_P * Pw;
	TIM1 -> CCR1 = defaultDuty;
	TIM8 -> CCR1 = defaultDuty;
	TIM8 -> CCR2 = defaultDuty;
	TIM8 -> CCR3 = defaultDuty;
	TIM8 -> CCR4 = defaultDuty;


	float Ke = 0.5;
	float Kr = 0.5;
	float Kw = 0.5;
	float Kp = 0.5;

	stm32.B.External_IS = (uint8_t)(CoolingADC.External_IS * Ke);
	stm32.B.Radiator0_IS = (uint8_t)(CoolingADC.Radiator0_IS * Kr);
	stm32.B.Radiator1_IS = (uint8_t)(CoolingADC.Radiator1_IS* Kr);
	stm32.B.WaterPump0_IS = (uint8_t)(CoolingADC.WaterPump0_IS*Kw);
	stm32.B.WaterPump1_IS = (uint8_t)(CoolingADC.WaterPump1_IS*Kw);
	stm32.B.WaterPump_P = (uint8_t)(CoolingADC.WaterPump_P*Kp);

}


