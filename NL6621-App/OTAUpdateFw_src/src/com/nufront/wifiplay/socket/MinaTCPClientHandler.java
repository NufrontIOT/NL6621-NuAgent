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
	 * ���յ���Ϣʱ���õķ�����Ҳ�������ڽ�����Ϣ�ķ�����һ������£�message ��һ�� IoBuffer
	 * �࣬�����ʹ����Э������������ô����ǿ��ת��Ϊ����Ҫ�����͡�ͨ������ ���ǻ�ʹ��Э���������ģ� ������������ӣ� ��ΪЭ����������
	 * TextLineCodecFactory���������ǿ���ǿ��תmessage ΪString ���͡�
	 */
	public void messageReceived(IoSession session, Object message)
			throws Exception {
		Log.w("�յ���Ϣ", "-----------��Ϣ����----");
		IoBuffer buffer = (IoBuffer) message;
		//session.getRemoteAddress();			//��ȡIP��ַ
		Log.w("Handler�յ���������Ϊ��", buffer.getHexDump());
		serviceSignalingProcess((IoBuffer) message,
				((IoBuffer) message).limit(), session);

		// TODO IoSession.write();
	}

	/*
	 * ����TCP ��˵�����ӱ��ر�ʱ��������������� ����UDP ��˵��IoSession ��close()����������ʱ�Ż�ٵ����������
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
	 * ���������һ��Session ���󱻴�����ʱ�򱻵��á�����TCP ������˵�����ӱ����ܵ�ʱ�� ���ã���Ҫע���ʱTCP
	 * ���Ӳ�δ�������˷������������溬�壬Ҳ�������ӵĶ��� IoSession ��������ϵ�ʱ�򣬻ص���������� ����UDP
	 * ��˵���������ݰ��յ���ʱ��ص������������ΪUDP �������ӵġ�
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
	 * ���������IoSession ��ͨ���������״̬ʱ���ã�����UDP Э����˵���������ʼ�ղ��ᱻ���á�
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
	 * ������������ӱ���ʱ���ã���������sessionCreated()����֮�󱻵��á�����TCP ��
	 * ˵�����������ӱ�����֮����ã������������ִ��һЩ��֤�������������ݵȡ� ����UDP
	 * ��˵�����������sessionCreated()ûʲô���𣬵��ǽ������ִ�С������ÿ
	 * ��һ��ʱ�䣬����һЩ���ݣ���ôsessionCreated()����ֻ���ڵ�һ�ε��ã����� sessionOpened()����ÿ�ζ������
	 */
	@Override
	public void sessionOpened(IoSession session) throws Exception {
		// log.debug("******************* Session Opened!");
		// TODO IoSession.write();
	}

	/*
	 * �����������ĳ���Mina ��������쳣ʱ�ص���һ�������ǹر�IoSession��
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
	 * ��������Ϣ�ɹ�ʱ�������������ע������Ĵ�ǣ����ͳɹ�֮��Ҳ����˵������Ϣ�ǲ�������������ġ�
	 */
	@Override
	public void messageSent(IoSession session, Object message) throws Exception {
		// log.debug("******************* messageSent!");
		Log.e("������Ϣ", "-----------TCP��Ϣ�ѷ���----");
		IoBuffer buffer = (IoBuffer) message;
		//session.getRemoteAddress();			//��ȡIP��ַ
		Log.e("Handler�����������ݴ�СΪ��", buffer.limit() + "");
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
			int recordLength = 30; // ÿһ��GPS��¼�ĳ���
			int recordStart = 9; // ��һ����¼��ʼ��λ��
			int count = signaling.get(8); // ��¼����
			signaling.position(0);
			String s = "Client got:  0x"
					+ Integer.toHexString(signaling.get(0))
					+ ";  SignalingCode is: 0x"
					+ Integer.toHexString(signaling.get(3))
					+ "; Packet length:" + length;

			for (int i = 0; i < count; i++) {
				s += (" ��"
						+ i
						+ "����¼,����:"
						+ Integer.toString(signaling.getInt(recordStart + i
								* recordLength + 12)) + " γ��:" + signaling
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
					Log.w("�յ�ͼƬ��", soundArray[i]);
					Message msg = new Message();
					msg.what = PHOTO;
					Bundle bundle = new Bundle();
					bundle.putString("photo", soundArray[i]);
					msg.setData(bundle);
					// ������Ϣ��Handler
					NufrontServiceActivity.nuActivity.handler.sendMessage(msg);

				} else if (Pattern.compile("([^\\s]+(\\.(?i)(wav))$)")
						.matcher(soundArray[i]).matches()) {
					Log.w("�յ�������", soundArray[i]);
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
					printMessageOnTextView("���ڲ��ŵ�" + i + "��:"
							+ playUri.toString() + "\n");
				}
			}

			releaseMediaPlayer();

		} else {

		}*/

	}

	/**
	 * �ͷ���һ��MediaPlayer��Դ
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
	 * ��HttpURLConnection�ķ�ʽ��ȡURL��ͼƬ
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
		// �ò����Ǵ��β����¿ؼ�
		
		  Message msg = new Message(); 
		  msg.what = 0; 
		  Bundle bundle = new Bundle(); 
		  bundle.putString("x", s); 
		  msg.setData(bundle); 
		  // ������Ϣ��Handler handler.sendMessage(msg);
		 
		Log.w("�յ���25�����", s);
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
