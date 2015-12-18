/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  printf.c
 *
 *    Description:  Serial port to achieve printf interface
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
#include <stdio.h>
#include "nl6621_usart.h"


///////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
 
    /*Data should only be written to the THR when the THR Empty (THRE) bit (LSR[5]) is set.*/
    /* Loop until the end of transmission */
   	while (USART_GetFlagStatus(USART_FLAG_TC) == RESET);

    NST_WR_UART_REG((ch & (uint16_t)0x01FF), THR_OFFSET);
	    
	return ch;
}

#endif

/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/
