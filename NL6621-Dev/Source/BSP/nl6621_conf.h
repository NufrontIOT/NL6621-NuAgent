/*
 * =====================================================================================
 *
 *       Filename:  nl6621_conf.h
 *
 *    Description:  Library configuration file.
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
 *        version    Author      Date         Purpose
 *        0.0.1      Hui Lou     2015.11.25   Create and initialize    
 *
 * =====================================================================================
 */
							
#ifndef NL6621_CONF_H
#define NL6621_CONF_H



/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
#include <stdint.h>
#include "os_cfg.h"

#include "nl6621_nvic.h"
#include "nl6621_gpio.h"
#include "nl6621_exti.h"
#include "nl6621_usart.h"
#include "nl6621_wdg.h"
#include "nl6621_timer.h"
#include "nl6621_i2c.h"
#include "nl6621_spi.h"
#include "nl6621_qspi.h"
#include "nl6621_dma.h"
#include "nl6621_i2s.h"
#include "nl6621_sdio.h"





/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */



#include "types_def.h"
#include "app_cfg.h"
#include "board.h"
#include "reg.h"


#endif /* NL6621_GPIO_H */



