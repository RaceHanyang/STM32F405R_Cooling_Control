/*
 * GAS_Vadc.c
 *
 *  Created on: 2021. 08. 01.
 *      Author: Suprhimp
 */
#include <GAS_Can.h>
#include <GAS_Vadc.h>
#include "stdio.h"
#include "math.h"

uint32_t NumberOfVadc = 8;
volatile uint32_t ValueOfADC[8];
CoolingADC_t Cool6ingADC;

void GAS_Vadc_init(void);
void GAS_Vadc_dmaIn(void);
void GAS_Vadc_getValue(void);



void GAS_Vadc_init(void)
{
	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ValueOfADC, NumberOfVadc) != HAL_OK)
	{
		Error_Handler();
	}
}

static uint16_t calcTemp(float ADCVal){
	float V = (float)(ADCVal*3.3/4095);
	float temp = 0.0048*exp(3.0409*V);
	return (int)(temp*10);
//	return ;
}

void GAS_Vadc_getValue(void)
{
	/*
	 * FROM f48d32
	 * TO-DO!!!
	 *
	 * Inquire Voltage -> Meaningful Measurement conversion method.
	 * Modify NumberOfVadc 8->10? 9 if RANK2 removed?
	 * Length of ValueOfADC shall to be updated properly.
	 * Implement the conversions!
	 * */

	// R = 0.1 -> product 10
	CoolingADC.Radiator0_IS=ValueOfADC[0]*10;
	CoolingADC.Remain=ValueOfADC[1]*10; ///Unused ADC PIN?
	CoolingADC.External_IS=ValueOfADC[2]*10;
	CoolingADC.WaterPump_P=ValueOfADC[3]*10;
	CoolingADC.Radiator0_T=calcTemp(ValueOfADC[4]);			//Left RadTemp
	CoolingADC.WaterPump0_IS=ValueOfADC[5]*10;
	CoolingADC.Radiator1_T=calcTemp(ValueOfADC[3]);			//Right RadTemp

	CoolingADC.WaterPump1_IS=ValueOfADC[7]*10;

}
