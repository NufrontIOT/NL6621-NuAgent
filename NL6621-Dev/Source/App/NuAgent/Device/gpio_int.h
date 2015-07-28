/*
 * =====================================================================================
 *
 *       Filename:  gpio_int.h
 *
 *    Description:  Head file for gpio interrupt
 *
 *        Version:  0.0.1
 *        Created:  2015/7/6 0:08:05
 *       Revision:  none
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 *--------------------------------------------------------------------------------------          
 * ChangLog:
 *  version    Author      Date         Purpose
 *  0.0.1      YOUR NAME    2015/7/6      
 *
 * =====================================================================================
 */
#ifndef __GPIO_INT_H__
#define __GPIO_INT_H__

int gpio_num2irq(int portNum);
void gpio_int_setup(int gpio, bool trigMode, bool polarity);
void gpio_int_enable(int gpio);
void gpio_int_disable(int gpio);


#endif
