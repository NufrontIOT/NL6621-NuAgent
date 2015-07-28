/**
 *User Client
 *扫描设备信令类
 * 
 */
package com.nufront.wifiplay.signaling;

import java.io.UnsupportedEncodingException;

import org.apache.mina.core.buffer.IoBuffer;




/**
 *User Client
 *
 * 
 */

public class FirmwareBinDataSignaling extends CommonByte {
	
	private static final int signalingLength = 3;		//信令长度

	public FirmwareBinDataSignaling(byte[] data) {
		/* 将信令放入队列，并等待发送  */		
		super(signalingLength);
		
		putByteArray(data);										//数据
		
		
		flip();
	}
	
	public IoBuffer getSignaling() {
		return getIoBuffer();
	}
}
