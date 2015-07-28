package com.nufront.thread;

import java.net.DatagramPacket;
import java.net.DatagramSocket;

import com.nufront.util.socket.UdpClientSocket;

public class UdpReceiveThread extends Thread{
	UdpClientSocket udpclientsocket = null;
	byte[] b = null;
	public UdpReceiveThread(byte[] b) {
		// TODO Auto-generated constructor stub
		this.b = b;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub  
		try{
	        DatagramSocket server = new DatagramSocket(8787);  
	        DatagramPacket packet = new DatagramPacket(b , b.length);  
	        
	            server.receive(packet);  
	            String s = new String(packet.getData( ), 0, packet.getLength( ));  
	            System.out.println(packet.getAddress( ) + " at port "   
	                       + packet.getPort( ) + " says " + s);  
	          
		}catch(Exception e){
			
		}
	}
	
}
