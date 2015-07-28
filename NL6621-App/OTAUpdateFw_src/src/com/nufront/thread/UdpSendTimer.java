package com.nufront.thread;

import java.util.Timer;
import java.util.TimerTask;

import com.nufront.util.socket.UdpClientSocket;

import android.util.Log;

public class UdpSendTimer extends TimerTask{
	UdpClientSocket udpclientsocket = null;
	byte[] b = null;
	public static final String TAG = "UdpSendTimer";
	public UdpSendTimer(UdpClientSocket udpclientsocket,byte[] b) {
		// TODO Auto-generated constructor stub
		this.udpclientsocket = udpclientsocket;
		this.b = b;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		try{
			udpclientsocket.send("255.255.255.255", 7878, b);
			Log.i(TAG, "udp broacast: "+b);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
}
