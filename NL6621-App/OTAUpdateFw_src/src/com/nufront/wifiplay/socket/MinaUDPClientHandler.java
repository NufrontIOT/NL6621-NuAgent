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
public class MinaUDPClientHandler extends IoHandlerAdapter {
	
	public static final int MUSIC_CONTROL_STOP = 101;
	public static final int MUSIC_CONTROL_START = 102;
	public static final int NEIGHBOUR_REMOVE = 201;
	
	public static final String MUSIC_CONTROL_STOP_STRING = "MUSIC_CONTROL_STOP";
	public static final String NEIGHBOUR_REMOVE_STRING = "NEIGHBOUR_REMOVE";
	

	/*
	 * ���յ���Ϣʱ���õķ�����Ҳ�������ڽ�����Ϣ�ķ�����һ������£�message ��һ�� IoBuffer
	 * �࣬�����ʹ����Э������������ô����ǿ��ת��Ϊ����Ҫ�����͡�ͨ������ ���ǻ�ʹ��Э���������ģ� ������������ӣ� ��ΪЭ����������
	 * TextLineCodecFactory���������ǿ���ǿ��תmessage ΪString ���͡�
	 */
	public void messageReceived(IoSession session, Object message)
			throws Exception {
		Log.w("�յ���Ϣ", "-----------��Ϣ����----");
		IoBuffer buffer = (IoBuffer) message;
		
		Log.w("Handler�յ���������Ϊ��", buffer.getHexDump());
		serviceSignalingProcess((IoBuffer) message,
				((IoBuffer) message).limit(), session);

	}


	@Override
	public void sessionClosed(IoSession session) throws Exception {
		//Log.w("Session Closed!", "Session Closed!");
	}

	/*
	 * ���������һ��Session ���󱻴�����ʱ�򱻵��á�����TCP ������˵�����ӱ����ܵ�ʱ�� ���ã���Ҫע���ʱTCP
	 * ���Ӳ�δ�������˷������������溬�壬Ҳ�������ӵĶ��� IoSession ��������ϵ�ʱ�򣬻ص���������� ����UDP
	 * ��˵���������ݰ��յ���ʱ��ص������������ΪUDP �������ӵġ�
	 */
	@Override
	public void sessionCreated(IoSession session) throws Exception {
		// log.debug("******************* Session Created!");
		//SocketSessionConfig config = (SocketSessionConfig) session.getConfig();
		//log.info("sessionCreated, Current receive buffer :"+ config.getReceiveBufferSize() + ", set to 64k.");
		//config.setReceiveBufferSize(16 * 1024);
		//config.setReuseAddress(true);
		//session.getConfig().setIdleTime(IdleStatus.READER_IDLE, 30);	//30

	}

	/*
	 * ���������IoSession ��ͨ���������״̬ʱ���ã�����UDP Э����˵���������ʼ�ղ��ᱻ���á�
	 */
	@Override
	public void sessionIdle(IoSession session, IdleStatus status)
			throws Exception {
		// log.debug(session + "******************* Session Idle!");
		if (status == IdleStatus.READER_IDLE) {
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
		//log.error(cause.getMessage(),cause);
		/*if (!session.isClosing())
			session.close(false);*/
	}

	/*
	 * ��������Ϣ�ɹ�ʱ�������������ע������Ĵ�ǣ����ͳɹ�֮��Ҳ����˵������Ϣ�ǲ�������������ġ�
	 */
	@Override
	public void messageSent(IoSession session, Object message) throws Exception {
		// log.debug("******************* messageSent!");
		//Log.w("������Ϣ", "-----------��Ϣ�ѷ���----");
		//IoBuffer buffer = (IoBuffer) message;
		//Log.w("Handler���͵���������Ϊ��", buffer.getHexDump());
		//Log.w("MinaUDPClientHandler", "����" + buffer.get(3) + "������");
	}

	public void serviceSignalingProcess(IoBuffer signaling, int length, IoSession session) {
	
		SoundPlaySignalingHandler.getInstance().signalingHandle(signaling, length, session);

	}

	/**
	 * �ͷ���һ��MediaPlayer��Դ
	 */

	public static void releaseMediaPlayer() {

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
	

}
