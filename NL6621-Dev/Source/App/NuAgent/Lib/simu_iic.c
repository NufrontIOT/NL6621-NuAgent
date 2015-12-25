#include "simu_iic.h"


#ifdef  SIMC_IIC


int SimuIIC_data_read(unsigned int addr, unsigned int length, unsigned char * buf)
{
#if  0
    int i;
    int temp = addr;
    
    for (i = 0; i < length; i++) {
        buf[i] = SimuIIC_ReadOneByte(temp++);
    }
    
    return i;
#else
    int i;
    int temp = addr; 
	 
    IIC_Start();  

	IIC_Send_Byte(DeviceAddr);	    //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(temp>>8);//发送高地址
	IIC_Wait_Ack();	   
    IIC_Send_Byte(temp%256);   //发送低地址
	IIC_Wait_Ack(); 
	IIC_Start();  
	IIC_Send_Byte(DeviceAddr|0X01);           //进入接收模式			   
	IIC_Wait_Ack();	 
	   
    for (i = 0; i < length -1; i++) {
		buf[i]=IIC_Read_Byte(1);							   
    	IIC_Wait_Ack();  
	}
	buf[++i]=IIC_Read_Byte(0);							   									  		   		    	   
    IIC_Stop();//产生一个停止条件 
    NSTusecDelay(100); 
	return i;		
#endif
}


int SimuIIC_data_write(unsigned int addr, unsigned int length, unsigned char * buf)
{
#if  0
    int i;
    int temp = addr;
    
    for (i = 0; i < length; i++) {
       SimuIIC_WriteOneByte(temp++, buf[i]);
    }
    
    return i;
#else
    int i;
    int temp = addr; 
	 
    IIC_Start();  

	IIC_Send_Byte(DeviceAddr);	    //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(temp>>8);//发送高地址
	IIC_Wait_Ack();	   
    IIC_Send_Byte(temp%256);   //发送低地址
	IIC_Wait_Ack(); 
    for (i = 0; i < length; i++) {
		IIC_Send_Byte(buf[i]);     //发送字节							   
    	IIC_Wait_Ack();  
	}										  		   		    	   
    IIC_Stop();//产生一个停止条件 
    NSTusecDelay(5000); 
	return i;	
#endif   	 
}	

//在AT24C08指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t SimuIIC_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C02){
		IIC_Send_Byte(DeviceAddr);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	 
	} else 
	    IIC_Send_Byte(DeviceAddr+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr|0X01);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

//在AT24C08指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void SimuIIC_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE > AT24C02) {
		IIC_Send_Byte(DeviceAddr);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
 	} else {
		IIC_Send_Byte(DeviceAddr+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	

	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
    NSTusecDelay(5000); 	   	 
}

//初始化IIC
void SIMU_I2C_Init(void)
{	
    GPIO_InitTypeDef  GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pin = IIC_SDA|IIC_SCL;		//管脚PIN51
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out; 	//设置输出
	GPIO_Init(&GPIO_InitStructure);	

	SET_IIC_SDA(1);				     
	SET_IIC_SCL(1);		
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	SET_IIC_SDA(1);	  	  
	SET_IIC_SCL(1);
    NSTusecDelay(20); 
	SET_IIC_SDA(0);	  //START:when CLK is high,DATA change form high to low 
    NSTusecDelay(20); 
	SET_IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	SET_IIC_SCL(0);
	SET_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    NSTusecDelay(20); 
	SET_IIC_SCL(1);
	SET_IIC_SDA(1);//发送I2C总线结束信号
    NSTusecDelay(20); 							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	SET_IIC_SDA(1);   NSTusecDelay(5); 	   
	SET_IIC_SCL(1);   NSTusecDelay(5); 	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SET_IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	SET_IIC_SCL(0);
	SDA_OUT();
	SET_IIC_SDA(0);
    NSTusecDelay(10); 
	SET_IIC_SCL(1);
    NSTusecDelay(10); 
	SET_IIC_SCL(0);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	SET_IIC_SCL(0);
	SDA_OUT();
	SET_IIC_SDA(1);
    NSTusecDelay(10); 
	SET_IIC_SCL(1);
    NSTusecDelay(10); 
	SET_IIC_SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
	SET_IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        SET_IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
        NSTusecDelay(10);    //对TEA5767这三个延时都是必须的
    	SET_IIC_SCL(1);
        NSTusecDelay(10); 
	    SET_IIC_SCL(0);	
        NSTusecDelay(10); 
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(uint8_t ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
    	SET_IIC_SCL(0);
        NSTusecDelay(10); 
	    SET_IIC_SCL(1);
        receive<<=1;
        if(READ_SDA)receive++;   
		NSTusecDelay(5); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

#endif
























