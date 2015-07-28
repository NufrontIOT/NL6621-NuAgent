package com.nufront.wifiplay.signaling;

import org.apache.mina.core.buffer.IoBuffer;




/**
 *User Client
 *
 * 
 */

public class LoginInSignaling extends CommonByte {
	/* 登陆信令类，用于建立一个登陆信令BYTE队列 */
	
	private static final int signalingLength = 16;		//0xFD01，登陆信令

	public LoginInSignaling() {
		/* 将信令放入队列，并等待发送  */		
		super(signalingLength);
		putByte((byte)0xff);						//开始符：字节：1；说明：Service-To-Device 的信令为0xfe，反之为0xff
		putShort((short)signalingLength);						//长度：字节数：2；说明：2字节SHORT型，整个信令数据包的长度，包括开始和结束符
		putByte((byte)0xfd);						//信令号：字节：1；说明：前置信令号
		putByte((byte)0x01);						//信令号：字节：1；说明：信令编号
		//putLong(super.getDeviceID());								//设备ID：字节：4；说明：4字节整型
		putByte((byte)0x00);						//
		putByte(checkData(signalingLength - 2));	//校验码：字节：1；说明：校验位的值为从开始符到校验码前一位的按位异或（包括开始符）
		putByte((byte)0xff);						//结束符：字节：1；说明：Service-To-Device 的信令为0xfe，反之为0xff
		
		flip();
		//bufferToArray();
		//initDataPacket();	
	}
	
	public IoBuffer getSignaling() {
		return getIoBuffer();
	}
}
