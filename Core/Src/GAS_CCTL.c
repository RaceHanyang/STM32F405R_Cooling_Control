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

uint16_t StartCount = 0;


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
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

}

static void GAS_CCTL_onAll(void){
	HAL_GPIO_WritePin(GPIOB,Radiator0_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Radiator1_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,WaterPump0_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Waterpump1_INH_Pin, GPIO_PIN_SET);
}

static void GAS_CCTL_offAll(void){
	HAL_GPIO_WritePin(GPIOB,ExternalFan_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Radiator0_INH_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Radiator1_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,WaterPump0_INH_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,Waterpump1_INH_Pin, GPIO_PIN_SET);
}


void GAS_CCTL_Control(void){
	GAS_CCTL_onAll();
	if (StartCount < 100){

		StartCount+=1;
	}
	else{
		float a0 = 0.37;
			float b0 = -30.5;
			float a1 = 0.925;
			float b1 = -113.75;
			float a2 = (float)(49/74);
			float b2 = (float)(2475/74);

			volatile int8_t duty0 = (int)(CoolingADC.Radiator0_T*a0 + b0) ;			//Radiator0,1_T => celcius degree of coolant producted by 10.
			volatile int8_t duty1 = (int)(CoolingADC.Radiator1_T*a1 + b1);



			if (duty1>99)duty1 = 99;
			if (duty1<30)duty1 = 30;
			if (duty0>99)duty0 = 99;
			if (duty0<30)duty0=30;

			if (duty1>49){
				HAL_GPIO_WritePin(GPIOB,ExternalFan_INH_Pin, GPIO_PIN_SET);
			}else{
				HAL_GPIO_WritePin(GPIOB,ExternalFan_INH_Pin, GPIO_PIN_RESET);

			}


			if (TC_switch.B.manualMode_ON!=1){
				TIM1 -> CCR1 = duty1;  //External FAN
				TIM3 -> CCR1 = duty1;  //Radiator fan 1  (right)
				TIM8 -> CCR2 = duty1;	 //Water pump 1     (right)
				TIM3 -> CCR3 = duty0;  //Radiator 0	   (left)
				TIM8 -> CCR4 = duty0;	 //Water pump 0      (left)
			}
			if(TC_switch.B.manualMode_ON){
				uint8_t defaultDuty = TC_order.B.defaultDutyOrder;
				TIM1 -> CCR1 = TC_order.B.ExternalFan_DutyOrder ;// + CoolingADC.External_IS * Pe;
				TIM3 -> CCR1 = TC_order.B.Radiator0_DutyOrder ;// + CoolingADC.Radiator_T * Pr;
				TIM8 -> CCR2 = TC_order.B.WaterPump0_DutyOrder;// + CoolingADC.WaterPump_P * Pw;
				TIM3 -> CCR3 = TC_order.B.Radiator1_DutyOrder ;// + CoolingADC.Radiator_T * Pr;
				TIM8 -> CCR4 = TC_order.B.WaterPump1_DutyOrder ;// + CoolingADC.WaterPump_P * Pw;
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
		//	stm32.B.WaterPump_P = (uint8_t)(CoolingADC.WaterPump_P*Kp);
			stm32.B.Radiator0_T = CoolingADC.Radiator0_T;
			stm32.B.Radiator1_T = CoolingADC.Radiator1_T;
			stm32.B.manualMode_isON = TC_switch.B.manualMode_ON;

	}
}


