/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  nl6621_i2c.h
 *
 *    Description:  This file contains all the functions prototypes for the I2C 
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

#ifndef NL6621_I2C_H
#define NL6621_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nl6621_conf.h"


/*EEPROM start base address*/
#define EEPROM_PARAM_START_ADDR	    	0x600


/* Control register */
#define IC_CON              0x00
#define SLV_DIS            (1 << 6)        /* Disable slave mode */
#define RESTART            (1 << 5)        /* Send a Restart condition */
#define ADDR_10BIT         (1 << 4)        /* 10-bit addressing */
#define STANDARD_MODE      (1 << 1)        /* standard mode */
#define FAST_MODE          (2 << 1)        /* fast mode */
#define HIGH_MODE          (3 << 1)        /* high speed mode */
#define MASTER_EN          (1 << 0)        /* Master mode */



/* Target address register */
#define IC_TAR              0x04
#define IC_TAR_10BIT_ADDR  (1 << 12)    /* 10-bit addressing */
#define IC_TAR_SPECIAL     (1 << 11)    /* Perform special I2C cmd */
#define IC_TAR_GC_OR_START (1 << 10)    /* 0: Gerneral Call Address */
                                        /* 1: START BYTE */


/* Slave Address Register */
#define IC_SAR                0x08        /* Not used in Master mode */


/* High Speed Master Mode Code Address Register */
#define IC_HS_MADDR           0x0c

/* Rx/Tx Data Buffer and Command Register */
#define IC_DATA_CMD           0x10
#define IC_RD                 (1 << 8)    /* 1: Read 0: Write */

/* Standard Speed Clock SCL High Count Register */
#define IC_SS_SCL_HCNT        0x14

/* Standard Speed Clock SCL Low Count Register */
#define IC_SS_SCL_LCNT        0x18

/* Fast Speed Clock SCL High Count Register */
#define IC_FS_SCL_HCNT        0x1c

/* Fast Spedd Clock SCL Low Count Register */
#define IC_FS_SCL_LCNT        0x20

/* High Speed Clock SCL High Count Register */
#define IC_HS_SCL_HCNT        0x24

/* High Speed Clock SCL Low Count Register */
#define IC_HS_SCL_LCNT        0x28

/* Interrupt Status Register */
#define IC_INTR_STAT          0x2c        /* Read only */
#define R_GEN_CALL           (1 << 11)
#define R_START_DET          (1 << 10)
#define R_STOP_DET           (1 << 9)
#define R_ACTIVITY           (1 << 8)
#define R_RX_DONE            (1 << 7)
#define R_TX_ABRT            (1 << 6)
#define R_RD_REQ             (1 << 5)
#define R_TX_EMPTY           (1 << 4)
#define R_TX_OVER            (1 << 3)
#define R_RX_FULL            (1 << 2)
#define R_RX_OVER            (1 << 1)
#define R_RX_UNDER           (1 << 0)

/* Interrupt Mask Register */
#define IC_INTR_MASK          0x30        /* Read and Write */
#define M_GEN_CALL           (1 << 11)
#define M_START_DET          (1 << 10)
#define M_STOP_DET           (1 << 9)
#define M_ACTIVITY           (1 << 8)
#define M_RX_DONE            (1 << 7)
#define M_TX_ABRT            (1 << 6)
#define M_RD_REQ             (1 << 5)
#define M_TX_EMPTY           (1 << 4)
#define M_TX_OVER            (1 << 3)
#define M_RX_FULL            (1 << 2)
#define M_RX_OVER            (1 << 1)
#define M_RX_UNDER           (1 << 0)

/* Raw Interrupt Status Register */
#define IC_RAW_INTR_STAT      0x34        /* Read Only */
#define GEN_CALL             (1 << 11)    /* General call */
#define START_DET            (1 << 10)    /* (RE)START occured */
#define STOP_DET             (1 << 9)    /* STOP occured */
#define ACTIVITY             (1 << 8)    /* Bus busy */
#define RX_DONE              (1 << 7)    /* Not used in Master mode */
#define TX_ABRT              (1 << 6)    /* Transmit Abort */
#define RD_REQ               (1 << 5)    /* Not used in Master mode */
#define TX_EMPTY             (1 << 4)    /* TX FIFO <= threshold */
#define TX_OVER              (1 << 3)    /* TX FIFO overflow */
#define RX_FULL              (1 << 2)    /* RX FIFO >= threshold */
#define RX_OVER              (1 << 1)    /* RX FIFO overflow */
#define RX_UNDER             (1 << 0)    /* RX FIFO empty */

/* Receive FIFO Threshold Register */
#define IC_RX_TL         0x38

/* Transmit FIFO Treshold Register */
#define IC_TX_TL         0x3c

/* Clear Combined and Individual Interrupt Register */
#define IC_CLR_INTR      0x40
#define CLR_INTR        (1 << 0)

/* Clear RX_UNDER Interrupt Register */
#define IC_CLR_RX_UNDER  0x44
#define CLR_RX_UNDER    (1 << 0)

/* Clear RX_OVER Interrupt Register */
#define IC_CLR_RX_OVER   0x48
#define CLR_RX_OVER     (1 << 0)

/* Clear TX_OVER Interrupt Register */
#define IC_CLR_TX_OVER   0x4c
#define CLR_TX_OVER     (1 << 0)

#define IC_CLR_RD_REQ    0x50

/* Clear TX_ABRT Interrupt Register */
#define IC_CLR_TX_ABRT   0x54
#define CLR_TX_ABRT     (1 << 0)
#define IC_CLR_RX_DONE   0x58

/* Clear ACTIVITY Interrupt Register */
#define IC_CLR_ACTIVITY  0x5c
#define CLR_ACTIVITY    (1 << 0)

/* Clear STOP_DET Interrupt Register */
#define IC_CLR_STOP_DET  0x60
#define CLR_STOP_DET    (1 << 0)

/* Clear START_DET Interrupt Register */
#define IC_CLR_START_DET 0x64
#define CLR_START_DET   (1 << 0)

/* Clear GEN_CALL Interrupt Register */
#define IC_CLR_GEN_CALL  0x68
#define CLR_GEN_CALL    (1 << 0)

/* Enable Register */
#define IC_ENABLE        0x6c
#define ENABLE_BIT      (1 << 0)

/* Status Register */
#define IC_STATUS             0x70        /* Read Only */
#define STAT_SLV_ACTIVITY    (1 << 6)    /* Slave not in idle */
#define STAT_MST_ACTIVITY    (1 << 5)    /* Master not in idle */
#define STAT_RFF             (1 << 4)    /* RX FIFO Full */
#define STAT_RFNE            (1 << 3)    /* RX FIFO Not Empty */
#define STAT_TFE             (1 << 2)    /* TX FIFO Empty */
#define STAT_TFNF            (1 << 1)    /* TX FIFO Not Full */
#define STAT_ACTIVITY        (1 << 0)    /* Activity Status */

/* Transmit FIFO Level Register */
#define IC_TXFLR              0x74        /* Read Only */
#define TXFLR                (1 << 0)    /* TX FIFO level */

/* Receive FIFO Level Register */
#define IC_RXFLR              0x78        /* Read Only */
#define RXFLR                (1 << 0)    /* RX FIFO level */

/* Transmit Abort Source Register */
#define IC_TX_ABRT_SOURCE     0x80
#define ABRT_SLVRD_INTX      (1 << 15)
#define ABRT_SLV_ARBLOST     (1 << 14)
#define ABRT_SLVFLUSH_TXFIFO (1 << 13)
#define ABRT_LOST            (1 << 12)
#define ABRT_MASTER_DIS      (1 << 11)
#define ABRT_10B_RD_NORSTRT  (1 << 10)
#define ABRT_SBYTE_NORSTRT   (1 << 9)
#define ABRT_HS_NORSTRT      (1 << 8)
#define ABRT_SBYTE_ACKDET    (1 << 7)
#define ABRT_HS_ACKDET       (1 << 6)
#define ABRT_GCALL_READ      (1 << 5)
#define ABRT_GCALL_NOACK     (1 << 4)
#define ABRT_TXDATA_NOACK    (1 << 3)
#define ABRT_10ADDR2_NOACK   (1 << 2)
#define ABRT_10ADDR1_NOACK   (1 << 1)
#define ABRT_7B_ADDR_NOACK   (1 << 0)

/* Enable Status Register */
#define IC_ENABLE_STATUS      0x9C
#define IC_EN                (1 << 0)    /* I2C in an enabled state */

/* Component Parameter Register 1*/
#define IC_COMP_PARAM_1       0xF4
#define APB_DATA_WIDTH       (0x3 << 0)

#define IC_SDA_SETUP            0x94

/* added by xiaolin --begin */
#define SS_MIN_SCL_HIGH         4000
#define SS_MIN_SCL_LOW          4700
#define FS_MIN_SCL_HIGH         600
#define FS_MIN_SCL_LOW          1300
#define HS_MIN_SCL_HIGH_100PF   60
#define HS_MIN_SCL_LOW_100PF    120


//Louis add   I2C DMA传输
#define IC_DMA_CR       0x88
#define IC_TDMAE        0x02
#define IC_RDMAE        0x01
//I2C DMA传输个数
#define IC_DMA_TDLR	    0x8C
#define IC_DMA_RDLR		0x90

#define IC_ACK_GENERAL_CALL 0x98
#define IC_ENABLE_STATUS    0x9C
#define IC_FS_SPKLEN		0xA0



typedef struct _NST_I2C_PRIV {
    UINT8    Speed;
    UINT32   ClkFrq;
    INT16    TargetAdr;
} NST_I2C_PRIV;


#define STANDARD        1
#define FAST            2
#define HIGH            3

#define I2C_CLK_FRQ     1


//#define AT24C08
#define E2PROM_ADDR       (0xA0 >> 1) // HW475(0xA0) HW240(0xA8) 
#define E2PROM_HIGH_ADDR  (0xA2 >> 1) // HW475(0xA2) HW240(0xAA) 
#define E2PROM_SIZE        0x40000
#define E2HIGH_SPACE      (1 << 16)


#define IC_INTR_DEFAULT_MASK     (RX_FULL | TX_EMPTY | TX_ABRT | STOP_DET)

 
#define NST_WR_I2C_REG(Value, Adr) \
    {\
        (*(volatile UINT32 *)(_I2C_BASE_ADR + Adr)) = (UINT32)(Value);\
    }

#define NST_RD_I2C_REG(Adr)  (*(volatile UINT32 *)(_I2C_BASE_ADR + Adr))

#define ADR_HIGH_PART(adr)        ((adr >> 16) & (0x03))

typedef struct
{
  uint16_t I2C_ClockHihg;           /*!< Specifies the clock frequency.(HIHG) */

  uint16_t I2C_ClockLow;            /*!< Specifies the clock frequency.(LOW) */
  
  uint16_t I2C_Mode;                /*!< Specifies the I2C mode.
                                         This parameter can be a value of @ref I2C_mode */

  uint16_t I2C_OwnAddress1;         /*!< Specifies the first device own address.
                                         This parameter can be a 7-bit or 10-bit address. */
}I2C_InitTypeDef;


/*
* Name:         I2C_Init
* Description:  Enables or disables the specified TIM interrupts.
*               
* Parameter:    TIMx: where x can be 0 to 1 to select the TIMx peripheral.
*
* Return:       None
* 
* Note:         None
*/
void I2C_Init(I2C_InitTypeDef* I2C_InitStruct);

/*
* Name:         I2C_Cmd
* Description:  Enables or disables the specified I2C peripheral.
*               
* Parameter:    NewState: new state of the I2C1 peripheral. 
*               This parameter can be: ENABLE or DISABLE.
*
* Return:       None
* 
* Note:         None
*/
void I2C_Cmd(FunctionalState NewState);

/*
* Name:         I2C_DMA_Cmd
* Description:  Enables or disables the specified I2C DMA and Set DMA RX/TX FIFO.
*
* Parameter:    DMA_TYPE: DMA RX  and  DMA TX
*               This parameter can be any combination of the following values:
*               @arg IC_TDMAE:  Transmit DMA Enable.
*               @arg IC_RDMAE:  Receive DMA Enable. 
*
*               length: new state of the TIM interrupts.
*
* Return:       None
* 
* Note:         None
*/
void I2C_DMA_Cmd(uint16_t DMA_TYPE,uint16_t Length);

/*
* Name:         I2C_WaitForBus
* Description:  Waiting bus I2C release.
*
* Parameter:    None
*               
* Return:       None
* 
* Note:         None
*/
void I2C_WaitForBus(void);

/*
* Name:         I2C_XferFinish
* Description:  Waiting for the bus to receive.
*
* Parameter:    None
*               
* Return:       None
* 
* Note:         None
*/
void I2C_XferFinish(void);


/*
* Name:         BSP_I2CSetAdr
* Description:  Set read and write address.
*
* Parameter:    Adr: Device address	 [AT24C08 Range(0~1023)]
*               time: Timeout check answer signal.(us)
*
* Return:       None
* 
* Note:         None
*/
void BSP_I2CSetAdr(uint32_t Adr,uint32_t time);


/*
* Name:         BSP_I2CRead
* Description:  Continuously read data.
*
* Parameter:    pBuf: Read data buffer
*
*               Len:  Read data length	 [AT24C08 Range(0~1023)]
*
*               time: Timeout check answer signal.(us)
*               
* Return:      BOOL_T (NST_TRUE or NST_FALSE).
* 
* Note:         None
*/
BOOL_T BSP_I2CRead(uint8_t* pBuf, uint32_t Len, uint32_t time);

/*
* Name:         BSP_I2CWrite
* Description:  Continuously write data.
*
* Parameter:    pBuf: Write data buffer
*
*               Len:  Write data length	 [AT24C08 Range(0~1023)]
*
* Return:       BOOL_T (NST_TRUE or NST_FALSE).
* 
* Note:         None
*/
BOOL_T BSP_I2CWrite(uint8_t* pBuf, uint32_t Len);

/*
* Name:         BSP_I2CSeqRead
* Description:  Read device for each address of multiple data
*
* Parameter:    pBuf: Write data buffer
*
*               Len:  Write data length	 [AT24C08 Range(0~1023)]
*
* Return:       BOOL_T (NST_TRUE or NST_FALSE).
* 
* Note:         None
*/
BOOL_T BSP_I2CSeqRead(uint32_t Adr, uint8_t *pBuf, uint32_t Len);

/*
* Name:         BSP_I2CSeqWrite
* Description:  Write device for each address of multiple data
*
* Parameter:    pBuf: Write data buffer
*
*               Len:  Write data length	 [AT24C08 Range(0~1023)]
*
* Return:       BOOL_T (NST_TRUE or NST_FALSE).
* 
* Note:         None
*/
BOOL_T BSP_I2CSeqWrite(uint32_t Adr, uint8_t *pBuf, uint32_t Len);

/*
* Name:         BSP_I2CByteWrite
* Description:  Write device for each address data.
*
* Parameter:    Adr: Write data Address.
*
*               pByte: Write data.
*
* Return:       BOOL_T (NST_TRUE or NST_FALSE).
* 
* Note:         None
*/
void   BSP_I2CByteWrite(UINT32 Adr, uint8_t* pByte);

/*
* Name:         BSP_I2CByteRead
* Description:  Read device for each address data.
*
* Parameter:    Adr: Read data Address.
*
*               pByte: Read data.
*
* Return:       None
* 
* Note:         None
*/
void   BSP_I2CByteRead(uint32_t Adr, UINT8* pByte);


#ifdef __cplusplus
}
#endif

#endif /* NL6621_TIMER_H */

