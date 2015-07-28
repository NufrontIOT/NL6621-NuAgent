package com.nufront.wifiplay.signaling;

import org.apache.mina.core.buffer.IoBuffer;




/**
 *User Client
 *
 * 
 */

public class LoginInSignaling extends CommonByte {
	/* ��½�����࣬���ڽ���һ����½����BYTE���� */
	
	private static final int signalingLength = 16;		//0xFD01����½����

	public LoginInSignaling() {
		/* �����������У����ȴ�����  */		
		super(signalingLength);
		putByte((byte)0xff);						//��ʼ�����ֽڣ�1��˵����Service-To-Device ������Ϊ0xfe����֮Ϊ0xff
		putShort((short)signalingLength);						//���ȣ��ֽ�����2��˵����2�ֽ�SHORT�ͣ������������ݰ��ĳ��ȣ�������ʼ�ͽ�����
		putByte((byte)0xfd);						//����ţ��ֽڣ�1��˵����ǰ�������
		putByte((byte)0x01);						//����ţ��ֽڣ�1��˵����������
		//putLong(super.getDeviceID());								//�豸ID���ֽڣ�4��˵����4�ֽ�����
		putByte((byte)0x00);						//
		putByte(checkData(signalingLength - 2));	//У���룺�ֽڣ�1��˵����У��λ��ֵΪ�ӿ�ʼ����У����ǰһλ�İ�λ��򣨰�����ʼ����
		putByte((byte)0xff);						//���������ֽڣ�1��˵����Service-To-Device ������Ϊ0xfe����֮Ϊ0xff
		
		flip();
		//bufferToArray();
		//initDataPacket();	
	}
	
	public IoBuffer getSignaling() {
		return getIoBuffer();
	}
}
