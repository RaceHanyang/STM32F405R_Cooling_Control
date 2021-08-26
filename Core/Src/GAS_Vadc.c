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
volatile uint16_t ValueOfADC[5];
CoolingADC_t CoolingADC;

void GAS_Vadc_init(void);
void GAS_Vadc_dmaIn(void);
void GAS_Vadc_getValue(void);



void GAS_Vadc_init(void)
{
	if(HAL_ADC_Start_DMA(&hadc1, ValueOfADC, NumberOfVadc) != HAL_OK)
	{
		Error_Handler();
	}
}

static uint16_t calcTemp(uint16_t ADCVal){
	float ratio = 1;
	uint16_t rawtemp = pow(0.467,(ADCVal-5.01))+13.875;
	return (int)(ratio * rawtemp);
}

void GAS_Vadc_getValue(void)
{
	CoolingADC.Radiator0_IS=ValueOfADC[0];
	CoolingADC.Remain=ValueOfADC[1];
	CoolingADC.External_IS=ValueOfADC[2];
	CoolingADC.WaterPump_P=ValueOfADC[3];
	CoolingADC.Radiator0_T=calcTemp(ValueOfADC[4]);
	CoolingADC.WaterPump1_IS=ValueOfADC[5];
	CoolingADC.Radiator1_IS=calcTemp(ValueOfADC[6]);
	CoolingADC.WaterPump1_IS=ValueOfADC[7];

}
