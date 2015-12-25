/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  timer.h
 *
 *    Description:  This file contains all the functions prototypes for the TIMER 
 *                  firmware library.
 *
 *        Version:  0.0.1
 *        Created:  2015/11/27 14:47:43
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

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_conf.h"


/* SysTick Registers */
#define SysTickCtrl  (volatile int*)0xE000E010
#define SysTickLoad  (volatile int*)0xE000E014
#define SysTickValue (volatile int*)0xE000E018
#define SysTickCalib (volatile int*)0xE000E01c

/* SysTick Masks */
#define SysTickCountFlag (1<<16)
#define SysTickClkSource (1<<2)  // ÄÚºËÊ±ÖÓÔ´
#define SysTickInterrupt (1<<1)
#define SysTickEnable    (1<<0)

#define TMR_INT_MASK         (1<<2)
/*0 - free-running mode
1 - user-defined count mode*/
#define TMR_USER_DEFINE_MODE (1<<1) 
#define TMR_ENA              (1<<0)



/** 
  * @brief  TIM Time Base Init structure definition
  * @note   This structure is used with all TIMx except for TIM6 and TIM7.    
  */

typedef struct
{	
  uint32_t TIM_Period;            /*!< Specifies the period value to be loaded into the active
                                       Auto-Reload Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.  */ 

  uint16_t TIM_CounterMode;       /*!< Specifies the counter mode.
                                       This parameter can be a value of @ref TIM_Counter_Mode */

} TIM_TimeBaseInitTypeDef;   

typedef enum
{
  TIMER0,
  TIMER1
}TIM_TypeDef;

#define IS_TIM_ALL_PERIPH(PERIPH) (((PERIPH) == TIMER0) || \
                                   ((PERIPH) == TIMER1) )

#define TIM_CounterMode_UserDown           ((uint16_t)0x02)
#define TIM_CounterMode_FreeRun            ((uint16_t)0x00)

#define IS_TIM_COUNTER_MODE(MODE) (((MODE) == TIM_CounterMode_UserDown) ||  \
                                   ((MODE) == TIM_CounterMode_FreeRun) )


/*
* Name:         TIM_Cmd
* Description:  Enables or disables the specified TIM peripheral.
*               
* Parameter:    TIMx: where x can be 0 to 1 to select the TIMx peripheral.
*               NewState: new state of the TIMx peripheral.
*               This parameter can be: IRQ_ENABLE or IRQ_DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void TIM_Cmd(TIM_TypeDef TIMx, FunctionalState NewState);

/*
* Name:         TIM_ClearITPendingBit
* Description:  Clears the TIMx's interrupt pending bits.
*               
* Parameter:    TIMx: where x can be 0 to 1 to select the TIMx peripheral.
*
* Return:       None
* 
* Note:         None
*/
void TIM_ClearITPendingBit(TIM_TypeDef TIMx);

/*
* Name:         TIM_ITConfig
* Description:  Enables or disables the specified TIM interrupts.
*               
* Parameter:    TIMx: where x can be 0 to 1 to select the TIMx peripheral.
*               NewState: new state of the TIMx peripheral.
*               This parameter can be: IRQ_ENABLE or IRQ_DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void TIM_ITConfig(TIM_TypeDef TIMx, FunctionalState NewState);

/*
* Name:         TIM_GetITStatus
* Description:  Enables or disables the specified TIM interrupts.
*               
* Parameter:    TIMx: where x can be 0 to 1 to select the TIMx peripheral.
*
* Return:       None
* 
* Note:         None
*/
ITStatus TIM_GetITStatus(TIM_TypeDef TIMx);

/*
* Name:         TIM_TimeBaseInit
* Description:  Initializes the TIMx Time Base Unit peripheral according to 
*               the specified parameters in the TIM_TimeBaseInitStruct.
* Parameter:    TIMx: where x can be 0 to 1 to select the TIMx peripheral.
*               TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef
*               structure that contains the configuration information for the specified TIM peripheral.
*
* Return:       None
* 
* Note:         None
*/
void TIM_TimeBaseInit(TIM_TypeDef TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);



#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */

