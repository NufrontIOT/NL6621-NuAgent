/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  qspi.c
 *
 *    Description:  This file provides all the QSPI firmware functions.
 *
 *        Version:  0.0.1
 *        Created:  2015/12/2 09:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/12/2    Create and initialize
 *
 * ====================================================================
 */
#include "qspi.h"



void QSPI_Init(QSPI_InitTypeDef* QSPI_InitStruct)
{
  
    /* check the parameters */

    assert_param(IS_QSPI_CPOL(QSPI_InitStruct->QSPI_CPOL));
    assert_param(IS_QSPI_CPHA(QSPI_InitStruct->QSPI_CPHA));  


    NST_QSPI->SSIENR  = 0x00;
    NST_QSPI->IMR     = 0x00;

    NST_QSPI->BAUDR   = QSPI_InitStruct->QSPI_BaudRatePrescaler; 
    NST_QSPI->CTRLR0  = (QSPI_InitStruct->QSPI_CPOL | QSPI_InitStruct->QSPI_CPHA |\
	                   QSPI_InitStruct->QSPI_DataSize | QSPI_InitStruct->QSPI_Mode |\
					   QSPI_InitStruct->QSPI_Direction);

    NST_QSPI->CTRLR1  = 0x00;
}

void QSPI_Cmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != IRQ_DISABLE)
	{
   		/* Enable the selected SPI peripheral */
		NST_QSPI->SSIENR = 0x01;
	}
	else
	{
		/* Disable the selected SPI peripheral */
		NST_QSPI->SSIENR = 0x00;
	}
}

FlagStatus QSPI_GetFlagStatus(uint16_t QSPI_FLAG)
{
	FlagStatus bitstatus = RESET;

	/* Check the status of the specified SPI flag */
	if ((NST_QSPI->SR  & QSPI_FLAG))
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

uint16_t QSPI_ReceiveData(void)
{
	/* Return the data in the DR register */
	return  NST_QSPI->DR;
}

void QSPI_SendData(uint16_t Data)
{
	NST_QSPI->DR = Data;   
}

uint16_t QSPI_GetTxFIFOLevel(void)
{
   	return (uint16_t)(NST_QSPI->QSPI_TXFLR & 0xff);
}

uint16_t QSPI_GetRxFIFOLevel(void)
{
   	return (uint16_t)(NST_QSPI->QSPI_RXFLR & 0xff);
}

void QSPI_SetTxFIFOLevel(uint16_t tx)
{
   	NST_QSPI->TXFTLR = tx;
}

void QSPI_SetRxFIFOLevel(uint16_t rx)
{
   	NST_QSPI->RXFTLR = rx;
}

void QSPI_Clear_Interrupt(uint8_t QSPI_IT)
{
     int reg_vale = 0;

     if(QSPI_IT != ICR_FLAG)
 	     reg_vale = (SPI_REGS *)((uint32_t)NST_QSPI->TXOICR + QSPI_IT*4);
     else 
	     reg_vale = NST_QSPI->ICR;  
		  
     reg_vale = reg_vale;
}

void QSPI_ITConfig(uint16_t QSPI_IT, FunctionalState NewState)
{  
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if (NewState != DISABLE)
	{
		/* Enable the Interrupt sources */
		NST_QSPI->IMR |= QSPI_IT;
	}
	else
	{
		/* Disable the Interrupt sources */
		NST_QSPI->IMR &= (uint16_t)~QSPI_IT;
	}
}

FlagStatus QSPI_GetITFlagStatus(uint16_t QSPI_FLAG)
{
	FlagStatus bitstatus = RESET;

	if ((NST_QSPI->ISR & QSPI_FLAG) != (uint16_t)RESET)
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

//QSPI写入一个字节
void  BSP_QSpiWriteByte(uint8_t Byte)   
{  
	while (QSPI_GetFlagStatus(QSPI_SR_TF_NOT_FULL) == RESET);
    
    NST_QSPI->DR = Byte;      
}

//qspi continous read
void BSP_QSpiRead(uint32_t RdCnt, uint8_t* pBuf)   
{
    uint8_t RxByte;
    
    while(RdCnt > 0) {
        if(NST_QSPI->SR & QSPI_SR_RF_NOT_EMPT) {
            *pBuf++ =  NST_QSPI->DR;
            RdCnt--;
        }
    }

    // throw the redundant bytes
    if(NST_QSPI->SR & QSPI_SR_RF_NOT_EMPT) {
        RxByte =  NST_QSPI->DR;
        RxByte =  RxByte; // remove warning
    }
}

//用于标志一个读写过程完成
void BSP_QSpiWait(void)
{
    UINT8 retry=0;
    
    while(!(NST_QSPI->SR & QSPI_SR_TF_EMPT))//等待发送区空  
    {
		retry++;
		if(retry>250)return ;
    }
    
    retry=0;
    while(NST_QSPI->SR & QSPI_SR_BUSY) //等待接收完一个byte  
    {
		retry++;
		if(retry>250)return ;
    }

}



#if 0
//QSPI direct read init
VOID BSP_QSpiDirectReadInit(UINT16 SpiClkDiv)
{
    // SPIpins can't used as GPIO
    NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL) & (~ENA_QSPIM_GPIO));

    NST_QSPI->SSIENR= 0x00;
    NST_QSPI->IMR= 0x00;
    //NST_QSPI->DMACR= 0x00;
    //NST_QSPI->DMATDLR= 0x00;
    NST_QSPI->BAUDR= SpiClkDiv; 
	NST_QSPI->CTRLR0=  (QSPI_TMOD_TR) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
    NST_QSPI->READ_CMD = FLASH_CMD_SEQ_READ|(QSPI_STD_FAST_RD_MODE << QSPI_RD_MODE_OFFSET);
    NST_QSPI->CTRLR1= 0x00;
    //NST_QSPI->SER= 0x01;
    NST_QSPI->SSIENR= 0x01;
}

//QSPI direct write init
VOID BSP_QSpiDirectWriteInit(UINT16 SpiClkDiv)
{
    // SPIpins can't used as GPIO
    NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL) & (~ENA_SPIM_GPIO));

    NST_QSPI->SSIENR= 0x00;
    NST_QSPI->IMR= 0x00;
    //NST_QSPI->DMACR= 0x00;
    //NST_QSPI->DMATDLR= 0x00;
    NST_QSPI->BAUDR= SpiClkDiv; 
	NST_QSPI->CTRLR0=  (QSPI_TMOD_TR) | (QSPI_FRF_SPI) | (QSPI_DataSize_8) |( QSPI_CPOL_High | QSPI_CPHA_2Edge);
    NST_QSPI->PGM_CMD = FLASH_CMD_PAGE_WRITE;
    NST_QSPI->CTRLR1= 0x00;
    //NST_QSPI->SER= 0x01;
    NST_QSPI->SSIENR= 0x01;
}

#endif

#if 0 

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the SPI global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = SPI_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = IRQ_ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	  
    SPI_ITConfig(IMR_TXEIM|IMR_RXOIM,IRQ_ENABLE);

	void SPI_IRQHandler(void)
	{
	    NVIC_DisableIRQ(SPI_IRQn);
	  
		if (SPI_GetITFlagStatus(RISR_RXOIR|RISR_TXEIR) != RESET)
	    {
			SPI_Clear_Interrupt(ICR_FLAG); 
			//ADD UERS CODE...
			//...  
		}
	
	    NVIC_EnableIRQ(SPI_IRQn);  
	} 
#endif 



/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/

