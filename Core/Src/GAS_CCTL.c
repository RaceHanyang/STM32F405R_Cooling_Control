/*
 * GAS_PWM.c
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 */

#include <GAS_CCTL.h>
#include "stdio.h"

TC_switch_t TC_switch;
TC_order_t TC_order;
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

//static void GAS_CCTL_onAll(void){
//	HAL_GPIO_WritePin(GPIOB,ExternalFan_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Radiator0_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Radiator1_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,WaterPump0_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Waterpump1_INH_Pin, GPIO_PIN_SET);
//}
//
//static void GAS_CCTL_offAll(void){
//	HAL_GPIO_WritePin(GPIOB,ExternalFan_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Radiator0_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Radiator1_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,WaterPump0_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Waterpump1_INH_Pin, GPIO_PIN_SET);
//}


void GAS_CCTL_Control(void){

	float a0 = 2.2;
	uint8_t b0 = 6;
	float a1 = 9.8;
	int16_t b1 = -150;
	uint8_t duty0 = (int)(a0*CoolingADC.Radiator0_T + b0);			//Radiator0,1_T => celcius degree of coolant producted by 10.
	uint8_t duty1 = (int)(a1*CoolingADC.Radiator1_T + b1);

	if (1){//TC_switch.B.AutoMode_ON){
		TIM1 -> CCR1 = duty1;  //External FAN
		TIM8 -> CCR1 = 50;//duty1;  //Radiator fan 1  (right)
		TIM8 -> CCR2 = 50;//duty1;	 //Water pump 1     (right)
		TIM8 -> CCR3 = 50;//duty0;  //Radiator 0	   (left)
		TIM8 -> CCR4 = 50;//duty0;	 //Water pump 0      (left)
	}
	else{
		uint8_t defaultDuty = TC_order.B.defaultDutyOrder;
		TIM1 -> CCR1 = TC_switch.B.OnAll & TC_switch.B.ExternalFan ?  defaultDuty + TC_order.B.ExternalFan_DutyOrder : 0;// + CoolingADC.External_IS * Pe;
		TIM8 -> CCR1 = TC_switch.B.OnAll & TC_switch.B.Radiator1_ON ?  defaultDuty + TC_order.B.Radiator0_DutyOrder : 0;// + CoolingADC.Radiator_T * Pr;
		TIM8 -> CCR2 = TC_switch.B.OnAll & TC_switch.B.WaterPump1_ON ? defaultDuty + TC_order.B.WaterPump0_DutyOrder: 0;// + CoolingADC.WaterPump_P * Pw;
		TIM8 -> CCR3 = TC_switch.B.OnAll & TC_switch.B.Radiator0_ON ? defaultDuty + TC_order.B.Radiator1_DutyOrder : 0;// + CoolingADC.Radiator_T * Pr;
		TIM8 -> CCR4 = TC_switch.B.OnAll & TC_switch.B.WaterPump1_ON ? defaultDuty + TC_order.B.WaterPump1_DutyOrder : 0;// + CoolingADC.WaterPump_P * Pw;
	}




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
	stm32.B.Radiator0_T = CoolingADC.Radiator0_T;
	stm32.B.Radiator1_T = CoolingADC.Radiator1_T;
}


