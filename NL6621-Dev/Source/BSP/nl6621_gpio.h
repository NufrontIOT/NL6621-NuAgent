/*
 * =====================================================================================
 *
 *       Filename:  nl6621_gpio.h
 *
 *    Description:  This file contains all the functions prototypes for the GPIO 
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
							
#ifndef NL6621_GPIO_H
#define NL6621_GPIO_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nl6621_conf.h"


#define NST_WR_GPIO_REG(Offset, Value)  /*0x40040000*/\
    {\
        (*(volatile UINT32 *)(_GPIO_BASE_ADR + Offset)) = (uint32_t)Value;\
    }

#define NST_RD_GPIO_REG(Offset)   (*(volatile UINT32 *)(_GPIO_BASE_ADR + Offset))


/** 
  * @brief  Configuration Mode enumeration  
  */

typedef enum
{ 
  GPIO_Mode_In  = 0x00,

  GPIO_Mode_Out = 0x01,

}GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_In) || ((MODE) == GPIO_Mode_Out) )

/** 
  * @brief  GPIO Init structure definition  
  */

typedef struct
{
  uint32_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_pins_define */

  GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;


/** 
  * @brief  Bit_SET and Bit_RESET enumeration  
  */

typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

/**
  * @}
  */

/** @defgroup GPIO_Exported_Constants
  * @{
  */

/** @defgroup GPIO_pins_define 
  * @{
  */

//#define GPIO_Pin_0                 ((uint32_t)0x00000001)  /*!< Pin 0 selected */
//#define GPIO_Pin_1                 ((uint16_t)0x00000002)  /*!< Pin 1 selected */
//#define GPIO_Pin_2                 ((uint32_t)0x00000004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint32_t)0x00000008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint32_t)0x00000010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint32_t)0x00000020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint32_t)0x00000040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint32_t)0x00000080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint32_t)0x00000100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x00000200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint32_t)0x00000400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint32_t)0x00000800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint32_t)0x00001000)  /*!< Pin 12 selected */
//#define GPIO_Pin_13                ((uint32_t)0x00002000)  /*!< Pin 13 selected */
//#define GPIO_Pin_14                ((uint32_t)0x00004000)  /*!< Pin 14 selected */
//#define GPIO_Pin_15                ((uint32_t)0x00008000)  /*!< Pin 15 selected */
//#define GPIO_Pin_16                ((uint32_t)0x00010000)  /*!< Pin 16 selected */
#define GPIO_Pin_17                ((uint32_t)0x00020000)  /*!< Pin 17 selected */
#define GPIO_Pin_18                ((uint32_t)0x00040000)  /*!< Pin 18 selected */
#define GPIO_Pin_19                ((uint32_t)0x00080000)  /*!< Pin 19 selected */
#define GPIO_Pin_20                ((uint32_t)0x00100000)  /*!< Pin 20 selected */
#define GPIO_Pin_21                ((uint32_t)0x00200000)  /*!< Pin 21 selected */
#define GPIO_Pin_22                ((uint32_t)0x00400000)  /*!< Pin 22 selected */
#define GPIO_Pin_23                ((uint32_t)0x00800000)  /*!< Pin 23 selected */
#define GPIO_Pin_24                ((uint32_t)0x0100000)   /*!< Pin 24 selected */
#define GPIO_Pin_25                ((uint32_t)0x02000000)  /*!< Pin 25 selected */
#define GPIO_Pin_26                ((uint32_t)0x04000000)  /*!< Pin 26 selected */
#define GPIO_Pin_27                ((uint32_t)0x08000000)  /*!< Pin 27 selected */
#define GPIO_Pin_28                ((uint32_t)0x10000000)  /*!< Pin 28 selected */
#define GPIO_Pin_29                ((uint32_t)0x20000000)  /*!< Pin 29 selected */
#define GPIO_Pin_30                ((uint32_t)0x40000000)  /*!< Pin 30 selected */
#define GPIO_Pin_31                ((uint32_t)0x80000000)  /*!< Pin 31 selected */

#define GPIO_Pin_All               ((uint32_t)0xFFF71FF8)  /*!< All pins selected */

#define IS_GPIO_PIN(PIN) ((((PIN) & (uint32_t)0x00) == 0x00) && ((PIN) != (uint32_t)0x00))

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_17) || \
                              ((PIN) == GPIO_Pin_18) || \
                              ((PIN) == GPIO_Pin_19) || \
							  ((PIN) == GPIO_Pin_20) || \
                              ((PIN) == GPIO_Pin_21) || \
                              ((PIN) == GPIO_Pin_22) || \
                              ((PIN) == GPIO_Pin_23) || \
                              ((PIN) == GPIO_Pin_24) || \
                              ((PIN) == GPIO_Pin_25) || \
                              ((PIN) == GPIO_Pin_26) || \
                              ((PIN) == GPIO_Pin_27) || \
                              ((PIN) == GPIO_Pin_28) || \
                              ((PIN) == GPIO_Pin_29) || \
                              ((PIN) == GPIO_Pin_30) || \
                              ((PIN) == GPIO_Pin_31))
                              


/*
* Name:         GPIO_Init
* Description:  Initializes the GPIOx peripheral according to the specified
*               parameters in the GPIO_InitStruct.
* 
* Parameter:    GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that
*                                contains the configuration information for the specified GPIO peripheral.
*
* Return:       None
* 
* Note:         None
*/
void GPIO_Init(GPIO_InitTypeDef* GPIO_InitStruct);
/*
* Name:         GPIO_StructInit
* Description:  Fills each GPIO_InitStruct member with its default value.
* 
* Parameter:    GPIO_InitStruct : pointer to a GPIO_InitTypeDef structure which will
*                                 be initialized.
*
* Return:       None
* 
* Note:         None
*/
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
/*
* Name:         GPIO_ReadInputDataBit
* Description:  Reads the specified input port pin.
* 
* Parameter:    GPIO_Pin : specifies the port bit to read.
*                          This parameter can be any combination of GPIO_Pin_x where x can be (3..12 and 17...31). 
*
* Return:       None
* 
* Note:         None
*/
uint8_t GPIO_ReadInputDataBit(uint32_t GPIO_Pin);
/*
* Name:         GPIO_StructInit
* Description:  Reads the specified GPIO input data port.
* 
* Parameter:    None 
*
* Return:       None
* 
* Note:         None
*/
uint32_t GPIO_ReadInputData(void);

/*
* Name:         GPIO_SetBits
* Description:  Sets the selected data port bits.
* 
* Parameter:    GPIO_Pin : specifies the port bits to be written.
*                          This parameter can be any combination of GPIO_Pin_x where x can be (3..12 and 17...31). 
*
* Return:       None
* 
* Note:         None
*/
void GPIO_SetBits(uint32_t GPIO_Pin);
/*
* Name:         GPIO_SetBits
* Description:  Clears the selected data port bits.
* 
* Parameter:    GPIO_Pin : specifies the port bits to be written.
*                          This parameter can be any combination of GPIO_Pin_x where x can be (3..12 and 17...31). 
*
* Return:       None
* 
* Note:         None
*/
void GPIO_ResetBits(uint32_t GPIO_Pin);
/*
* Name:         GPIO_SetBits
* Description:  Selects the GPIO pin used as EXTI Line.
* 
* Parameter:    GPIO_Pin : specifies the port bits to be written.
*                                This parameter can be any combination of GPIO_Pin_x where x can be (3..12 and 17...31). 
*               Cmd            : This parameter can be: IRQ_ENABLE or IRQ_DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void GPIO_EXTILineConfig(uint32_t GPIO_Pin, FunctionalState Cmd);

/*
* Name:         BSP_GPIOSetDir
* Description:  Set Direction for GPIO port.
* 
* Parameter:    GPIO_Pin : specifies the port bits to be written.
*                          This parameter can be any combination of GPIO_Pin_x where x can be (3..12 and 17...31). 
*             	dir: 	   Direction value, should be:
* 							- 0: Input.
* 							- 1: Output.   
*
* Return:       None
* 
* Note:         None
*/
void  BSP_GPIOSetDir(uint32_t GPIO_Pin, unsigned char dir);


/*
* Name:         BSP_GPIOSetValue
* Description:  Set Direction for GPIO port.
* 
* Parameter:    GPIO_Pin : specifies the port bits to be written.
*                          This parameter can be any combination of GPIO_Pin_x where x can be (3..12 and 17...31). 
*             	bitValue : reg value, should be 0 / 1 
*
* Return:       None
* 
* Note:         None
*/
void  BSP_GPIOSetValue(uint32_t GPIO_Pin, unsigned char bitValue);


#ifdef __cplusplus
}
#endif

#endif /* NL6621_GPIO_H */



