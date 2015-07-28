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
	 * 这个方法的返回值是重点： 1、当内容刚好时，返回false，告知父类接收下一批内容
	 * 2、内容不够时需要下一批发过来的内容，此时返回false，这样父类 CumulativeProtocolDecoder
	 * 会将内容放进IoSession中，等下次来数据后就自动拼装再交给本类的doDecode
	 * 3、当内容多时，返回true，因为需要再将本批数据进行读取，父类会将剩余的数据再次推送本 类的doDecode
	 */
	@Override
	public boolean doDecode(IoSession session, IoBuffer in,
			ProtocolDecoderOutput out) throws Exception {
		//断包处理。如果头不是0xFE,清空BUFFER
		if (in.remaining() >= packHeadLength) {
			int pos = in.position();
			if(in.get() != (byte)0xFE){
				in.clear();
				Log.w("------RCVD-----", "处理头部断包，清空BUFFER");
				return false;
			}
			//读取长度后重置位置
			short length = in.getShort();
			in.position(pos);

			//粘包处理
			if (in.remaining() >= length) {
				Log.w("------RCVD-----", "开始处理信令");
				byte[] bs = new byte[length];
					in.get(bs);
					IoBuffer buffer = IoBuffer.allocate(length).setAutoExpand(
							true);
					buffer.put(bs);
					buffer.flip();
			
					Log.w("收到信令内容为：", buffer.getHexDump());
					
					out.write(buffer);	//吧流交给Handler处理
					
					return true;


				} else {
					Log.w("MINA框架", "不正确的包尾");
		
				}
			return false;
			}


		return false;
	}
	

	/**
	 * 这个方法的返回值是重点： 1、当内容刚好时，返回false，告知父类接收下一批内容
	 * 2、内容不够时需要下一批发过来的内容，此时返回false，这样父类 CumulativeProtocolDecoder
	 * 会将内容放进IoSession中，等下次来数据后就自动拼装再交给本类的doDecode
	 * 3、当内容多时，返回true，因为需要再将本批数据进行读取，父类会将剩余的数据再次推送本 类的doDecode
	 */
	/*@Override
	public boolean doDecode(IoSession session, IoBuffer in,
			ProtocolDecoderOutput out) throws Exception {

		if (in.remaining() >= packHeadLength) {
			// 如果头不是0xFE,则找到0xFE为止
			int count = 0;
			//Log.w("------RCVD-----", "" + ((byte)in.get(count) != (byte)0xFE));
			while (in.get(count) != (byte)0xFE && count < in.limit()) {
				count++;
				Log.w("------RCVD-----", "处理头部断包");
			}
			if(count>0)
				in.position(count);
			

			// 信令处理开始
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

					//Log.w("收到消息", "Cucum-----------消息到达----");
					Log.w("收到信令内容为：", buffer.getHexDump());
					
					//if(CommonByte.checkData(buffer, length) == true)
					//	SignalingFactory.getSignalingFactory().getSignalingBuffer(buffer, length);
					
					//if (in.remaining() >= 3) // 如果读取内容后还粘了包，就让父类再给俺 一次，进行下一次解析
					
					out.write(buffer);	//吧流交给Handler处理
					
					//Log.w("收到消息", "Cucum-----------消息到达----");
					//Log.w("收到信令内容为：", buffer.getHexDump());
					
						return true;


				} else {
					Log.w("MINA框架", "不正确的包尾");

				}

			}

		}

		return false;
	}*/

	

}
