/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_usart.c
 *
 *    Description:  This file provides all the USART firmware functions.
 *
 *        Version:  0.0.1
 *        Created:  2015/11/26 11:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/11/26    Create and initialize
 *
 * ====================================================================
 */
#include "nl6621_usart.h"
#include <stdio.h>
#include "ucos_ii.h"
#include "os_dmem.h"


void USART_Init(USART_InitTypeDef* USART_InitStruct)
{
    uint16_t Divisor = APB_CLK_FREQ_40M/(USART_InitStruct->USART_BaudRate*16);
    uint32_t Tmp = 0;
	uint32_t reg_vale = 0;

    assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
    assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
    assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
    assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
    assert_param(IS_USART_MODE(USART_InitStruct->USART_FIFOMode));
	//停止位1.5bit，数据位必须在5bit,  停止位2bit，数据位为6~8bit

    Tmp = APB_CLK_FREQ_40M%(USART_InitStruct->USART_BaudRate*16);
    if (Tmp > ((USART_InitStruct->USART_BaudRate*16)>>1))
        Divisor++;

	NST_WR_UART_REG(0x80, LCR_OFFSET);    //0x87，停止位1.5个，位数8，解锁
 	//波特率配置
    NST_WR_UART_REG((Divisor >> 8), DLH_OFFSET);  
    NST_WR_UART_REG((Divisor & 0xFF), DLL_OFFSET);  
 	//奇偶校验
    if(USART_InitStruct->USART_Parity == USART_Parity_No) {
	   reg_vale = 0;
	} else {
	   reg_vale |= 	USART_InitStruct->USART_Parity;
	}
	reg_vale |= USART_InitStruct->USART_StopBits;
  	reg_vale |= USART_InitStruct->USART_WordLength;

    NST_WR_UART_REG(reg_vale, LCR_OFFSET);    //上锁

   	//FIFO配置
	reg_vale = 0;
	if(USART_InitStruct->USART_FIFOMode != FCR_DMA_MODE && USART_InitStruct->USART_FIFOMode)
        reg_vale = USART_InitStruct->USART_FIFOMode | FCR_FIFO_ON;
    
    NST_WR_UART_REG(reg_vale, FCR_OFFSET);  //0x01

}

void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
    /* USART_InitStruct members default value */
    USART_InitStruct->USART_BaudRate = 115200;
    USART_InitStruct->USART_WordLength = USART_WordLength_8b;
    USART_InitStruct->USART_StopBits = USART_StopBits_1;
    USART_InitStruct->USART_Parity = USART_Parity_No ;
    USART_InitStruct->USART_FIFOMode = FCR_FIFO_OFF; 
}


void USART_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
  
    if (NewState != IRQ_DISABLE)
    {
       /* Enable the selected USART by setting the UE bit in the CR1 register */
	    NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL) & (~ENA_UART_GPIO));
    }
    else
    {
       /* Disable the selected USART by clearing the UE bit in the CR1 register */
	    NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL) & (ENA_UART_GPIO));
    }
}

void USART_SendData(uint16_t Data)
{
	/* Check the parameters */
    assert_param(IS_USART_DATA(Data)); 
    
    /*Data should only be written to the THR when the THR Empty (THRE) bit (LSR[5]) is set.*/
    /* Loop until the end of transmission */
   	while (USART_GetFlagStatus(USART_FLAG_TC) == RESET);

    NST_WR_UART_REG((Data & (uint16_t)0x01FF), THR_OFFSET);
}

uint16_t USART_ReceiveData(void)
{
     while (USART_GetFlagStatus(USART_FLAG_RXNE) == RESET);
    
     return NST_RD_UART_REG(RBR_OFFSET);  
}

void USART_ITConfig(uint16_t USART_IT, FunctionalState NewState)
{
	 if(NewState == IRQ_ENABLE) {
	     NST_WR_UART_REG(USART_IT|NST_RD_UART_REG(IER_OFFSET), IER_OFFSET); 
	 } else {
	     NST_WR_UART_REG((NST_RD_UART_REG(IER_OFFSET)&(~USART_IT)), IER_OFFSET); 
	 }
}

FlagStatus USART_GetFlagStatus(uint16_t USART_FLAG)
{
    FlagStatus bitstatus = RESET;
    assert_param(IS_USART_FLAG(USART_FLAG));
    /* The CTS flag is not available for UART4 and UART5 */
  
    if ((NST_RD_UART_REG(LSR_OFFSET)&USART_FLAG) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

uint32_t USART_ClearFlag(void)
{
   return NST_RD_UART_REG(LSR_OFFSET);
}

ITStatus USART_GetITStatus(uint16_t USART_IT)
{
    FlagStatus bitstatus = RESET;
    /* The CTS flag is not available for UART4 and UART5 */
  
    if ((NST_RD_UART_REG(IIR_OFFSET)&USART_IT) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;   
}

