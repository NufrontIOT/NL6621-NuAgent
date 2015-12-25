/*
 * ====================================================================
 *     Copyright: (c) 2015 GuangDong  Nufront SOC Chip Co., Ltd.
 *     All rights reserved.
 *
 *       Filename:  BspDemo.h
 *
 *    Description:  NL6621 BSP Demo(gpio,exti,usart,spi,wdg,spi,timer,i2c,dma,i2s,sdio)                 
 *
 *        Version:  0.0.1
 *        Created:  2015/12/3 15:47:43
 *       Revision:  none
 *
 *         Author:  Hui Lou (Link), louis_zhl@foxmail.com
 *   Organization:  Nufront
 *
 *--------------------------------------------------------------------------------------          
 *       ChangLog:
 *        version    Author      Date        Purpose
 *        0.0.1      Hui Lou    2015/12/3    Create and initialize
 *
 * ====================================================================
 */

#ifndef BSPDEMO_H
#define BSPDEMO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_conf.h"





//#define GPIO_Demo                //GPIO输入输出例子
//#define GPIO_EXTI_Demo	         //GPIO中断例子
#define USART_Demo	             //USART中断例子
#define TIMER_Demo               //定时器例子
//#define WATCH_Demo               //看门狗例子
//#define I2C_EERPOM_Demo          //EEPROM例子
#define QSPI_FLASH_Demo          //QSPI FLASH例子
//#define HW_I2S_SUPPORT		 //I2S读写功能
//#define DMA_MOVE_MEM		     //使用DMA搬运内存

/*如果需要测试SPI Master传输数据，打开SPI_FLASH_Demo and SPI_SDIO_CMD_TEST*/
//#if 1
//	#define SPI_FLASH_Demo           //SPI FLASH例子  
//	#define SPI_SDIO_CMD_TEST		 //SPI master传输数据 --> SDIO
//#else
///*如果需要测试SDIO Slave传输数据，打开SDIO_TEST*/
//	#define SDIO_TEST				 //SDIO传输数据
//#endif



void NL6621_BSP_TEST(void);

/*******************************************************/
//NL6621 GPIO设置输入输出Demo
/*******************************************************/
#ifdef	GPIO_Demo
	void GPIO_InOut_Demo(void);
#endif

/*******************************************************/
//NL6621 GPIO设置中断Demo
/*******************************************************/
#ifdef	GPIO_EXTI_Demo
	void GPIO_Interrupt_Demo(void);
	void BSP_GPIOIntISR_Demo(uint32_t EXTI_Line);
#endif

/*******************************************************/
//NL6621 串口Demo
/*******************************************************/
#ifdef  USART_Demo
	void USART_Init_Demo(uint32_t bound);
	void BSP_UartISR_Demo(void);
#endif

/*******************************************************/
//NL6621 定时器Demo
/*******************************************************/
#ifdef TIMER_Demo
	void TMR0_IRQFunc(void); 
	void TMR1_IRQFunc(void); 
#endif 

/*******************************************************/
//NL6621 看门狗Demo
/*******************************************************/
#ifdef WATCH_Demo
	void IWDG_Init_Demo(void); 
	void WATCH_Feed_Demo(void);
#endif

/*******************************************************/
//NL6621 EEPROM Demo
/*******************************************************/
#ifdef I2C_EERPOM_Demo
	int eeprom_data_read(unsigned int addr, unsigned int length, unsigned char * buf);
	int eeprom_data_write(unsigned int addr, unsigned int length, unsigned char * buf);
	int eeprom_init(void);
#endif

/*******************************************************/
//NL6621 QSPI FLASH Demo
/*******************************************************/
#ifdef SPI_FLASH_Demo   /* USE SPI_FLASH_Demo */
	/* flash io interface */
	void BSP_SpiInit(void);
	void SpiFlashEraseChip(void);
	void SpiFlashRead(uint32_t ReadStartPos, uint8_t* pBuf, uint32_t Len);  
	void SpiFlashWriteOnePage(uint32_t Adr, uint8_t* pBuf,  uint16_t DataLen); 
	void SpiFlashEraseSector(uint32_t SectorAddr);
	
	#ifdef SPI_SDIO_CMD_TEST	
		void BSP_SpiEum(void);
		void BSP_SpiCmd53Read(uint16_t bytecnt,uint8_t* pBuf);
		void BSP_SpiCmd53Write(uint16_t bytecnt,uint8_t* pBuf);
		void BSP_SpiSdioCmdTest(void);
	#endif
#endif	  /* USE SPI_FLASH_Demo END*/

/*******************************************************/
//NL6621 SPI FLASH Demo
/*******************************************************/
#ifdef QSPI_FLASH_Demo 	   /* USE QSPI_FLASH_Demo */
	void  BSP_QSpiInit(uint16_t SpiClkDiv);
	void  QSpiFlashRead(uint32_t ReadStartPos, uint8_t* pBuf, uint32_t Len); 
	void  QSpiFlashEraseChip(void);
	void  QSpiFlashWriteOnePage(uint32_t Adr, uint8_t* pBuf, uint16_t DataLen); 
	void  QSpiFlashEraseSector(uint32_t SectorAddr);
	int32_t QSpiWriteOneSector(uint32_t SectorAddr, uint8_t* pBuf);
	int32_t QSpiWriteAny(uint32_t WriteAddr, uint8_t* pData, uint16_t DataLen);  
#endif /* USE QSPI_FLASH_Demo END*/





#if(defined SPI_FLASH_Demo || defined QSPI_FLASH_Demo)  /* USE SPI_FLASH_Demo OR QSPI_FLASH_Demo */
	// Flash芯片选择，只能选择一个
	
#define DUMMY_BYTE                  0x55
#define MAX_FLASH_RD_SIZE           65536

#define FLASH_PAGE_SIZE             256
#define FLASH_SECTOR_SIZE           4096
#define FLASH_BLOCK64_SIZE          1024*64
#define FLASH_BLOCK32_SIZE          1024*32


// Flash 命令定义
#define  FLASH_CMD_SEQ_READ          0x03


#define  FLASH_CMD_WREN              0x06
#define  FLASH_CMD_WRDIS             0x04
#define  FLASH_CMD_CHIP_ERASE        0xC7 // chip erase
#define  FLASH_CMD_SECTOR_ERASE      0x20 // sector erase, 0x20
#define  FLASH_CMD_BLOCK64_ERASE     0xD8 // block erase, 64K(0xD8) 
#define  FLASH_CMD_BLOCK32_ERASE     0x52 // block erase, 32K(0x52)
#define  FLASH_CMD_PAGE_WRITE        0x02
#define  FLASH_CMD_RD_SR             0x05 // read status register
#define  FLASH_CMD_WR_SR             0x01 // read status register
#define  FLASH_SR_BUSY              (1 << 0)  // 1: busy, 0: ready
#define  FLASH_SR_WEL               (1 << 1)  // 1: write enable 

/* Flash 地址空间分配*/
#define NVM_MAX_SIZE				0x80000   // 4Mbits 
//#define NVM_MAX_SIZE				0x100000 // 8Mbits 

#define FW_IMG_SIZE_MAX		     	0x30000	// max firmware image size 192K(include image header 256 bytes)

// firmware image area
#define NVM_FW_MAIN_OFFSET        	0

// auto test result area 
#define NV_INFO_START	        	(0x32000) // take 1 sector

#define NVM_FW_BAK_OFFSET          	(0x34000)

// config param
#define NVM_PARAM_OFFSET			(NVM_FW_BAK_OFFSET + FW_IMG_SIZE_MAX)
#define NVM_PARAM_PARTITION_NUM_MAX   3   // each param partition take 1 sector

// file system
#define NVM_FILE_SYSTEM_OFFSET	(NVM_PARAM_OFFSET + FLASH_SECTOR_SIZE * NVM_PARAM_PARTITION_NUM_MAX) // for 8 Mbits Flash
//#define NVM_FILE_SYSTEM_OFFSET 0 // debug with 4Mbits Flash,  fatfs require at least 128 sectors(4M bits)

#endif	    /* USE SPI_FLASH_Demo OR QSPI_FLASH_Demo  END*/

#ifdef __cplusplus
}
#endif

#endif /* BSPDEMO_H */

