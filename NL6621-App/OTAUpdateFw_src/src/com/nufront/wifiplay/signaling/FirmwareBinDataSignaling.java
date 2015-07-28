/**
 *User Client
 *ɨ���豸������
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
	
	private static final int signalingLength = 3;		//�����

	public FirmwareBinDataSignaling(byte[] data) {
		/* �����������У����ȴ�����  */		
		super(signalingLength);
		
		putByteArray(data);										//����
		
		
		flip();
	}
	
	public IoBuffer getSignaling() {
		return getIoBuffer();
	}
}
