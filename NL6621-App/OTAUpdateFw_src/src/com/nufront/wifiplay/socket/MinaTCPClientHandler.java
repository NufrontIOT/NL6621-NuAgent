/*==============================================================================
            Copyright (c) 2004-2012 Nufront Incorporated.
            All Rights Reserved.
            Nufront Confidential and Proprietary
==============================================================================*/
package com.nufront.wifiplay.socket;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.transport.socket.SocketSessionConfig;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.nufront.wifiplay.signaling.SoundPlaySignalingHandler;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;




/**
 * @author Nufront
 * 
 */
public class MinaTCPClientHandler extends IoHandlerAdapter {

	private final static Logger log = LoggerFactory
			.getLogger(MinaTCPClientHandler.class);
	
	//private static MediaPlayer mediaPlayer;
		

	
	public MinaTCPClientHandler() {
		super();
		//signalingFactory = new SignalingFactory();
	}

	/*
	 * 接收到消息时调用的方法，也就是用于接收消息的方法，一般情况下，message 是一个 IoBuffer
	 * 类，如果你使用了协议编解码器，那么可以强制转换为你需要的类型。通常我们 都是会使用协议编解码器的， 就像上面的例子， 因为协议编解码器是
	 * TextLineCodecFactory，所以我们可以强制转message 为String 类型。
	 */
	public void messageReceived(IoSession session, Object message)
			throws Exception {
		Log.w("收到消息", "-----------消息到达----");
		IoBuffer buffer = (IoBuffer) message;
		//session.getRemoteAddress();			//获取IP地址
		Log.w("Handler收到信令内容为：", buffer.getHexDump());
		serviceSignalingProcess((IoBuffer) message,
				((IoBuffer) message).limit(), session);

		// TODO IoSession.write();
	}

	/*
	 * 对于TCP 来说，连接被关闭时，调用这个方法。 对于UDP 来说，IoSession 的close()方法被调用时才会毁掉这个方法。
	 */
	@Override
	public void sessionClosed(IoSession session) throws Exception {
		//log.debug("******************* Session Closed!");
		Log.w("Session Closed!", "Session Closed!");
		session.close(true);
		MinaTCPClient.getInstance().setTcpOnLine(false);
	/*	if(MusicControl.getInstance().isPlayFlag()||MusicMvsiliconControl.getInstance().isPlayFlag()) {
			MinaTCPClient.getInstance().reconnect();
		}*/
		/*if(MusicMvsiliconControl.getInstance().isPlayFlag()) {
			MinaTCPClient.getInstance().reconnect();
		}*/
	}

	/*
	 * 这个方法当一个Session 对象被创建的时候被调用。对于TCP 连接来说，连接被接受的时候 调用，但要注意此时TCP
	 * 连接并未建立，此方法仅代表字面含义，也就是连接的对象 IoSession 被创建完毕的时候，回调这个方法。 对于UDP
	 * 来说，当有数据包收到的时候回调这个方法，因为UDP 是无连接的。
	 */
	@Override
	public void sessionCreated(IoSession session) throws Exception {
		// log.debug("******************* Session Created!");
		SocketSessionConfig config = (SocketSessionConfig) session.getConfig();
		log.info("sessionCreated, Current receive buffer :"+ config.getReceiveBufferSize() + ", set to 64k.");
		config.setReceiveBufferSize(16 * 1024);
		//config.setReuseAddress(true);
		session.getConfig().setIdleTime(IdleStatus.WRITER_IDLE, 30);	//30

	}

	/*
	 * 这个方法在IoSession 的通道进入空闲状态时调用，对于UDP 协议来说，这个方法始终不会被调用。
	 */
	@Override
	public void sessionIdle(IoSession session, IdleStatus status)
			throws Exception {
		// log.debug(session + "******************* Session Idle!");
		if (status == IdleStatus.WRITER_IDLE) {
			log.warn("session idled 30 seconds.");
			session.close(true);
		}

	}

	/*
	 * 这个方法在连接被打开时调用，它总是在sessionCreated()方法之后被调用。对于TCP 来
	 * 说，它是在连接被建立之后调用，你可以在这里执行一些认证操作、发送数据等。 对于UDP
	 * 来说，这个方法与sessionCreated()没什么区别，但是紧跟其后执行。如果你每
	 * 隔一段时间，发送一些数据，那么sessionCreated()方法只会在第一次调用，但是 sessionOpened()方法每次都会调用
	 */
	@Override
	public void sessionOpened(IoSession session) throws Exception {
		// log.debug("******************* Session Opened!");
		// TODO IoSession.write();
	}

	/*
	 * 这个方法在你的程序、Mina 自身出现异常时回调，一般这里是关闭IoSession。
	 */
	@Override
	public void exceptionCaught(IoSession session, Throwable cause)
			throws Exception {
		log.error(cause.getMessage(),cause);
		if (!session.isClosing())
			log.warn("session exceptionCaught.");
			session.close(false);
	}

	/*
	 * 当发送消息成功时调用这个方法，注意这里的措辞，发送成功之后，也就是说发送消息是不能用这个方法的。
	 */
	@Override
	public void messageSent(IoSession session, Object message) throws Exception {
		// log.debug("******************* messageSent!");
		Log.e("发出消息", "-----------TCP消息已发送----");
		IoBuffer buffer = (IoBuffer) message;
		//session.getRemoteAddress();			//获取IP地址
		Log.e("Handler发送信令内容大小为：", buffer.limit() + "");
	}

	public void serviceSignalingProcess(IoBuffer signaling, int length, IoSession session) {
		//if(CommonByte.checkData(signaling, length) == true)
		SoundPlaySignalingHandler.getInstance().signalingHandle(signaling, length, session);
		
		/*if (0x01 == signaling.get(3)) {
			printMessageOnTextView("Client got:  0x"
					+ Integer.toHexString(signaling.get(0))
					+ ";  SignalingCode is: 0x"
					+ Integer.toHexString(signaling.get(3))
					+ "; Packet length:" + length + ";  Receive success: 0x"
					+ Integer.toHexString(signaling.get(length - 3)) + "\n");
		} else if (0x04 == signaling.get(3)) {

		} else if (0x10 == signaling.get(3)) {

		} else if (0x11 == signaling.get(3)) {

		} else if (0x13 == signaling.get(3)) {
			int recordLength = 30; // 每一条GPS记录的长度
			int recordStart = 9; // 第一条记录开始的位置
			int count = signaling.get(8); // 记录条数
			signaling.position(0);
			String s = "Client got:  0x"
					+ Integer.toHexString(signaling.get(0))
					+ ";  SignalingCode is: 0x"
					+ Integer.toHexString(signaling.get(3))
					+ "; Packet length:" + length;

			for (int i = 0; i < count; i++) {
				s += (" 第"
						+ i
						+ "条记录,经度:"
						+ Integer.toString(signaling.getInt(recordStart + i
								* recordLength + 12)) + " 纬度:" + signaling
						.getInt(recordStart + i * recordLength + 16));
			}
			
			printMessageOnTextView(s + "\n");

		} else if (0x25 == signaling.get(3)) {
			String s = new String(signaling.array(), 8, length - 10);


			String[] soundArray = s.split("\\,");

			String readySend = "Client got:  0x"
					+ Integer.toHexString(signaling.get(0))
					+ ";  SignalingCode is: 0x"
					+ Integer.toHexString(signaling.get(3))
					+ "; Packet length:" + length;

			for (int i = 0; i < soundArray.length; i++) {
				readySend = readySend + ";  Text Content: " + soundArray[i];

			}

			printMessageOnTextView(readySend + "\n");

			for (int i = 0; i < soundArray.length; i++) {

				if (Pattern.compile("([^\\s]+(\\.(?i)(jpg|png|gif|bmp))$)")
						.matcher(soundArray[i]).matches()) {
					Log.w("收到图片：", soundArray[i]);
					Message msg = new Message();
					msg.what = PHOTO;
					Bundle bundle = new Bundle();
					bundle.putString("photo", soundArray[i]);
					msg.setData(bundle);
					// 发送消息到Handler
					NufrontServiceActivity.nuActivity.handler.sendMessage(msg);

				} else if (Pattern.compile("([^\\s]+(\\.(?i)(wav))$)")
						.matcher(soundArray[i]).matches()) {
					Log.w("收到声音：", soundArray[i]);
					Uri playUri = Uri.parse(soundArray[i]);
					try {
						MediaPlayer mediaPlayer = new MediaPlayer();
						mediaPlayer.setDataSource(playUri.toString());
						mediaPlayer.prepare();
						mediaPlayer.start();
					} catch (IllegalArgumentException e) {
						printMessageOnTextView(e.toString());
					} catch (IOException e) {
						printMessageOnTextView(e.toString());
					}
					printMessageOnTextView("正在播放第" + i + "首:"
							+ playUri.toString() + "\n");
				}
			}

			releaseMediaPlayer();

		} else {

		}*/

	}

	/**
	 * 释放上一次MediaPlayer资源
	 */
	public static void releaseMediaPlayer() {
/*		if (mediaPlayer != null) {
			if (mediaPlayer.isPlaying()) {
				mediaPlayer.stop();
			}
			mediaPlayer.release();
			mediaPlayer = null;
		}*/
	}

	/**
	 * 以HttpURLConnection的方式获取URL的图片
	 * 
	 * @param url
	 * @return Bitmap
	 */
	public static Bitmap getBitmapFromUrl(String url) {

		Bitmap bitmap = null;
		URL BitmapUrl = null;

		try {
			BitmapUrl = new URL(url);
		} catch (MalformedURLException e1) {
			e1.printStackTrace();
		}

		try {
			HttpURLConnection conn = (HttpURLConnection) BitmapUrl
					.openConnection();
			//conn.setConnectTimeout(6 * 1000);
			conn.connect();
			InputStream is = conn.getInputStream();
			bitmap = BitmapFactory.decodeStream(is);
			is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return bitmap;
	}

	public static void printMessageOnTextView(String s) {
		// 该部分是传参并更新控件
		
		  Message msg = new Message(); 
		  msg.what = 0; 
		  Bundle bundle = new Bundle(); 
		  bundle.putString("x", s); 
		  msg.setData(bundle); 
		  // 发送消息到Handler handler.sendMessage(msg);
		 
		Log.w("收到第25条信令：", s);
	}
	
/*	public static void tackSelfMarkerOnMap(MapMarker mapMarker)
	{
		Message msg = new Message();
		msg.what = MinaUDPClientHandler.SELF_MARKER;
		Bundle bundle = new Bundle(); 
		bundle.putSerializable(MinaUDPClientHandler.MARKER_STRING, mapMarker);
		msg.setData(bundle);
		StarwayServiceActivity.getInstance().handler.sendMessage(msg);
	
	}*/

}
