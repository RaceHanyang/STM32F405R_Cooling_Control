/*
 * GAS_PWM.h
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 */

#ifndef INC_GAS_CCTL_H_
#define INC_GAS_CCTL_H_

#include "tim.h"
#include "stm32f4xx_hal_tim.h"
#include "GAS_Vadc.h"
#include "GAS_CAN.h"

extern void GAS_CCTL_outputInit(void);
extern void GAS_CCTL_change_ExternalFAN(uint8_t Duty);
extern void GAS_CCTL_change_RadiatorFAN(uint8_t Duty);
extern void GAS_CCTL_change_WaterPump(uint8_t Duty);

#endif /* INC_GAS_CCTL_H_ */
