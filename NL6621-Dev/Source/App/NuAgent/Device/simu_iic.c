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

	IIC_Send_Byte(DeviceAddr);	    //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(temp>>8);//���͸ߵ�ַ
	IIC_Wait_Ack();	   
    IIC_Send_Byte(temp%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 
	IIC_Start();  
	IIC_Send_Byte(DeviceAddr|0X01);           //�������ģʽ			   
	IIC_Wait_Ack();	 
	   
    for (i = 0; i < length -1; i++) {
		buf[i]=IIC_Read_Byte(1);							   
    	IIC_Wait_Ack();  
	}
	buf[++i]=IIC_Read_Byte(0);							   									  		   		    	   
    IIC_Stop();//����һ��ֹͣ���� 
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

	IIC_Send_Byte(DeviceAddr);	    //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(temp>>8);//���͸ߵ�ַ
	IIC_Wait_Ack();	   
    IIC_Send_Byte(temp%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 
    for (i = 0; i < length; i++) {
		IIC_Send_Byte(buf[i]);     //�����ֽ�							   
    	IIC_Wait_Ack();  
	}										  		   		    	   
    IIC_Stop();//����һ��ֹͣ���� 
    NSTusecDelay(5000); 
	return i;	
#endif   	 
}	

//��AT24C08ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
uint8_t SimuIIC_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C02){
		IIC_Send_Byte(DeviceAddr);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	 
	} else 
	    IIC_Send_Byte(DeviceAddr+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr|0X01);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

//��AT24C08ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void SimuIIC_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE > AT24C02) {
		IIC_Send_Byte(DeviceAddr);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
 	} else {
		IIC_Send_Byte(DeviceAddr+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	

	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
    NSTusecDelay(5000); 	   	 
}

//��ʼ��IIC
void SIMU_I2C_Init(void)
{	
    GPIO_InitTypeDef  GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pin = IIC_SDA|IIC_SCL;		//�ܽ�PIN51
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out; 	//�������
	GPIO_Init(&GPIO_InitStructure);	

	SET_IIC_SDA(1);				     
	SET_IIC_SCL(1);		
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	SET_IIC_SDA(1);	  	  
	SET_IIC_SCL(1);
    NSTusecDelay(20); 
	SET_IIC_SDA(0);	  //START:when CLK is high,DATA change form high to low 
    NSTusecDelay(20); 
	SET_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	SET_IIC_SCL(0);
	SET_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    NSTusecDelay(20); 
	SET_IIC_SCL(1);
	SET_IIC_SDA(1);//����I2C���߽����ź�
    NSTusecDelay(20); 							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	SET_IIC_SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
	SET_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        SET_IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
        NSTusecDelay(10);    //��TEA5767��������ʱ���Ǳ����
    	SET_IIC_SCL(1);
        NSTusecDelay(10); 
	    SET_IIC_SCL(0);	
        NSTusecDelay(10); 
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(uint8_t ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

#endif
























