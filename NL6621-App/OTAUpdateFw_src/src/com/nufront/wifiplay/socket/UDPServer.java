package com.nufront.wifiplay.socket;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.sql.Struct;
import java.util.ArrayList;

import android.content.Context;
import android.os.Bundle;
import android.os.ConditionVariable;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class UDPServer{
	public static final String TAG = "UDPServer";
	public static final int LENGTH = 48;
	private int mPort;
	//UDP接收
	private DatagramSocket serversocket = null;
	//DatagramPacket表示存放数据的数据报
	private DatagramPacket packet = null;
	private ServerThread mServer;
	private boolean mSure = true;
	//private  ConditionVariable cv =  new  ConditionVariable();  
	private NetDataPackage datapackage = new NetDataPackage();
	private Context context = null;
	private boolean flag = false;
	private int index = 0;
	private Handler mhandler;
	private static final int MES_UDP_LOGIN = 0;
	public UDPServer(Context context,int port,Handler handler){
		this.context = context;
		mhandler = handler;
		mPort = port;
		mServer = new ServerThread();
		mServer.start();
		
	}
 //将指定byte数组以16进制的形式打印到控制台
   public void printHexString( byte[] b) {  
	 
      for (int i = 0; i < b.length; i++) { 
        String hex = Integer.toHexString(b[i] & 0xFF); 
        if (hex.length() == 1) { 
          hex = '0' + hex; 
        } 
        
        ParseString(hex.toUpperCase());
      }
     
   }
      
      
	private void ParseString(String str){
		
		if (str.toUpperCase().equals("18")) {
			index = 0;
			flag = true;
		}
		if (flag == true && index < 4) {
			datapackage.totalsize += str; 
			index++;
		}else if(index >=4 && index < 8){
			datapackage.reqNum += str;
			index++;
		}else if(index >=8 && index <10){
			datapackage.funType += str;
			index ++;
		}else if(index >=10 && index <12 ){
			datapackage.mesType += str;
			index ++;
		}else if(index >=12 && index <14 ){
			datapackage.exb += str;
			index++;
		}else if(index >=14 && index <16 ){
			datapackage.exo += str;
			index++;
		}else if(index >=16 && index <20 ){
			datapackage.exd += str;
			index++;
		}else if(index >=20 && index <24 ){
			datapackage.resd += str;
			index++;
			flag = false;
		}
		
		
		if(index == 24){
			Log.i(TAG, "datapackage.totalsize = " + datapackage.totalsize);
			Log.i(TAG, "datapackage.reqNum = " + datapackage.reqNum);
			Log.i(TAG, "datapackage.funType = " + datapackage.funType);
			Log.i(TAG, "datapackage.mesType = " + datapackage.mesType);
			Log.i(TAG, "datapackage.exb = " + datapackage.exb);
			Log.i(TAG, "datapackage.exo = " + datapackage.exo);
			Log.i(TAG, "datapackage.exd = " + datapackage.exd);
			Log.i(TAG, "datapackage.resd = " + datapackage.resd);
			CheckDataString();
			datapackage.Clear();
			index = 0;
		}
		
	}
	private void CheckDataString(){
		if(datapackage.funType.equals("0600") && datapackage.mesType.equals("0100")){
			Message msg = mhandler.obtainMessage();
			msg.what = MES_UDP_LOGIN;
			Bundle bundle = new Bundle();
			bundle.putString("IP", packet.getAddress().toString());
			msg.setData(bundle);
			mhandler.sendMessage(msg);
		
		}
	}
	public void Receive() throws IOException{
			
			byte data[] = new byte[LENGTH];
			//DatagramPacket(byte[] buf, int length):
			//用来接受长度为length的buf数据(即数据存于字节数组buf中)
			packet= new DatagramPacket(data , data.length);
			
			if (serversocket == null) {
				try {
					serversocket = new DatagramSocket(mPort);
						
				} catch (SocketException  e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					

				}
			}
			Log.i(TAG, "start receive !");
			serversocket.receive(packet);
			System.out.println("接收....................");
			
			if(packet != null){
				printHexString(data);
			
		}
			
	}
	private void Toast(String text) {
		android.widget.Toast.makeText(context, text,
				android.widget.Toast.LENGTH_SHORT).show();		
	}
	public void CloseServer(){
		if(serversocket != null){
			serversocket.close();
			serversocket = null;
		}
		if(mServer != null){
			mServer.interrupted();
			mServer = null;
		}
		mSure = false;
		
	}
	public class ServerThread extends Thread{
		public void run(){
			while (mSure) {
				if(mSure == false){
					break;
				}
				Log.i(TAG, "mSure = " + mSure);
				try {
					Receive();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
}
