/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_dma.h
 *
 *    Description:  This file contains all the functions prototypes for the DMA 
 *                  firmware library.
 *
 *        Version:  0.0.1
 *        Created:  2015/12/1 15:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/12/1    Create and initialize
 *
 * ====================================================================
 */

#ifndef NL6621_DMA_H
#define NL6621_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nl6621_conf.h"


#ifdef DMA_MOVE_MEM
	extern OS_EVENT *pDmaSem;
	extern PUINT8 pDmaSrc;
	extern PUINT8 pDmaDst;
	extern UINT16 DmaSize;
#endif // DMA_MOVE_MEM //


/*
* Name:         BSP_DmaInit
* Description:  DMA initialization,Memory to peripheral.
*
* Parameter:    channel_num: Select channel(0~3).
*
* Return:       None
* 
* Note:         None
*/
void BSP_DmaInit (uint8_t channel_num);

#ifdef DMA_MOVE_MEM
	/*
	* Name:         BSP_DmaMoveMemInit
	* Description:  DMA initialization,Memory to Memory.
	*
	* Parameter:    channel_num: Select channel(0~3).
	*
	* Return:       None
	* 
	* Note:         None
	*/
	void BSP_DmaMoveMemInit (uint8_t channel_num);
#endif // DMA_MOVE_MEM //

/*
* Name:         BSP_DmaStart
* Description:  Start DMA transmission.
*
* Parameter:    pSrc: Set DMA Source addr. 
*               pDest:Set DMA Destination addr.
*               channel_num: Select channel(0~3).
*
* Return:       None
* 
* Note:         None
*/
void BSP_DmaStart(uint32_t* pSrc, uint32_t* pDest, uint8_t channel_num);

/*
* Name:         BSP_DmaMoveMemInit
* Description:  DMA initialization,Memory to Memory.
*
* Parameter:    channel_num: Select channel(0~3).
*
* Return:       None
* 
* Note:         None
*/
void BSP_DmaHalt(uint8_t channel_num);

/*
* Name:         BSP_DmaDeinit
* Description:  ALL Channel DMA initialization.
*
* Parameter:    None
*
* Return:       None
* 
* Note:         None
*/
void BSP_DmaDeinit(void);

/*
* Name:         BSP_DmaIntISR
* Description:  DMA interrupt handler.
*
* Parameter:    None
*
* Return:       None
* 
* Note:         None
*/
void BSP_DmaIntISR(void);


#ifdef __cplusplus
}
#endif

#endif /* NL6621_TIMER_H */

