/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_gpio.c
 *
 *    Description:  This file provides all the GPIO firmware functions.
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
#include "nl6621_gpio.h"


void GPIO_Init(GPIO_InitTypeDef* GPIO_InitStruct)
{
    int pin_reg = 0,mode_reg;

    /* Check the parameters */
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin)); 

	/* set ADDR_GPIO_PIN_MUX_CTRL Register*/
    pin_reg = NST_RD_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL);
    pin_reg |= GPIO_InitStruct->GPIO_Pin; 

	/* set SWPORTA_DDR Register*/
    mode_reg = NST_RD_GPIO_REG(SWPORTA_DDR);
    if(GPIO_InitStruct->GPIO_Mode){
        mode_reg |= GPIO_InitStruct->GPIO_Pin;
    } else {
        mode_reg &= ~GPIO_InitStruct->GPIO_Pin; 
    }

    NST_WR_PWM_REG(ADDR_GPIO_PIN_MUX_CTRL, pin_reg); 
    NST_WR_GPIO_REG(SWPORTA_DDR, mode_reg);   
}

void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_In;
}

void  BSP_GPIOSetDir(uint32_t GPIO_Pin, unsigned char dir)
{
    int reg_val;
	
    if(dir)
    {
        reg_val = NST_RD_GPIO_REG(SWPORTA_DDR);
        reg_val |= GPIO_Pin;
    }
    else
    {
        reg_val = NST_RD_GPIO_REG(SWPORTA_DDR);
        reg_val &= ~GPIO_Pin; 
    }
    NST_WR_GPIO_REG(SWPORTA_DDR, reg_val); 
}

void  BSP_GPIOSetValue(uint32_t GPIO_Pin, unsigned char bitValue)
{
     int reg_val;
	
    if(bitValue)
    {
        reg_val = NST_RD_GPIO_REG(SWPORTA_DR);
        reg_val |= GPIO_Pin;
    }
    else
    {
        reg_val = NST_RD_GPIO_REG(SWPORTA_DR);
        reg_val &= ~GPIO_Pin; 
    }
    NST_WR_GPIO_REG(SWPORTA_DR, reg_val); 
}


void GPIO_SetBits(uint32_t GPIO_Pin)
{
	int reg_val;

    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
  
    reg_val = NST_RD_GPIO_REG(SWPORTA_DR);
    reg_val |= GPIO_Pin;

    NST_WR_GPIO_REG(SWPORTA_DR, reg_val); 
}

void GPIO_ResetBits(uint32_t GPIO_Pin)
{
	int reg_val;

    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
  
    reg_val = NST_RD_GPIO_REG(SWPORTA_DR);
    reg_val &= ~GPIO_Pin; 

    NST_WR_GPIO_REG(SWPORTA_DR, reg_val); 
}

uint8_t GPIO_ReadInputDataBit(uint32_t GPIO_Pin)
{
    int reg_val;

    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin)); 
	    	
    reg_val = NST_RD_GPIO_REG(EXT_PORTA); 
    reg_val &= GPIO_Pin;

    return (reg_val?1:0);
}

uint32_t GPIO_ReadInputData(void)
{
    int reg_val;

    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
	     	
    reg_val = NST_RD_GPIO_REG(EXT_PORTA); 
    reg_val &= GPIO_Pin_All;

    return reg_val;
}

void GPIO_EXTILineConfig(uint32_t GPIO_Pin, FunctionalState Cmd)
{
    int reg_val;
    
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
			 
     //Mask or unmask interrupt
    reg_val = NST_RD_GPIO_REG(G_INT_MASK);
    if(Cmd){
        reg_val |= GPIO_Pin;
    } else {
        reg_val &= ~(GPIO_Pin); 
    }
    NST_WR_GPIO_REG(G_INT_MASK, reg_val);  
}
