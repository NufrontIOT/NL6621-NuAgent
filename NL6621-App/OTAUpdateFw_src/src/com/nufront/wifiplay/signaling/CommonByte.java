package com.nufront.wifiplay.signaling;

import java.net.DatagramPacket;
import java.nio.ByteBuffer;

import org.apache.mina.core.buffer.IoBuffer;



public abstract class CommonByte{
	
	private IoBuffer buffer;
	private byte outblock[];
	private DatagramPacket outpacket;
	private int length;


	public CommonByte(){		
		outblock = new byte[512];
		buffer = IoBuffer.allocate(100).setAutoExpand(true);
		length = outblock.length;
	}
	
	
	public CommonByte(int len){
		length = len;
		outblock = new byte[length];
		buffer = IoBuffer.allocate(100).setAutoExpand(true);
	}
	
	public IoBuffer getIoBuffer() {
		return buffer;
	}
	
	public void clearIoBuffer() {
		buffer.clear();
	}
	
	public boolean putInt(String s){
		buffer.putInt(Integer.parseInt(s));
		return true;
	}
	
	public boolean putInt(int i){
		buffer.putInt(i);
		return true;
	}
	
	
	public boolean putDouble(String s){
		buffer.putDouble(Double.parseDouble(s));
		return true;
	}
	
	public boolean putLong(String s){
		buffer.putLong(Long.parseLong(s));
		return true;
	}
	
	public boolean putLong(Long l){
		buffer.putLong(l);
		return true;
	}
	
	public boolean putShort(short s){
		buffer.putShort(s);
		return true;
	}
	
	public boolean putByte(byte b){
		buffer.put(b);
		return true;
	}
	
	
	public byte checkData(int len){
		buffer.position(0);
		byte check = (byte)0x00;
		for(int i=0;i < len;i++){
			check = (byte)(check ^ buffer.get());
		}
		return check;
	}
	
	public static boolean checkData(IoBuffer signaling, int len){
		signaling.position(0);
		byte check = (byte)0x00;
		for(int i=0;i < len - 2;i++){
			check = (byte)(check ^ signaling.get());
		}
		if(signaling.get(len - 2) == check) {
			return true;
		}else {
			return false;
		}
	}
	
	public void flip(){
		buffer.flip();
	}
	
	public void bufferToArray(){
		buffer.get(outblock);
	}
	
	public byte[] getArray(){
		return outblock;
	}
	
	public void putByteArray(byte[] arr){
		buffer.put(arr);
	}
	
	public int getLength(){
		return length;
	}
	
	public void setLength(int n){
		length = n;
	}
	
	public byte getCheckData(){
		return outblock[length - 2];
	}
	
	public byte getSignalingCode(int n){
		/*	第四位为标志位，N一般为3	*/
		return outblock[n];
		
	}
	
	public void initDataPacket(){
		/*	将队列中数据放入DatagramPacket	*/
		outpacket = new DatagramPacket(outblock,outblock.length);
	}
	
	public DatagramPacket getDataPacket(){
		return outpacket;
	}
	
	public byte getArrayByte(int n){
		return outblock[n];	
	}
	
	public long getArrayLong(int n){
		return (long)outblock[n];	
	}
	
	public double getArrayDouble(int n){
		return (double)outblock[n];	
	}
	
	public short getArrayShort(int n){
		return (short)outblock[n];	
	}
	
	public int getArrayInt(int n){
		ByteBuffer buf= ByteBuffer.allocate(length + 5);
		buf.put(outblock);
		return buf.getInt(n);
	}
	
	public static byte[] receive(DatagramPacket inpacket){
		
		byte inblock[] = new byte[256];
		inpacket = new DatagramPacket(inblock,inblock.length);
//		socket.receive(inpacket);

		byte[] rec = inpacket.getData();
		System.out.println("Client got: 0x" + Integer.toHexString(rec[8])+" "+inpacket.getLength());
		return rec;
	}
	
	public abstract IoBuffer getSignaling();


}

