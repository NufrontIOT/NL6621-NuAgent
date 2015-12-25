/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  qspi.h
 *
 *    Description:  This file contains all the functions prototypes for the QSPI 
 *                  firmware library.
 *
 *        Version:  0.0.1
 *        Created:  2015/12/2 09:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/12/2    Create and initialize
 *
 * ====================================================================
 */

#ifndef QSPI_H
#define QSPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_conf.h"




//4种SPI协议
#define QSPI_FRF_SPI              0x00
#define QSPI_FRF_SSP              0x10
#define QSPI_FRF_MICROWIRE        0x20
#define QSPI_FRF_RESV             0x30

//4种传输模式
#define QSPI_TMOD_TR               0x0000        /* xmit & recv */
#define QSPI_TMOD_TO               0x0100        /* xmit only */
#define QSPI_TMOD_RO               0x0200        /* recv only */
#define QSPI_TMOD_EPROMREAD        0x0300        /* eeprom read mode */

//分频系数
#define QSPI_BaudRatePrescaler_2      2   //2~65534

//传输数据位数
//16bit
//#define SPI_DataSize                ((uint16_t)0x0F) //4~16bit
//8bit
#define QSPI_DataSize_8                 ((uint16_t)0x07)  

//SPI模式
#define QSPI_CPOL_Low                    ((uint16_t)0x0000)
#define QSPI_CPOL_High                   ((uint16_t)0x0080)
#define IS_QSPI_CPOL(CPOL) (((CPOL) == QSPI_CPOL_Low) || \
                           ((CPOL) == QSPI_CPOL_High))

#define QSPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define QSPI_CPHA_2Edge                  ((uint16_t)0x0040)
#define IS_QSPI_CPHA(CPHA) (((CPHA) == QSPI_CPHA_1Edge) || \
                           ((CPHA) == QSPI_CPHA_2Edge))


//#define SPI_SLVOE_OFFSET          10
//#define SPI_SRL_OFFSET            11
//#define SPI_CFS_OFFSET            12
//#define SPI_SLV_OE               0x1


//SR状态寄存器
#define QSPI_SR_MASK                0x7f        /* cover 7 bits */
#define QSPI_SR_BUSY                (1 << 0)	   /* spi总线在忙碌 */
#define QSPI_SR_TF_NOT_FULL         (1 << 1)
#define QSPI_SR_TF_EMPT             (1 << 2)
#define QSPI_SR_RF_NOT_EMPT         (1 << 3)
#define QSPI_SR_RF_FULL             (1 << 4)
#define QSPI_SR_TX_ERR              (1 << 5)
#define QSPI_SR_DCOL                (1 << 6)
#define IS_QSPI_GET_FLAG(FLAG) (((FLAG) == QSPI_SR_BUSY) || \
                               ((FLAG) == QSPI_SR_TF_NOT_FULL) || ((FLAG) == QSPI_SR_TF_EMPT) || \
                               ((FLAG) == QSPI_SR_RF_NOT_EMPT) || ((FLAG) == QSPI_SR_RF_FULL) || \
                               ((FLAG) == QSPI_SR_TX_ERR) || ((FLAG) == QSPI_SR_DCOL))



//IMR中断使能
#define   QSPI_IMR_MSTIM    (1<<5)
#define   QSPI_IMR_RXFIM    (1<<4)
#define   QSPI_IMR_RXOIM    (1<<3)
#define   QSPI_IMR_RXUIM    (1<<2)
#define   QSPI_IMR_TXOIM    (1<<1)
#define   QSPI_IMR_TXEIM    (1<<0)

//ISR状态寄存器
#define   QSPI_RISR_MSTIR    (1<<5)
#define   QSPI_RISR_RXFIR    (1<<4)
#define   QSPI_RISR_RXOIR    (1<<3)
#define   QSPI_RISR_RXUIR    (1<<2)
#define   QSPI_RISR_TXOIR    (1<<1)
#define   QSPI_RISR_TXEIR    (1<<0)

//清除中断标记
#define   QSPI_TXOICR_FLAG    1
#define   QSPI_RXOICR_FLAG    2
#define   QSPI_RXUICR_FLAG    4
#define   QSPI_MSTICR_FLAG    8
#define   QSPI_ICR_FLAG       0x0F

//DMA使能位
#define   QSPI_DMACR_TDMAE    0x02
#define   QSPI_DMACR_RDMAE    0x01

//DMA FIFO SIZE  
#define QSPI_RD_MODE_OFFSET            16
#define QSPI_STD_RD_MODE               0
#define QSPI_STD_FAST_RD_MODE          1
#define QSPI_DUAL_FAST_RD_MODE         2
#define QSPI_QUAD_FAST_RD_MODE         3
#define QSPI_DUAL_IO_FAST_RD_MODE      4
#define QSPI_QUAD_IO_FAST_RD_MODE      5
#define QSPI_QUAD_IO_WORD_RD_MODE      6
#define QSPI_OTHER_RD_MODE             7

#define QSPI_RD_PREFETCH_OFFSET        19



typedef PACKED struct _QSPI_REGS {
    VUINT32 CTRLR0;      // 0x00
    VUINT32 CTRLR1;      // 0x04
    VUINT32 SSIENR;      // 0x08
    VUINT32 BAUDR;       // 0x0C
    VUINT32 TXFTLR;      // 0x10 TX FIFO threshold level
    VUINT32 RXFTLR;      // 0x14 RX FIFO threshold level
    VUINT32 QSPI_TXFLR;       // 0x18
    VUINT32 QSPI_RXFLR;       // 0x1C
    VUINT32 SR;          // 0x20 status reg
    VUINT32 IMR;         // 0x24
    VUINT32 ISR;         // 0x28
    VUINT32 RISR;        // 0x2C
    VUINT32 TXOICR;      // 0x30
    VUINT32 RXOICR;      // 0x34
    VUINT32 RXUICR;      // 0x38
    VUINT32 AHBICR;      // 0x3C
    VUINT32 ICR;         // 0x40
    VUINT32 HOLD_WP;     // 0x44
    VUINT32 READ_CMD;    // 0x48
    VUINT32 PGM_CMD;     // 0x4C
    VUINT32 CACHE_FLUSH;      // 0x50
    VUINT32 CACHE_DIS_UPDATE; // 0x54
    VUINT32 RSVD[20];         // 
    VUINT32 DR;               // 
}  QSPI_REGS;

#ifndef EXT
  #define EXT extern
#endif /* EXT */

/*------------------------ Non Debug Mode ------------------------------------*/
#ifndef  DEBUG
#define NST_QSPI  ((QSPI_REGS *) _QSPI_BASE_ADR)
#else
/*------------------------ Debug Mode ----------------------------------------*/
EXT QSPI_REGS  *NST_QSPI; // 0x40140000
#endif


#define NST_WR_QSPI_REG(Value, Adr) \
    {\
        (*(volatile UINT32 *)(_QSPI_BASE_ADR + Adr)) = (UINT32)(Value);\
    }

#define NST_RD_QSPI_REG(Adr)  (*(volatile UINT32 *)(_QSPI_BASE_ADR + Adr))


/** 
  * @brief  SPI Init structure definition  
  */

typedef struct
{
  uint16_t QSPI_Direction;           /*!< Specifies the SPI unidirectional or bidirectional data mode.
                                         This parameter can be a value of @ref SPI_data_direction */

  uint16_t QSPI_Mode;                /*!< Specifies the SPI operating mode.
                                         This parameter can be a value of @ref SPI_mode */

  uint16_t QSPI_CPOL;                /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_Clock_Polarity */

  uint16_t QSPI_CPHA;               /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_Clock_Phase */

  uint16_t QSPI_BaudRatePrescaler;  /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_BaudRate_Prescaler.
                                         @note The communication clock is derived from the master
                                               clock. The slave clock does not need to be set. */
 
  uint16_t QSPI_DataSize;/*!< Specifies the SPI data size.
                                         This parameter can be a value of @ref SPI_data_size */
}QSPI_InitTypeDef;



/*
* Name:         QSPI_Init
* Description:  Initializes the SPI1 peripheral according to the specified 
*               parameters in the SPI_InitStruct.
* Parameter:    QSPI_InitStruct: pointer to a SPI_InitTypeDef structure that
*               contains the configuration information for the specified SPI peripheral.
*
* Return:       None
* 
* Note:         None
*/
void QSPI_Init(QSPI_InitTypeDef* QSPI_InitStruct);

/*
* Name:         QSPI_Cmd
* Description:  Enables or disables the specified SPI peripheral.
*
* Parameter:    NewState: new state of the QSPI1 peripheral. 
*               This parameter can be: ENABLE or DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void QSPI_Cmd(FunctionalState NewState);


/*
* Name:         QSPI_GetFlagStatus
* Description:  Checks whether the specified SPI flag is set or not.
*
* Parameter:    SPI_FLAG: specifies the QSPI1 flag to check. 
*
* Return:       The new state of SPI_FLAG (SET or RESET).
* 
* Note:         None
*/
FlagStatus QSPI_GetFlagStatus(uint16_t QSPI_FLAG);

/*
* Name:         QSPI_ReceiveData
* Description:  Returns the most recent received data by the QSPI1 peripheral. 
*
* Parameter:    None
*
* Return:       The value of the received data.
* 
* Note:         None
*/
uint16_t QSPI_ReceiveData(void);

/*
* Name:         QSPI_SendData
* Description:  Transmits a Data through the QSPI1 peripheral.
*
* Parameter:    Data : Data to be transmitted.
*
* Return:       None
* 
* Note:         None
*/
void QSPI_SendData(uint16_t Data);


/*
* Name:         QSPI_GetRxFIFOLevel
* Description:  Receive FIFO Level. Contains the number of valid data entries in the receive FIFO.
*
* Parameter:    None
*
* Return:       The value of the received FIFO Level.
* 
* Note:         None
*/
uint16_t QSPI_GetRxFIFOLevel(void);

/*
* Name:         QSPI_GetTxFIFOLevel
* Description:  Transmit FIFO Level. Contains the number of valid data entries in the transmit FIFO.
*
* Parameter:    None
*
* Return:       The value of the received FIFO Level.
* 
* Note:         None
*/
uint16_t QSPI_GetTxFIFOLevel(void);

/*
* Name:         QSPI_SetTxFIFOLevel
* Description:  Set Transmit FIFO Level. 
*
* Parameter:    tx:Controls the level of entries (or below) at which the transmit FIFO controller triggers an interrupt. 
*
* Return:       None
* 
* Note:         None
*/
void QSPI_SetTxFIFOLevel(uint16_t tx);

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
void QSPI_SetRxFIFOLevel(uint16_t rx);

/*
* Name:         QSPI_Clear_Interrupt
* Description:  Clears the SPI interrupt pending bits.
*
* Parameter:    QSPI_IT:Clear interrupt. 
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
void QSPI_Clear_Interrupt(uint8_t QSPI_IT);

/*
* Name:         QSPI_ITConfig
* Description:  Enables or disables the specified QSPI interrupts.
*
* Parameter:    QSPI_IT: specifies the QSPI interrupts sources to be enabled or disabled.
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
void QSPI_ITConfig(uint16_t QSPI_IT, FunctionalState NewState);

/*
* Name:         QSPI_GetFlagStatus
* Description:  Checks whether the specified QSPI flag is set or not.
*
* Parameter:    QSPI_FLAG: specifies the QSPI flag to check. 
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
FlagStatus QSPI_GetITFlagStatus(uint16_t QSPI_FLAG);

/*
* Name:         BSP_QSpiWriteByte
* Description:  Transmits a Data through the QSPI1 peripheral.
*
* Parameter:    Byte : Data to be transmitted.
*
* Return:       None
* 
* Note:         None
*/
void BSP_QSpiWriteByte(uint8_t Byte);

/*
* Name:         BSP_QSpiRead
* Description:  Returns the most recent received data by the QSPI1 peripheral. 
*
* Parameter:    RdCnt: read data count.
*               pBuf:  read data point.
*
* Return:       None
* 
* Note:         None
*/
void BSP_QSpiRead(uint32_t RdCnt, uint8_t* pBuf);

/*
* Name:         BSP_QSpiWait
* Description:  Waiting for QSPI free.
*
* Parameter:    None
*
* Return:       None
* 
* Note:         None
*/
void BSP_QSpiWait(void);


#ifdef __cplusplus
}
#endif

#endif /* QSPI_H */

