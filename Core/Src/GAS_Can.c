/*
 * CAN.c
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#include <GAS_Can.h>


CAN_FilterTypeDef sFilterConfig;
CAN_FilterTypeDef sFilterConfig2;
CAN_RxHeaderTypeDef canRxHeader;
CAN_TxHeaderTypeDef canTxHeader;
uint8_t canRx0Data[8];
uint32_t TxMailBox;
stm32_msg_t stm32;
TC_msg_t TC_msg;
stm32_msg_t TC237;
uint16_t STM32_ID = 0x405C;
uint16_t TC275_ID = 0x275A;

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
	canTxHeader.StdId = STM32_ID;
	canTxHeader.IDE	= CAN_ID_EXT;
	canTxHeader.RTR	= CAN_RTR_DATA;
	canTxHeader.DLC	=	8;


}

void GAS_Can_rxSetting(void)
{
	/*
	 * CAN rx set function
	 * IdHigh, IdLow for each 16bit in 29bit ID
	 * Set different FIFO and FilterBank
	 *
	 */
	sFilterConfig.FilterIdHigh = (TC275_ID<<3)>>16;				/*first 2byte in 29bit (shift need to IED,RTR,0)*/
	sFilterConfig.FilterIdLow = (0xffff & (TC275_ID << 3)) | (1<<2);	/*second 2byte in 29bit + IDE (shift need to IED,RTR,0/)*/
	sFilterConfig.FilterMaskIdHigh = 0;
	sFilterConfig.FilterMaskIdLow = 0;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	 if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
	  {
	    /* Filter configuration Error */
	    Error_Handler();
	  }

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

	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
	  Error_Handler();
	}


}

void GAS_Can_sendMessage()
{
	/*
	 * CAN send message function
	 * send Message data with sendData[8]
	 */
	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
	HAL_CAN_AddTxMessage(&hcan2, &canTxHeader, &stm32.TxData[0], &TxMailBox);
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	/*
	 * CAN recieve data interrupt function
	 * Check Instance then recieve data in stm32_2.RxData
	 */
	if(hcan->Instance == CAN2)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, TC_msg.RxData);

	}
}


