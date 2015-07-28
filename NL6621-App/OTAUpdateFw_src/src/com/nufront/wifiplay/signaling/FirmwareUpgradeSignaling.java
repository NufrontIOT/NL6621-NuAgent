/**
 *�̼�����������
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
	/* �ɼ���¼��ִ�б�׼�汾������ */
	
	private static final int signalingLength = 10;		//�����

	public FirmwareUpgradeSignaling(int upgradeTag, int size) {
		/* �����������У����ȴ�����  */		
		super(signalingLength);
		/*String upgradeDoneTag = "exit";*/
	/*	if(upgradeTag ==1){
		     putByteArray(upgradeDoneTag.getBytes());
		}else{*/
		putShort((short)(signalingLength));			//�����ܳ��ȣ��ֽ�����2��˵����Big-endian
		putByte((byte)0x01);						//�豸����
		putByte((byte)0x20);						//����ID
		putByte((byte)upgradeTag);					//0����ʼ�� 1������
		putByte((byte)(0x00));						//����1
		putInt((int)(size));						//�����̶��Ĵ�С
		putByte((byte)(0x00));						//����2
		putByte((byte)(0x00));						//����3
		/*}*/
		flip();
		//bufferToArray();
		//initDataPacket();	
	}
	
	public IoBuffer getSignaling() {
		return getIoBuffer();
	}
}
