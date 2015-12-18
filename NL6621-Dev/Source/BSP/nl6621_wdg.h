/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_wat.h
 *
 *    Description:  This file contains all the functions prototypes for the WATCH 
 *                  firmware library.
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

#ifndef NL6621_WDG_H
#define NL6621_WDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nl6621_conf.h"




/*
* Name:         IWDG_WriteAccessCmd
* Description:  Enables or disables write access to Wdt0Torr registers.
*   
* Parameter:    None 
*
* @Param1[IN]:	None
*
* Return:       None
* 
* Note:         None
*/
void IWDG_WriteAccessCmd(void);


/*
* Name:         IWDG_ReloadCounter
* Description:  Sets IWDG Reload value.
*   
* Parameter:    Reload: specifies the IWDG Reload value.
*
* @Param1[IN]:	Reload:(Range of values  6~8)
*
* Return:       None
* 
* Note:         None
*/
void IWDG_SetReload(uint8_t Reload);

/*
* Name:         IWDG_ReloadCounter
* Description:  Reloads IWDG counter with value defined in the reload register.
*   
* Parameter:    None 
*
* @Param1[IN]:	None
*
* Return:       None
* 
* Note:         None
*/
void IWDG_ReloadCounter(void);

/*
* Name:         IWDG_Enable
* Description:  Enables IWDG (write access to IWDG_PR and IWDG_RLR registers disabled).
* 
* Parameter:    None 
*
* @Param1[IN]:	None
*
* Return:       None
* 
* Note:         None
*/
void IWDG_Enable(void);




#ifdef __cplusplus
}
#endif

#endif /* NL6621_WDG_H */

