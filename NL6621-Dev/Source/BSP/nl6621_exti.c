/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_exti.c
 *
 *    Description:  This file provides all the EXTI firmware functions.
 *
 *        Version:  0.0.1
 *        Created:  2015/11/25 15:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/11/25    Create and initialize
 *
 * ====================================================================
 */
#include "nl6621_exti.h"



void EXTI_DeInit(void)
{
	NST_WR_GPIO_REG(G_INT_TYPE_LEVEL, 0); 
	NST_WR_GPIO_REG(G_INT_POLARITY, 0);
    NST_WR_GPIO_REG(G_INT_ENA, 0);  
    NST_WR_GPIO_REG(G_INT_MASK, 0);      
}

void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
    int reg_val;

	/* Check the parameters */
    assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
    assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
    assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
    assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

    //set int type level
    reg_val = NST_RD_GPIO_REG(G_INT_TYPE_LEVEL);
    if(EXTI_InitStruct->EXTI_Trigger) {
        reg_val |= EXTI_InitStruct->EXTI_Trigger;
    } else {
        reg_val &= ~(EXTI_InitStruct->EXTI_Trigger); 
    }
    NST_WR_GPIO_REG(G_INT_TYPE_LEVEL, reg_val);
	 
    //set int polarity
    reg_val = NST_RD_GPIO_REG(G_INT_POLARITY);	 
    if(EXTI_InitStruct->EXTI_Mode){
        reg_val |= EXTI_InitStruct->EXTI_Mode;
    }
    else{
	    reg_val &= ~(EXTI_InitStruct->EXTI_Mode); 
    }
    NST_WR_GPIO_REG(G_INT_POLARITY, reg_val); 
	 
    //enable interrupt
    reg_val = NST_RD_GPIO_REG(G_INT_ENA);
	if(EXTI_InitStruct->EXTI_LineCmd) {
        reg_val |= EXTI_InitStruct->EXTI_Line;	
	} else {
        reg_val &= ~(EXTI_InitStruct->EXTI_Line);	
	}
	NST_WR_GPIO_REG(G_INT_ENA, reg_val);     
}

void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
    EXTI_InitStruct->EXTI_Line = 0;
    EXTI_InitStruct->EXTI_Mode = EXTI_LEVEL_SENSITIVE;
    EXTI_InitStruct->EXTI_Trigger = EXTI_ACTIVE_HIGH;
    EXTI_InitStruct->EXTI_LineCmd = IRQ_DISABLE;
}

ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
    ITStatus bitstatus = RESET;
    uint32_t enablestatus = 0;
    /* Check the parameters */
    assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
    enablestatus = NST_RD_GPIO_REG(G_INT_STATUS) & EXTI_Line;
											   
    if (((NST_RD_GPIO_REG(G_INT_ENA) & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET)){
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    return bitstatus;
}

void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
    int  reg_val = 0;

    /* Check the parameters */
    assert_param(IS_EXTI_LINE(EXTI_Line));
  
    //clear interrupt
    reg_val = NST_RD_GPIO_REG(PORTA_EOI);
    reg_val |= EXTI_Line;
    NST_WR_GPIO_REG(PORTA_EOI, reg_val); 
}

int EXTI_GetIRQ(uint32_t EXTI_Line)
{
 	uint32_t irq_num;
	int portNum;

	irq_num = EXTI_Line; 
	for(portNum=0;portNum<32;portNum++) {
	   if(irq_num&0x01) {
		   break;
	   } 
	   irq_num = irq_num >> 1; 
	}
    if((portNum >= 0)&&(portNum < 8))
        irq_num = 8 - portNum;
    else if((portNum >=8)&&(portNum < 16))
        irq_num = 33 -portNum;
    else
        irq_num = 29;
    
	return irq_num;
}

void EXTI_GPIO_Cmd(uint32_t EXTI_Line, FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
  
    if (NewState != IRQ_DISABLE)
    {
	    NVIC_EnableIRQ(EXTI_GetIRQ(EXTI_Line));
    }
    else
    {
	    NVIC_DisableIRQ(EXTI_GetIRQ(EXTI_Line));
    }
}







