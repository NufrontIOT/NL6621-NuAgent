/*
 * =====================================================================================
 *
 *       Filename:  nl6621_exti.h
 *
 *    Description:  This file contains all the functions prototypes for the EXTI 
 *                  firmware library.
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
							
#ifndef NL6621_EXTI_H
#define NL6621_EXTI_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nl6621_conf.h"


/** 
  * @brief  EXTI mode enumeration  
  */

typedef enum
{
  EXTI_LEVEL_SENSITIVE = 0x00,
  EXTI_EDGE_SENSITIVE = 0x01
}EXTIMode_TypeDef;

#define IS_EXTI_MODE(MODE) (((MODE) == EXTI_LEVEL_SENSITIVE) || ((MODE) == EXTI_EDGE_SENSITIVE))

/** 
  * @brief  EXTI Trigger enumeration  
  */

typedef enum
{
  EXTI_ACTIVE_LOW = 0x00,
  EXTI_ACTIVE_HIGH = 0x01,  
}EXTITrigger_TypeDef;

#define IS_EXTI_TRIGGER(TRIGGER) (((TRIGGER) == EXTI_ACTIVE_LOW) || \
                                  ((TRIGGER) == EXTI_Trigger_Falling) )
/** 
  * @brief  EXTI Init Structure definition  
  */

typedef struct
{
  uint32_t EXTI_Line;               /*!< Specifies the EXTI lines to be enabled or disabled.
                                         This parameter can be any combination of @ref EXTI_Lines */
   
  EXTIMode_TypeDef EXTI_Mode;       /*!< Specifies the mode for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

  EXTITrigger_TypeDef EXTI_Trigger; /*!< Specifies the trigger signal active edge for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

  FunctionalState EXTI_LineCmd;     /*!< Specifies the new state of the selected EXTI lines.
                                         This parameter can be set either to ENABLE or DISABLE */ 
}EXTI_InitTypeDef;

/**
  * @}
  */

/** @defgroup EXTI_Exported_Constants
  * @{
  */

/** @defgroup EXTI_Lines 
  * @{
  */

//#define EXTI_Line0       ((uint32_t)0x00000001)  /*!< External interrupt line 0 */
//#define EXTI_Line1       ((uint32_t)0x00000002)  /*!< External interrupt line 1 */
//#define EXTI_Line2       ((uint32_t)0x00000004)  /*!< External interrupt line 2 */
#define EXTI_Line3       ((uint32_t)0x00000008)  /*!< External interrupt line 3 */
#define EXTI_Line4       ((uint32_t)0x00000010)  /*!< External interrupt line 4 */
#define EXTI_Line5       ((uint32_t)0x00000020)  /*!< External interrupt line 5 */
#define EXTI_Line6       ((uint32_t)0x00000040)  /*!< External interrupt line 6 */
#define EXTI_Line7       ((uint32_t)0x00000080)  /*!< External interrupt line 7 */
#define EXTI_Line8       ((uint32_t)0x00000100)  /*!< External interrupt line 8 */
#define EXTI_Line9       ((uint32_t)0x00000200)  /*!< External interrupt line 9 */
#define EXTI_Line10      ((uint32_t)0x00000400)  /*!< External interrupt line 10 */
#define EXTI_Line11      ((uint32_t)0x00000800)  /*!< External interrupt line 11 */
#define EXTI_Line12      ((uint32_t)0x00001000)  /*!< External interrupt line 12 */
//#define EXTI_Line13      ((uint32_t)0x00002000)  /*!< External interrupt line 13 */
//#define EXTI_Line14      ((uint32_t)0x00004000)  /*!< External interrupt line 14 */
//#define EXTI_Line15      ((uint32_t)0x00008000)  /*!< External interrupt line 15 */
#define EXTI_Line16      ((uint32_t)0x00010000)  /*!< External interrupt line 16 */
#define EXTI_Line17      ((uint32_t)0x00020000)  /*!< External interrupt line 17 */
#define EXTI_Line18      ((uint32_t)0x00040000)  /*!< External interrupt line 18 */
#define EXTI_Line19      ((uint32_t)0x00080000)  /*!< External interrupt line 19 */
#define EXTI_Line20      ((uint32_t)0x00100000)  /*!< External interrupt line 20 */
#define EXTI_Line21      ((uint32_t)0x00200000)  /*!< External interrupt line 21 */
#define EXTI_Line22      ((uint32_t)0x00400000)  /*!< External interrupt line 22 */
#define EXTI_Line23      ((uint32_t)0x00800000)  /*!< External interrupt line 23 */
#define EXTI_Line24      ((uint32_t)0x01000000)  /*!< External interrupt line 24 */
#define EXTI_Line25      ((uint32_t)0x02000000)  /*!< External interrupt line 25 */
#define EXTI_Line26      ((uint32_t)0x04000000)  /*!< External interrupt line 26 */
#define EXTI_Line27      ((uint32_t)0x08000000)  /*!< External interrupt line 27 */
#define EXTI_Line28      ((uint32_t)0x10000000)  /*!< External interrupt line 28 */
#define EXTI_Line29      ((uint32_t)0x20000000)  /*!< External interrupt line 29 */
#define EXTI_Line30      ((uint32_t)0x40000000)  /*!< External interrupt line 30 */
#define EXTI_Line31      ((uint32_t)0x80000000)  /*!< External interrupt line 31 */
                                          
#define IS_EXTI_LINE(LINE) ((((LINE) & (uint32_t)0x00) == 0x00) && ((LINE) != (uint32_t)0x00))
#define IS_GET_EXTI_LINE(LINE) (((LINE) == EXTI_Line3) || \
                            ((LINE) == EXTI_Line4) || ((LINE) == EXTI_Line5) || \
                            ((LINE) == EXTI_Line6) || ((LINE) == EXTI_Line7) || \
                            ((LINE) == EXTI_Line8) || ((LINE) == EXTI_Line9) || \
                            ((LINE) == EXTI_Line10) || ((LINE) == EXTI_Line11) || \
                            ((LINE) == EXTI_Line12) || ((LINE) == EXTI_Line17) || \
                            ((LINE) == EXTI_Line18) || ((LINE) == EXTI_Line19) || \
                            ((LINE) == EXTI_Line20) || ((LINE) == EXTI_Line21) || \
                            ((LINE) == EXTI_Line22) || ((LINE) == EXTI_Line23) || \
                            ((LINE) == EXTI_Line24) || ((LINE) == EXTI_Line25) || \
                            ((LINE) == EXTI_Line26) || ((LINE) == EXTI_Line27) || \
                            ((LINE) == EXTI_Line28) || ((LINE) == EXTI_Line29) || \
							((LINE) == EXTI_Line30) || ((LINE) == EXTI_Line31) || \
							)

                    

/*
* Name:         EXTI_DeInit
* Description:  Deinitializes the EXTI peripheral registers to their default reset values.
* 
* Parameter:    None 
*
* Return:       None
* 
* Note:         None
*/
void EXTI_DeInit(void);

/*
* Name:         EXTI_DeInit
* Description:  Initializes the EXTI peripheral according to the specified
*               parameters in the EXTI_InitStruct.
* Parameter:    EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure
*               that contains the configuration information for the EXTI peripheral.
*
* Return:       None
* 
* Note:         None
*/
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);

/*
* Name:         EXTI_StructInit
* Description:  Fills each EXTI_InitStruct member with its reset value.
*               
* Parameter:    EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure which will
*               be initialized.
*
* Return:       None
* 
* Note:         None
*/
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);


/*
* Name:         EXTI_GetITStatus
* Description:  Checks whether the specified EXTI line is asserted or not.
*               
* Parameter:    EXTI_Line: specifies the EXTI line to check.
*               This parameter can be any combination of EXTI_Linex where x can be (3..12 and 17...31). 
*
* Return:       None
* 
* Note:         None
*/
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);

/*
* Name:         EXTI_ClearITPendingBit
* Description:  Clears the EXTI's line pending bits.
*               
* Parameter:    EXTI_Line: specifies the EXTI line to check.
*               This parameter can be any combination of EXTI_Linex where x can be (3..12 and 17...31). 
*
* Return:       None
* 
* Note:         None
*/
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);



/*
* Name:         EXTI_GetIRQ
* Description:  GET GPIO IRQ ID
*               
* Parameter:    EXTI_Line: specifies the EXTI line to check.
*               This parameter can be any combination of EXTI_Linex where x can be (3..12 and 17...31). 
*
* Return:       None
* 
* Note:         None
*/
int EXTI_GetIRQ(uint32_t EXTI_Line);

/*
* Name:         EXTI_GPIO_Cmd
* Description:  Enables or disables the specified GPIO IRQ.
*               
* Parameter:    EXTI_Line: specifies the EXTI line to check.
*               This parameter can be any combination of EXTI_Linex where x can be (3..12 and 17...31). 
*               NewState: new state of the USARTx peripheral.
*               This parameter can be: ENABLE or DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void EXTI_GPIO_Cmd(uint32_t EXTI_Line, FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif /* NL6621_GPIO_H */



