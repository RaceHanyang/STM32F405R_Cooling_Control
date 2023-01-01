/*
 * CAN.c
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#include <GAS_Can.h>
#include "string.h"

CAN_FilterTypeDef sFilterConfig;
CAN_FilterTypeDef sFilterConfig2;
CAN_RxHeaderTypeDef canRxHeader;
CAN_RxHeaderTypeDef canRxHeader2;
CAN_TxHeaderTypeDef canTxHeader; //for stm32_msg1_t
CAN_TxHeaderTypeDef canTxHeader2; //for stm32_msg2_t
uint8_t canRx0Data[8];
uint32_t TxMailBox;
stm32_msg1_t stm32_tx1;
stm32_msg2_t stm32_tx2;
TC_switch_t TC_switch;
TC_order_t TC_order;

//stm32_msg1_t TC237;
uint32_t STM32_tx1_ID = 0x405C01; //Re-factoring to 0x405C01 requires more bits. Since the only reference for this variable, canTxHeader.ExtId is uint32_t type, I believe this won't bring any issues but if things get messed up, be curious.
uint32_t STM32_tx2_ID = 0x405C02;
uint32_t TC_switch_ID = 0x275C01;
uint32_t TC_order_ID = 0x275C02;


/*-------------------------Function Prototypes--------------------------------*/
void GAS_Can_txSetting(void);
void GAS_Can_rxSetting(void);
void GAS_Can_init(void);
void GAS_Can_sendMessage();
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan);
//void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);

void GAS_Can_txSetting(void)
{
	/*
	 * CAN tx set function
	 * set ID, IDE, DLC
	 */
	canTxHeader.ExtId = STM32_tx1_ID;
	canTxHeader.IDE	= CAN_ID_EXT;
	canTxHeader.RTR	= CAN_RTR_DATA;
	canTxHeader.DLC	=	8;

	canTxHeader2.ExtId = STM32_tx2_ID;
	canTxHeader2.IDE	= CAN_ID_EXT;
	canTxHeader2.RTR	= CAN_RTR_DATA;
	canTxHeader2.DLC	=	8;


}

void GAS_Can_rxSetting(void)
{
	/*
	 * CAN rx set function
	 * IdHigh, IdLow for each 16bit in 29bit ID
	 * Set different FIFO and FilterBank
	 *
	 */
	sFilterConfig.FilterIdHigh = (TC_switch_ID<<3)>>16;				/*first 2byte in 29bit (shift need to IED,RTR,0)*/
	sFilterConfig.FilterIdLow = (0xffff & (TC_switch_ID << 3)) | (1<<2);	/*second 2byte in 29bit + IDE (shift need to IED,RTR,0/)*/
	sFilterConfig.FilterMaskIdHigh = (0x0ffffff0<<3)>>16;
	sFilterConfig.FilterMaskIdLow =(0xffff & (0x0FFFFFF0 << 3)) | (1<<2);
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 14;   /* YOU MUST USE FILTERBANK 14 if YOU USE CAN2!!!!!!!!!!!!!!! */
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

		 if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
		  {
		    /* Filter configuration Error */
		    Error_Handler();
		  }
//	sFilterConfig2.FilterIdHigh = (TC_order_ID<<3)>>16;				/*first 2byte in 29bit (shift need to IED,RTR,0)*/
//	sFilterConfig2.FilterIdLow = (0xffff & (TC_order_ID << 3)) | (1<<2);	/*second 2byte in 29bit + IDE (shift need to IED,RTR,0/)*/
//	sFilterConfig2.FilterMaskIdHigh = (0x0fffffff<<3)>>16;
//	sFilterConfig2.FilterMaskIdLow =(0xffff & (0x0FFFFFFF << 3)) | (1<<2);
//	sFilterConfig2.FilterFIFOAssignment = CAN_RX_FIFO1;
//	sFilterConfig2.FilterBank = 15;   /* YOU MUST USE FILTERBANK over 14 if YOU USE CAN2!!!!!!!!!!!!!!! */
//	sFilterConfig2.FilterMode = CAN_FILTERMODE_IDMASK;
//	sFilterConfig2.FilterScale = CAN_FILTERSCALE_32BIT;
//	sFilterConfig2.FilterActivation = ENABLE;
//	sFilterConfig2.SlaveStartFilterBank = 15;
//	 if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig2) != HAL_OK)
//	  {
//	    /* Filter configuration Error */
//	    Error_Handler();
//	  }

}

void GAS_Can_init(void)
{
	/*
	 * GAS_CAN_init
	 * ActiveNotification for Rx message pending
	 */
	GAS_Can_txSetting();
	GAS_Can_rxSetting();
	HAL_CAN_Start(&hcan2);

	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
	  Error_Handler();
	}

//	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
//	{
//	  Error_Handler();
//	}


}

void GAS_Can_sendMessage()
{
	/*
	 * CAN send message function
	 * send Message data with sendData[8]
	 */
	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
	HAL_CAN_AddTxMessage(&hcan2, &canTxHeader, &stm32_tx1.TxData[0], &TxMailBox);
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	/*
	 * CAN recieve data interrupt function
	 * Check Instance then recieve data in stm32_2.RxData
	 */
	if(hcan->Instance == CAN2)
	{
		uint8_t temp[8];
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, temp);
		if (canRxHeader.ExtId == 0x275C02){
			memcpy(TC_order.RxData,temp,sizeof(uint8_t)*8);
		}
		else{
			memcpy(TC_switch.RxData,temp,sizeof(uint8_t)*8);
		}
	}
}

//void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	/*
//	 * CAN recieve data interrupt function
//	 * Check Instance then recieve data in stm32_2.RxData
//	 */
//	if(hcan->Instance == CAN2)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &canRxHeader2, TC_order.RxData);
//
//	}
//}

