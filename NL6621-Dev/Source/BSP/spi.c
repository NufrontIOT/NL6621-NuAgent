/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  spi.c
 *
 *    Description:  This file provides all the SPI firmware functions.
 *
 *        Version:  0.0.1
 *        Created:  2015/11/27 09:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/11/27    Create and initialize
 *
 * ====================================================================
 */
#include "spi.h"




void SPI_Init(SPI_InitTypeDef* SPI_InitStruct)
{ 
    /* check the parameters */

    assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
    assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));  


    NST_SPI->SSIENR  = 0x00;
    NST_SPI->IMR     = 0x00;
    NST_SPI->DMACR   = 0x00;
    NST_SPI->DMATDLR = 0x00;
    NST_SPI->BAUDR   = SPI_InitStruct->SPI_BaudRatePrescaler; 
    NST_SPI->CTRLR0  = (SPI_InitStruct->SPI_CPOL | SPI_InitStruct->SPI_CPHA |\
	                   SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_Mode |\
					   SPI_InitStruct->SPI_Direction);
    NST_SPI->CTRLR1  = 0x00;
    NST_SPI->SER     = SPI_InitStruct->SPI_DevNum;
}

void SPI_Cmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != IRQ_DISABLE)
	{
   		/* Enable the selected SPI peripheral */
		NST_SPI->SSIENR = 0x01;
	}
	else
	{
		/* Disable the selected SPI peripheral */
		NST_SPI->SSIENR = 0x00;
	}
}

void SPI_SendData(uint16_t Data)
{
	NST_SPI->DR = Data;   
}

FlagStatus SPI_GetFlagStatus(uint16_t SPI_FLAG)
{
	FlagStatus bitstatus = RESET;

	/* Check the status of the specified SPI flag */
	if ((NST_SPI->SR  & SPI_FLAG))
	{
		/* SPI_I2S_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* SPI_I2S_FLAG is reset */
		bitstatus = RESET;
	}
	/* Return the SPI_I2S_FLAG status */
	return  bitstatus;
}

uint16_t SPI_ReceiveData(void)
{
	/* Return the data in the DR register */
	return  NST_SPI->DR;
}

uint16_t SPI_GetTxFIFOLevel(void)
{
   	return (uint16_t)(NST_SPI->SPI_TXFLR & 0xff);
}

uint16_t SPI_GetRxFIFOLevel(void)
{
   	return (uint16_t)(NST_SPI->SPI_RXFLR & 0xff);
}

void SPI_SetTxFIFOLevel(uint16_t tx)
{
   	NST_SPI->TXFTLR = tx;
}

void SPI_SetRxFIFOLevel(uint16_t rx)
{
   	NST_SPI->RXFTLR = rx;
}

void SPI_Clear_Interrupt(uint8_t SPI_IT)
{
     int reg_vale = 0;

     if(SPI_IT != ICR_FLAG)
 	     reg_vale = (SPI_REGS *)((uint32_t)NST_SPI->TXOICR + SPI_IT*4);
     else 
	     reg_vale = NST_SPI->ICR;  
		  
     reg_vale = reg_vale;
}

void SPI_ITConfig(uint16_t SPI_IT, FunctionalState NewState)
{  
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if (NewState != DISABLE)
	{
		/* Enable the Interrupt sources */
		NST_SPI->IMR |= SPI_IT;
	}
	else
	{
		/* Disable the Interrupt sources */
		NST_SPI->IMR &= (uint16_t)~SPI_IT;
	}
}

FlagStatus SPI_GetITFlagStatus(uint16_t SPI_FLAG)
{
	FlagStatus bitstatus = RESET;

	if ((NST_SPI->ISR & SPI_FLAG) != (uint16_t)RESET)
	{
		/* SPI_I2S_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* SPI_I2S_FLAG is reset */
		bitstatus = RESET;
	}
	/* Return the SPI_I2S_FLAG status */
	return  bitstatus;
}

void SPI_DMA_Cmd(uint16_t DMA_TYPE,uint16_t Length)
{
     NST_SPI->DMACR = DMA_TYPE;

	 if(DMA_TYPE == (DMACR_TDMAE|DMACR_RDMAE)){
	     NST_SPI->DMATDLR = Length;
		 NST_SPI->DMARDLR = Length;
	 }
	 else if(DMA_TYPE == DMACR_TDMAE)  {
	     NST_SPI->DMATDLR = Length;	 
	 }
	 else if(DMA_TYPE == DMACR_RDMAE) {
	     NST_SPI->DMARDLR = Length;	 
	 }
}








/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/

