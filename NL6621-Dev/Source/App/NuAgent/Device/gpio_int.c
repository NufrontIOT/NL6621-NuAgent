/*
 * =====================================================================================
 *
 *       Filename:  gpio_int.c
 *
 *    Description:  GPIO interrupt setup interface.
 *
 *        Version:  0.0.1
 *        Created:  2015/7/5 23:50:31
 *       Revision:  none
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 *--------------------------------------------------------------------------------------          
 * ChangLog:
 *  version    Author      Date         Purpose
 *  0.0.1      YOUR NAME    2015/7/5      
 *
 * =====================================================================================
 */
#include "common.h"


/*********************************************************************
 * Description:	Enable GPIO interrupt (just used for GPIO 0-31)
 * Arguments:
 *    trigMode	 Value that configures the interrupt type to be level-sensitive or edge-sensitive
 * 			    - 0: level-sensitive 
 * 			    - 1: edge-sensitive
 *    polarity	 polarity of edge, should be:
 *          when level sensitve: 
 *              - 0: active low
 * 			    - 1: active high
 *          whed edge sensitive:   
 *              - 0: falling-edge
 * 		        - 1: raiseing-edge
 *********************************************************************/
void gpio_int_setup(int gpio, bool trigMode, bool polarity)
{
    BSP_GPIOPinMux(gpio);
	BSP_GPIOSetDir(gpio, 0);
                                                        
	BSP_GPIOIntMask(gpio, 0);  /* unmask gpio interrupt. */
    BSP_GPIOIntEn(gpio, trigMode, polarity); 
}

int gpio_num2irq(int portNum)
{
 	int irq_num;

    if (portNum > 31)
        return 0;
	 
    if ((portNum >= 0) && (portNum < 8))
        irq_num = 8 - portNum;
    else if ((portNum >= 8) && (portNum < 16))
        irq_num = 33 - portNum;
    else
        irq_num = 29;
    
	return irq_num;
}


/* Enable GPIO interrupt */
void gpio_int_enable(int gpio)
{
    NVIC_EnableIRQ(gpio_num2irq(gpio));
}

/* Disable GPIO interrupt */
void gpio_int_disable(int gpio)
{
    int reg_val;

    NVIC_DisableIRQ(gpio_num2irq(gpio));

    /* clear interrupt */
    reg_val = NST_RD_GPIO_REG(PORTA_EOI);
    reg_val |= 1 << gpio;
    NST_WR_GPIO_REG(PORTA_EOI, reg_val);
}

