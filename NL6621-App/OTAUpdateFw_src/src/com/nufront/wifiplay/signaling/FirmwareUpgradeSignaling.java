/**
 *固件升级信令类
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

public class FirmwareUpgradeSignaling extends CommonByte {
	/* 采集记录仪执行标准版本信令类 */
	
	private static final int signalingLength = 10;		//信令长度

	public FirmwareUpgradeSignaling(int upgradeTag, int size) {
		/* 将信令放入队列，并等待发送  */		
		super(signalingLength);
		/*String upgradeDoneTag = "exit";*/
	/*	if(upgradeTag ==1){
		     putByteArray(upgradeDoneTag.getBytes());
		}else{*/
		putShort((short)(signalingLength));			//信令总长度：字节数：2；说明：Big-endian
		putByte((byte)0x01);						//设备类型
		putByte((byte)0x20);						//信令ID
		putByte((byte)upgradeTag);					//0：开始， 1：结束
		putByte((byte)(0x00));						//备用1
		putInt((int)(size));						//升级固定的大小
		putByte((byte)(0x00));						//备用2
		putByte((byte)(0x00));						//备用3
		/*}*/
		flip();
		//bufferToArray();
		//initDataPacket();	
	}
	
	public IoBuffer getSignaling() {
		return getIoBuffer();
	}
}
