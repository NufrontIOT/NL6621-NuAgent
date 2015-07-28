/**
 * 
 */
package com.nufront.wifiplay.socket;

import java.nio.charset.Charset;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.CumulativeProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolDecoderOutput;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import android.util.Log;

/**
 * @author Administrator
 * 
 */
public class TCPMessageDecoder extends CumulativeProtocolDecoder {

	private static Logger LOG = LoggerFactory
			.getLogger(TCPMessageDecoder.class);
	private final Charset charset;
	private final int packHeadLength = 3;

	/**
	 * 
	 */
	public TCPMessageDecoder(Charset charset) {
		this.charset = charset;
		// TODO Auto-generated constructor stub
	}
	
	/**
	 * ��������ķ���ֵ���ص㣺 1�������ݸպ�ʱ������false����֪���������һ������
	 * 2�����ݲ���ʱ��Ҫ��һ�������������ݣ���ʱ����false���������� CumulativeProtocolDecoder
	 * �Ὣ���ݷŽ�IoSession�У����´������ݺ���Զ�ƴװ�ٽ��������doDecode
	 * 3�������ݶ�ʱ������true����Ϊ��Ҫ�ٽ��������ݽ��ж�ȡ������Ὣʣ��������ٴ����ͱ� ���doDecode
	 */
	@Override
	public boolean doDecode(IoSession session, IoBuffer in,
			ProtocolDecoderOutput out) throws Exception {
		//�ϰ��������ͷ����0xFE,���BUFFER
		if (in.remaining() >= packHeadLength) {
			int pos = in.position();
			if(in.get() != (byte)0xFE){
				in.clear();
				Log.w("------RCVD-----", "����ͷ���ϰ������BUFFER");
				return false;
			}
			//��ȡ���Ⱥ�����λ��
			short length = in.getShort();
			in.position(pos);

			//ճ������
			if (in.remaining() >= length) {
				Log.w("------RCVD-----", "��ʼ��������");
				byte[] bs = new byte[length];
					in.get(bs);
					IoBuffer buffer = IoBuffer.allocate(length).setAutoExpand(
							true);
					buffer.put(bs);
					buffer.flip();
			
					Log.w("�յ���������Ϊ��", buffer.getHexDump());
					
					out.write(buffer);	//��������Handler����
					
					return true;


				} else {
					Log.w("MINA���", "����ȷ�İ�β");
		
				}
			return false;
			}


		return false;
	}
	

	/**
	 * ��������ķ���ֵ���ص㣺 1�������ݸպ�ʱ������false����֪���������һ������
	 * 2�����ݲ���ʱ��Ҫ��һ�������������ݣ���ʱ����false���������� CumulativeProtocolDecoder
	 * �Ὣ���ݷŽ�IoSession�У����´������ݺ���Զ�ƴװ�ٽ��������doDecode
	 * 3�������ݶ�ʱ������true����Ϊ��Ҫ�ٽ��������ݽ��ж�ȡ������Ὣʣ��������ٴ����ͱ� ���doDecode
	 */
	/*@Override
	public boolean doDecode(IoSession session, IoBuffer in,
			ProtocolDecoderOutput out) throws Exception {

		if (in.remaining() >= packHeadLength) {
			// ���ͷ����0xFE,���ҵ�0xFEΪֹ
			int count = 0;
			//Log.w("------RCVD-----", "" + ((byte)in.get(count) != (byte)0xFE));
			while (in.get(count) != (byte)0xFE && count < in.limit()) {
				count++;
				Log.w("------RCVD-----", "����ͷ���ϰ�");
			}
			if(count>0)
				in.position(count);
			

			// �����ʼ
			short length = in.getShort(count + 1);
			if (in.remaining() >= length) {
				byte tail = in.get(count + length - 1);
				Log.w("------RCVD-----tail", "" + tail);
				if ((byte)0xFE == tail) {
					byte[] bs = new byte[length];
					in.get(bs);
					IoBuffer buffer = IoBuffer.allocate(length).setAutoExpand(
							true);
					buffer.put(bs);
					buffer.flip();

					//Log.w("�յ���Ϣ", "Cucum-----------��Ϣ����----");
					Log.w("�յ���������Ϊ��", buffer.getHexDump());
					
					//if(CommonByte.checkData(buffer, length) == true)
					//	SignalingFactory.getSignalingFactory().getSignalingBuffer(buffer, length);
					
					//if (in.remaining() >= 3) // �����ȡ���ݺ�ճ�˰������ø����ٸ��� һ�Σ�������һ�ν���
					
					out.write(buffer);	//��������Handler����
					
					//Log.w("�յ���Ϣ", "Cucum-----------��Ϣ����----");
					//Log.w("�յ���������Ϊ��", buffer.getHexDump());
					
						return true;


				} else {
					Log.w("MINA���", "����ȷ�İ�β");

				}

			}

		}

		return false;
	}*/

	

}
