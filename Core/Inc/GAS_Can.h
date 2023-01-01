/*
 * CAN.h
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#ifndef SRC_GETANDSEND_H_
#define SRC_GETANDSEND_H_

#include "stm32f4xx_hal.h"
#include "can.h"
#include "GAS_Vadc.h"
#include <stdio.h>
#include <stdlib.h>

typedef union{
	uint8_t TxData[8];
	struct{
		uint8_t External_IS;
		uint8_t WaterPump0_IS;
		uint8_t WaterPump1_IS;
		uint8_t Radiator0_IS;
		uint8_t Radiator1_IS;
		uint8_t manualMode_isON;
		uint8_t Radiator0_T;
		uint8_t Radiator1_T;
	}__attribute__((aligned(1),packed)) B;

}stm32_msg1_t;

typedef union{
	uint8_t TxData[8];
	struct{
		uint8_t WaterPressure0;
		uint8_t WaterPressure1;
		uint8_t duty0;
		uint8_t duty1;
		uint8_t duty2;
		uint8_t Remain;
		uint8_t Remain2;
		uint8_t Remain3;
	}__attribute__((aligned(1),packed)) B;

}stm32_msg2_t;

typedef union{
	uint8_t RxData[8];
	struct{
		uint8_t manualMode_ON : 1;
		uint8_t WaterPump0_ON : 1;
		uint8_t WaterPump1_ON : 1;
		uint8_t Radiator0_ON : 1;
		uint8_t Radiator1_ON : 1;
		uint8_t ExternalFan :1;
		uint8_t OnAll : 1;
		uint8_t RST : 1;
		uint8_t Remain;
		uint16_t Remain2;
		uint32_t Remain3;
	}__attribute__((aligned(1),packed)) B;

}TC_switch_t;

typedef union{
	uint8_t RxData[8];
	struct{
		uint8_t WaterPump0_DutyOrder;
		uint8_t WaterPump1_DutyOrder;
		uint8_t Radiator0_DutyOrder;
		uint8_t Radiator1_DutyOrder;
		uint8_t ExternalFan_DutyOrder;
		uint8_t defaultDutyOrder;
		uint16_t Remain;

	}__attribute__((aligned(1),packed)) B;

}TC_order_t;

extern stm32_msg1_t stm32_tx1;
extern stm32_msg2_t stm32_tx2;
extern TC_switch_t TC_switch;
extern TC_order_t TC_order;

extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage();
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
#endif /* SRC_GETANDSEND_H_ */
