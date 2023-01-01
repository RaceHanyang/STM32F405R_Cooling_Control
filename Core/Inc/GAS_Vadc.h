/*
 * GAS_Vadc.h
 *
 *  Created on: 2020. 10. 30.
 *      Author: Suprhimp
 */

#ifndef INC_GAS_VADC_H_
#define INC_GAS_VADC_H_

#include "dma.h"
#include "adc.h"

extern void GAS_Vadc_init(void);
extern void GAS_Vadc_dmaIn(void);
extern void GAS_Vadc_getValue(void);

typedef struct{
	uint16_t External_IS;
	uint16_t WaterPump0_IS;
	uint16_t WaterPump1_IS;
	uint16_t Radiator0_IS;
	uint16_t Radiator1_IS;
	//uint16_t WaterPump_P;
	uint16_t Radiator0_T;
	uint16_t Radiator1_T;
	//uint16_t Remain;
	uint16_t WaterPressure0;
	uint16_t WaterPressure1;

}CoolingADC_t;

extern CoolingADC_t CoolingADC;

#endif /* INC_GAS_VADC_H_ */
