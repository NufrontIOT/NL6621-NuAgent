/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  wdg.c
 *
 *    Description:  This file provides all the WATCH firmware functions.
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
#include "wdg.h"



void IWDG_WriteAccessCmd(void)
{
    *Wdt0Cr = 0;
}

void IWDG_SetReload(uint8_t Reload)
{
	/*
	* ���Ź����ʱ�乫ʽ��Tout = 2^(Reload+16)/40000 ms
	* ���100ms:  Reload=6
	* ���200ms:  Reload=7  
    * ���400ms:  Reload=8
	*/
    uint8_t reg_val = 0x90;

	if(Reload > 5 && Reload < 9)
    {
	   reg_val |= Reload;
       *Wdt0Torr = reg_val;	
	}
}

void IWDG_ReloadCounter(void)
{
    *Wdt0Crr = 0x76;  //ι��
}


void IWDG_Enable(void)
{
    *Wdt0Cr = 1;  //ʹ�ܿ��Ź�,���Ź�һ��ʹ�ܣ��͹رղ��ˣ�����������Թرգ���ʧȥ���Ź��Ĺ����ˣ�
}


/*
******************************************************************************
**                            End Of File                                    **
******************************************************************************
*/

