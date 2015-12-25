/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  spi.h
 *
 *    Description:  This file contains all the functions prototypes for the SPI 
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

#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_conf.h"



//4种SPI协议
#define SPI_FRF_SPI              0x00
#define SPI_FRF_SSP              0x10
#define SPI_FRF_MICROWIRE        0x20
#define SPI_FRF_RESV             0x30

//4种传输模式
#define SPI_TMOD_TR               0x0000        /* xmit & recv */
#define SPI_TMOD_TO               0x0100        /* xmit only */
#define SPI_TMOD_RO               0x0200        /* recv only */
#define SPI_TMOD_EPROMREAD        0x0300        /* eeprom read mode */

//分频系数
#define SPI_BaudRatePrescaler_2      2   //2~65534

//传输数据位数
//16bit
//#define SPI_DataSize                ((uint16_t)0x0F) //4~16bit
//8bit
#define SPI_DataSize_8                 ((uint16_t)0x07)  

//SPI模式
#define SPI_CPOL_Low                    ((uint16_t)0x0000)
#define SPI_CPOL_High                   ((uint16_t)0x0080)
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_Low) || \
                           ((CPOL) == SPI_CPOL_High))

#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define SPI_CPHA_2Edge                  ((uint16_t)0x0040)
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1Edge) || \
                           ((CPHA) == SPI_CPHA_2Edge))


//#define SPI_SLVOE_OFFSET          10
//#define SPI_SRL_OFFSET            11
//#define SPI_CFS_OFFSET            12
//#define SPI_SLV_OE               0x1


//SR状态寄存器
/* Bit fields in SR, 7 bits */
#define SR_MASK                0x7f        /* cover 7 bits */
#define SR_BUSY                (1 << 0)	   /* spi总线在忙碌 */
#define SR_TF_NOT_FULL         (1 << 1)
#define SR_TF_EMPT             (1 << 2)
#define SR_RF_NOT_EMPT         (1 << 3)
#define SR_RF_FULL             (1 << 4)
#define SR_TX_ERR              (1 << 5)
#define SR_DCOL                (1 << 6)
#define IS_SPI_GET_FLAG(FLAG) (((FLAG) == SR_BUSY) || \
                               ((FLAG) == SR_TF_NOT_FULL) || ((FLAG) == SR_TF_EMPT) || \
                               ((FLAG) == SR_RF_NOT_EMPT) || ((FLAG) == SR_RF_FULL) || \
                               ((FLAG) == SR_TX_ERR) || ((FLAG) == SR_DCOL))



//IMR中断使能
#define   IMR_MSTIM    (1<<5)
#define   IMR_RXFIM    (1<<4)
#define   IMR_RXOIM    (1<<3)
#define   IMR_RXUIM    (1<<2)
#define   IMR_TXOIM    (1<<1)
#define   IMR_TXEIM    (1<<0)

//ISR状态寄存器
#define   RISR_MSTIR    (1<<5)
#define   RISR_RXFIR    (1<<4)
#define   RISR_RXOIR    (1<<3)
#define   RISR_RXUIR    (1<<2)
#define   RISR_TXOIR    (1<<1)
#define   RISR_TXEIR    (1<<0)

//清除中断标记
#define   TXOICR_FLAG    1
#define   RXOICR_FLAG    2
#define   RXUICR_FLAG    4
#define   MSTICR_FLAG    8
#define   ICR_FLAG       0x0F

//DMA使能位
#define   DMACR_TDMAE    0x02
#define   DMACR_RDMAE    0x01

//DMA FIFO SIZE  




typedef PACKED struct _SPI_REGS {
    VUINT32    CTRLR0;      // 0x00
    VUINT32    CTRLR1;      // 0x04
    VUINT32    SSIENR;      // 0x08
    VUINT32    MWCR;        // 0x0C
    VUINT32    SER;         // 0x10
    VUINT32    BAUDR;       // 0x14
    VUINT32    TXFTLR;      // 0x18 TX FIFO threshold level
    VUINT32    RXFTLR;      // 0x1C RX FIFO threshold level
    VUINT32    SPI_TXFLR;   // 0X20
    VUINT32    SPI_RXFLR;   // 0X24
    VUINT32    SR;          // 0X28 status reg
    VUINT32    IMR;         // 0x2C
    VUINT32    ISR;         // 0x30
    VUINT32    RISR;        // 0x34
    VUINT32    TXOICR;      // 0x38
    VUINT32    RXOICR;      // 0x3C
    VUINT32    RXUICR;      // 0x40
    VUINT32    MSTICR;      // 0x44
    VUINT32    ICR;         // 0x48
    VUINT32    DMACR;       // 0x4C
    VUINT32    DMATDLR;     // 0x50
    VUINT32    DMARDLR;     // 0x54
    VUINT32    IDR;         // 0x58
    VUINT32    VERSION;     // 0x5C

    /* Currently operates as 32 bits, though only the low 16 bits matter */
    VUINT32    DR;          // 0x60
}  SPI_REGS;

#ifndef EXT
  #define EXT extern
#endif /* EXT */

/*------------------------ Non Debug Mode ------------------------------------*/
#ifndef  DEBUG
#define NST_SPI  ((SPI_REGS *) _SPI_BASE_ADR)
#else
/*------------------------ Debug Mode ----------------------------------------*/
EXT SPI_REGS  *NST_SPI;
#endif


#define NST_WR_SPI_REG(Value, Adr) \
    {\
        (*(volatile UINT32 *)(_SPI_BASE_ADR + Adr)) = (UINT32)(Value);\
    }

#define NST_RD_SPI_REG(Adr)  (*(volatile UINT32 *)(_SPI_BASE_ADR + Adr))


/** 
  * @brief  SPI Init structure definition  
  */

typedef struct
{
  uint16_t SPI_Direction;           /*!< Specifies the SPI unidirectional or bidirectional data mode.
                                         This parameter can be a value of @ref SPI_data_direction */

  uint16_t SPI_Mode;                /*!< Specifies the SPI operating mode.
                                         This parameter can be a value of @ref SPI_mode */

  uint16_t SPI_CPOL;                /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_Clock_Polarity */

  uint16_t SPI_CPHA;               /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_Clock_Phase */
  uint16_t SPI_DevNum;           /*!< Specifies whether data transfers start from MSB or LSB bit.
                                         This parameter can be a value of @ref SPI_MSB_LSB_transmission */

  uint16_t SPI_BaudRatePrescaler;  /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_BaudRate_Prescaler.
                                         @note The communication clock is derived from the master
                                               clock. The slave clock does not need to be set. */
 
  uint16_t SPI_DataSize;/*!< Specifies the SPI data size.
                                         This parameter can be a value of @ref SPI_data_size */
}SPI_InitTypeDef;



/*
* Name:         SPI_Init
* Description:  Initializes the SPI1 peripheral according to the specified 
*               parameters in the SPI_InitStruct.
* Parameter:    SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
*               contains the configuration information for the specified SPI peripheral.
*
* Return:       None
* 
* Note:         None
*/
void SPI_Init(SPI_InitTypeDef* SPI_InitStruct);

/*
* Name:         SPI_Cmd
* Description:  Enables or disables the specified SPI peripheral.
*
* Parameter:    NewState: new state of the SPI1 peripheral. 
*               This parameter can be: ENABLE or DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void SPI_Cmd(FunctionalState NewState);


/*
* Name:         SPI_GetFlagStatus
* Description:  Checks whether the specified SPI flag is set or not.
*
* Parameter:    SPI_FLAG: specifies the SPI1 flag to check. 
*
* Return:       The new state of SPI_FLAG (SET or RESET).
* 
* Note:         None
*/
FlagStatus SPI_GetFlagStatus(uint16_t SPI_FLAG);

/*
* Name:         SPI_ReceiveData
* Description:  Returns the most recent received data by the SPI1 peripheral. 
*
* Parameter:    None
*
* @Param1[IN]:	None
*
* Return:       The value of the received data.
* 
* Note:         None
*/
uint16_t SPI_ReceiveData(void);

/*
* Name:         SPI_SendData
* Description:  Transmits a Data through the SPI1 peripheral.
*
* Parameter:    Data : Data to be transmitted.
*
* Return:       None
* 
* Note:         None
*/
void SPI_SendData(uint16_t Data);


/*
* Name:         SPI_GetRxFIFOLevel
* Description:  Receive FIFO Level. Contains the number of valid data entries in the receive FIFO.
*
* Parameter:    None
*
* Return:       The value of the received FIFO Level.
* 
* Note:         None
*/
uint16_t SPI_GetRxFIFOLevel(void);

/*
* Name:         SPI_GetTxFIFOLevel
* Description:  Transmit FIFO Level. Contains the number of valid data entries in the transmit FIFO.
*
* Parameter:    None
*
* Return:       The value of the received FIFO Level.
* 
* Note:         None
*/
uint16_t SPI_GetTxFIFOLevel(void);

/*
* Name:         SPI_SetTxFIFOLevel
* Description:  Set Transmit FIFO Level. 
*
* Parameter:    tx:Controls the level of entries (or below) at which the transmit FIFO controller triggers an interrupt. 
*
* Return:       None
* 
* Note:         None
*/
void SPI_SetTxFIFOLevel(uint16_t tx);

/*
* Name:         SPI_SetRxFIFOLevel
* Description:  Set Transmit FIFO Level. 
*
* Parameter:    rx:Controls the level of entries (or above) at which the receive FIFO controller triggers an interrupt. 
*
* Return:       None
* 
* Note:         None
*/
void SPI_SetRxFIFOLevel(uint16_t rx);

/*
* Name:         SPI_Clear_Interrupt
* Description:  Clears the SPI interrupt pending bits.
*
* Parameter:    SPI_IT:Clear interrupt. 
*               This parameter can be any combination of the following values:
*               @arg TXOICR_FLAG: Clear Transmit FIFO Overflow Interrupt.
*               @arg RXOICR_FLAG: Clear Receive FIFO Overflow Interrupt. 
*               @arg RXUICR_FLAG: Clear Receive FIFO Underflow Interrupt. 
*               @arg MSTICR_FLAG: Clear Receive FIFO Underflow Interrupt. 
*               @arg ICR_FLAG: Clear Receive FIFO All Interrupt. 
*
* Return:       None
* 
* Note:         None
*/
void SPI_Clear_Interrupt(uint8_t SPI_IT);

/*
* Name:         SPI_ITConfig
* Description:  Enables or disables the specified SPI interrupts.
*
* Parameter:    SPI_IT: specifies the SPI interrupts sources to be enabled or disabled.
*               This parameter can be any combination of the following values:
*               @arg IMR_MSTIM: Multi-Master Contention Interrupt Mask. 
*               @arg IMR_RXFIM: Receive FIFO Full Interrupt Mask.
*               @arg IMR_RXOIM: Receive FIFO Overflow Interrupt Mask.
*               @arg IMR_RXUIM: Receive FIFO Underflow Interrupt Mask.
*               @arg IMR_TXOIM: Transmit FIFO Overflow Interrupt Mask.
*               @arg IMR_TXEIM: Transmit FIFO Empty Interrupt Mask.
*
*               NewState: new state of the TIM interrupts.
*
* Return:       None
* 
* Note:         None
*/
void SPI_ITConfig(uint16_t SPI_IT, FunctionalState NewState);

/*
* Name:         SPI_GetFlagStatus
* Description:  Checks whether the specified SPI flag is set or not.
*
* Parameter:    SPI_FLAG: specifies the SPI flag to check. 
*               This parameter can be any combination of the following values:
*               @arg RISR_MSTIR: Multi-Master Contention Interrupt Status. 
*               @arg RISR_RXFIR: Receive FIFO Full Interrupt Status.
*               @arg RISR_RXOIR: Receive FIFO Overflow Interrupt Status.
*               @arg RISR_RXUIR: Receive FIFO Underflow Interrupt Status.
*               @arg RISR_TXOIR: Transmit FIFO Overflow Interrupt Status.
*               @arg RISR_TXEIR: Transmit FIFO Empty Interrupt Status.
*
*               NewState: new state of the TIM interrupts.
*
* Return:       The new state of SPI_FLAG (SET or RESET).
* 
* Note:         None
*/
FlagStatus SPI_GetITFlagStatus(uint16_t SPI_FLAG);


/*
* Name:         SPI_DMA_Cmd
* Description:  Enables or disables the specified SPI DMA and Set DMA RX/TX FIFO.
*
* Parameter:    DMA_TYPE: DMA RX  and  DMA TX
*               This parameter can be any combination of the following values:
*               @arg DMACR_TDMAE:  Transmit DMA Enable.
*               @arg DMACR_RDMAE:  Receive DMA Enable. 
*
*               length: Transmit or Receive Data Level
*
* Return:       None
* 
* Note:         None
*/
void SPI_DMA_Cmd(uint16_t DMA_TYPE,uint16_t Length);




#ifdef __cplusplus
}
#endif

#endif /* NL6621_SPI_H */

