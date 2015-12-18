#ifndef __SIMU_IIC_H
#define __SIMU_IIC_H
#include "includes.h"


#define SIMC_IIC  //ʹ��ģ��IICЭ��


#ifdef  SIMC_IIC


#define DeviceAddr 0xA0

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32������ʹ�õ���24c08�����Զ���EE_TYPEΪAT24C08
#define EE_TYPE     AT24C08


#define		IIC_SDA      GPIO_Pin_10		
#define		IIC_SCL      GPIO_Pin_9		
   	   		   
//IO��������
//IO��������
#define SDA_IN()            {BSP_GPIOSetDir(IIC_SDA,0);}
#define SDA_OUT()           {BSP_GPIOSetDir(IIC_SDA,1);}

#define SET_IIC_SDA(val)	{BSP_GPIOSetValue(IIC_SDA, val);}
#define SET_IIC_SCL(val)	{BSP_GPIOSetValue(IIC_SCL, val);}


#define  READ_SDA  GPIO_ReadInputDataBit(IIC_SDA)			

void      SIMU_I2C_Init(void);
//IIC���в�������
void      IIC_Init(void);                   //��ʼ��IIC��IO��				 
void      IIC_Start(void);				    //����IIC��ʼ�ź�
void      IIC_Stop(void);	  			    //����IICֹͣ�ź�
void      IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t   IIC_Read_Byte(uint8_t ack); //IIC��ȡһ���ֽ�
uint8_t   IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void      IIC_Ack(void);					//IIC����ACK�ź�
void      IIC_NAck(void);				    //IIC������ACK�ź�

uint8_t SimuIIC_ReadOneByte(uint16_t ReadAddr);
void    SimuIIC_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);
int     SimuIIC_data_write(unsigned int addr, unsigned int length, unsigned char * buf);
int     SimuIIC_data_read(unsigned int addr, unsigned int length, unsigned char * buf);

#endif //#ifdef  SIMC_IIC

#endif
















