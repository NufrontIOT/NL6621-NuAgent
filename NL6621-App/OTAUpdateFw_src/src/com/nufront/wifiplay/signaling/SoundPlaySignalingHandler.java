/**
 * 
 */
package com.nufront.wifiplay.signaling;


import java.io.UnsupportedEncodingException;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.session.IoSession;

import android.os.Handler;
import android.os.Message;
import android.util.Log;


import com.nufront.wifiplay.socket.MinaUDPClient;
import com.nufront.wifiplay.util.ServerCache;



/**
 * @author Administrator
 *
 */
public class SoundPlaySignalingHandler {
	

	//private SharedPreferences prefs;			//配置文件
	
	private static SoundPlaySignalingHandler instance;		//单例模式实例
	
	private final static Lock lock = new ReentrantLock();
	
	private Hashtable<Byte, CommonByte> hashtable;
	
	private boolean x02Rcvd = false;
	private boolean x03Rcvd = false;
	private boolean x04Rcvd = false;
	private boolean x05Rcvd = false;
	private boolean x20Rcvd = false;  //固件升级信令
	
	private boolean mvs0108Recvd = false;
	private boolean mvs0102Recvd = false;
	private boolean mvs0103Recvd = false;
	private boolean mvs0201Recvd = false;
	private boolean mvs0202Recvd = false;
	private int mvs0202FileNum = 0;
	private boolean mvs0203Recvd = false;
	private int mvs0203Count = 0;

	private byte mvs0102Status = -1;
	
	private boolean mvs0204Recvd = false;
	private int mvs0204Count = 0;
	private List<String> fileNameList = new ArrayList<String>();
	
	private boolean mvs0404Recvd = false;
	private int mvs0404Volume = -1;
	
	public static Handler singleHandler;
	
	public static short[] mvs0201Return = new short[0]; 

	
	public static Object[] webRadioStatus;
	
	private boolean wr70Recvd = false;
	private boolean wr71Recvd = false;
	private boolean wr72Recvd = false;
	private boolean wr73Recvd = false;
	private boolean wr74Recvd = false;
	private int wrNum = 0;
	private boolean wr75Recvd = false;

	public static Handler wrHandler;
	
	private int ap0x78Count = 0xFFFF;
	private byte mDeleteApStatus = 0x00;
	private int  mAddApStatus = 0xFFFF;
	private int  mModifyApStatus = 0x0000;
	private byte isModifyAp;
	private boolean wr7CRecvd = false;
	private boolean isPlayMusic = true;
	private byte EQType = 0;
	private static byte[][] ledParams ;
	private static byte heartStatus = (byte)0x00;
	//private static PlayStatusInfo usbPlayStatusInfo = null;
	
	int count = 0;

	int totalLength = 0;
	
	public SoundPlaySignalingHandler() {
		hashtable = new Hashtable<Byte, CommonByte>();
		

	}
	
	
	public static SoundPlaySignalingHandler getInstance() {
		if(instance == null) {
			lock.lock();
			try {
				if(instance == null) {
					instance = new SoundPlaySignalingHandler();
				}
			} finally {
				lock.unlock();
			}
		}
		
		return instance;
	}
	
	public String signalingHandle(IoBuffer signaling, int length, IoSession session) {
		
		byte type = signaling.get(2);			//获得信令代码
		if(0x01 != type) {
			return null;
		}
		
		IoBuffer buff = null;
		short signalingLen = signaling.getShort(0);		//获得数据块长度
		byte code = signaling.get(3);			//获得信令代码
		String string = "";
		String tmpString = "";
		
		byte len;
		byte status;
		
		InetSocketAddress address;
		switch (code) {
		case (byte)0xA1:	
			//通用确认包的处理
			byte ackCode = signaling.get(4);			//获得确认信令代码
			byte stereoStatus = signaling.get(5);		//音箱确认状态
			Log.w("SoundPlaySignalingHandler", "########收到A1通用确认包：" + ackCode);
			
			switch (ackCode) {
				case (byte)0x03:	//收到开始播放确认信令
					x03Rcvd = true;
					
					break;
				case (byte)0x04:	//收到停止播放确认信令
					x04Rcvd = true;
					totalLength = 0;
					
					break;
				case (byte)0x05:	//收到音量调整确认信令
					x05Rcvd = true;
					
					break;
				case (byte)0x20:   //收到固件升级确认信令
					x20Rcvd = true;
					break;
			}
			
			break;}
		
		Log.w("SoundPlaySignalingHandler", "#####收到信令来自：" + session.getRemoteAddress() + "######信令编号：" + Integer.toHexString((int)code));
		return  "#####收到信令来自：" + session.getRemoteAddress() + "######信令编号：" + Integer.toHexString((int)code);
		
	
	}
/*	private void clearGpio(IoSession session,byte regIndex,byte bitMask){
		IoBuffer buffer = new Mvsilicon0x0604Signaling(regIndex,bitMask).getSignaling();
		session.write(buffer);
	}
	private void setGpio(IoSession session,byte regIndex,byte bitMask){
		IoBuffer buffer = new Mvsilicon0x0603Signaling(regIndex,bitMask).getSignaling();
		session.write(buffer);
	}*/
	
	private String getStringFromBigEndian(byte[] bs) throws UnsupportedEncodingException{
		String re = "";
		if(bs!=null&&bs.length>0){
			for(int i=0;i<bs.length-1;i=i+2){
				byte temp = bs[i];
				if(bs.length>i+1){
					bs[i]=bs[i+1];
					bs[i+1]=temp;
				}
			}
			re = new String(bs,"UNICODE");
		}
		
		return re;
	}

	
	/**
	 * @return the x02Rcvd
	 */
	public final boolean isX02Rcvd() {
		return x02Rcvd;
	}


	/**
	 * @param x02Rcvd the x02Rcvd to set
	 */
	public final void setX02Rcvd(boolean x02Rcvd) {
		this.x02Rcvd = x02Rcvd;
	}


	/**
	 * @return the x03Rcvd
	 */
	public final boolean isX03Rcvd() {
		return x03Rcvd;
	}


	/**
	 * @param x03Rcvd the x03Rcvd to set
	 */
	public final void setX03Rcvd(boolean x03Rcvd) {
		this.x03Rcvd = x03Rcvd;
	}


	/**
	 * @return the x04Rcvd
	 */
	public final boolean isX04Rcvd() {
		return x04Rcvd;
	}


	/**
	 * @param x04Rcvd the x04Rcvd to set
	 */
	public final void setX04Rcvd(boolean x04Rcvd) {
		this.x04Rcvd = x04Rcvd;
	}


	/**
	 * @return the x05Rcvd
	 */
	public final boolean isX05Rcvd() {
		return x05Rcvd;
	}


	/**
	 * @param x05Rcvd the x05Rcvd to set
	 */
	public final void setX05Rcvd(boolean x05Rcvd) {
		this.x05Rcvd = x05Rcvd;
	}


	public boolean isMvs0201Recvd() {
		return mvs0201Recvd;
	}

	//固件升级信令
	public final void setX20Rcvd(boolean x20Rcvd){
		this.x20Rcvd = x20Rcvd;
	}

	public boolean isX20Rcvd(){
		return this.x20Rcvd;
	}
	
	public void setMvs0201Recvd(boolean mvs0201Recvd) {
		this.mvs0201Recvd = mvs0201Recvd;
	}


	public boolean isMvs0202Recvd() {
		return mvs0202Recvd;
	}


	public void setMvs0202Recvd(boolean mvs0202Recvd) {
		this.mvs0202Recvd = mvs0202Recvd;
	}


	public boolean isMvs0203Recvd() {
		return mvs0203Recvd;
	}


	public void setMvs0203Recvd(boolean mvs0203Recvd) {
		this.mvs0203Recvd = mvs0203Recvd;
	}


	public boolean isMvs0204Recvd() {
		return mvs0204Recvd;
	}


	public void setMvs0204Recvd(boolean mvs0204Recvd) {
		this.mvs0204Recvd = mvs0204Recvd;
	}


	public int getMvs0204Count() {
		return mvs0204Count;
	}


	public void setMvs0204Count(int mvs0204Count) {
		this.mvs0204Count = mvs0204Count;
	}


	


	public int getMvs0203Count() {
		return mvs0203Count;
	}


	public void setMvs0203Count(int mvs0203Count) {
		this.mvs0203Count = mvs0203Count;
	}




	public boolean isMvs0108Recvd() {
		return mvs0108Recvd;
	}


	public void setMvs0108Recvd(boolean mvs0108Recvd) {
		this.mvs0108Recvd = mvs0108Recvd;
	}


	public boolean isWr70Recvd() {
		return wr70Recvd;
	}


	public void setWr70Recvd(boolean wr70Recvd) {
		this.wr70Recvd = wr70Recvd;
	}


	public boolean isWr71Recvd() {
		return wr71Recvd;
	}


	public void setWr71Recvd(boolean wr71Recvd) {
		this.wr71Recvd = wr71Recvd;
	}


	public boolean isWr72Recvd() {
		return wr72Recvd;
	}


	public void setWr72Recvd(boolean wr72Recvd) {
		this.wr72Recvd = wr72Recvd;
	}


	public boolean isWr73Recvd() {
		return wr73Recvd;
	}


	public void setWr73Recvd(boolean wr73Recvd) {
		this.wr73Recvd = wr73Recvd;
	}


	public boolean isWr74Recvd() {
		return wr74Recvd;
	}


	public void setWr74Recvd(boolean wr74Recvd) {
		this.wr74Recvd = wr74Recvd;
	}


	public int getWrNum() {
		return wrNum;
	}


	public void setWrNum(int wrNum) {
		this.wrNum = wrNum;
	}
	public boolean isWr75Recvd() {
		return wr75Recvd;
	}
	public void setWr75Recvd(boolean wr75Recvd) {
		this.wr75Recvd = wr75Recvd;
	}
	
	public byte[][] getLedParams() {
		return ledParams;
	}
	public void setLedParams(byte[][] ledParams) {
		this.ledParams = ledParams;
	}
	public int getAp0x78Count(){
		return this.ap0x78Count;
	}
	
	public byte getDeleteApStatus(){
		return this.mDeleteApStatus;
	}
	public byte getIsModifyAp(){
		return this.isModifyAp;
	}
	public int getModifyApStatus(){
		return this.mModifyApStatus;
	}
	public int getAddApStatus(){
		return this.mAddApStatus;
	}
	public byte getMvs0102Status() {
		return mvs0102Status;
	}
	public void setMvs0102Status(byte mvs0102Status) {
		this.mvs0102Status = mvs0102Status;
	}
	public boolean getIsPlayMusic(){
		return this.isPlayMusic;
	}
	public boolean isMvs0102Recvd() {
		return mvs0102Recvd;
	}
	public void setMvs0102Recvd(boolean mvs0102Recvd) {
		this.mvs0102Recvd = mvs0102Recvd;
	}
	public int getMvs0202FileNum() {
		return mvs0202FileNum;
	}
	public void setMvs0202FileNum(int mvs0202FileNum) {
		this.mvs0202FileNum = mvs0202FileNum;
	}
	public List<String> getFileNameList() {
		return fileNameList;
	}
	public void setFileNameList(List<String> fileNameList) {
		this.fileNameList = fileNameList;
	}
	public byte getEQType(){
		return this.EQType;
	}
	public void setEQType(byte type){
		this.EQType = type;
	}
	public boolean isMvs0404Recvd() {
		return mvs0404Recvd;
	}
	public void setMvs0404Recvd(boolean mvs0404Recvd) {
		this.mvs0404Recvd = mvs0404Recvd;
	}
	public int getMvs0404Volume() {
		return mvs0404Volume;
	}
	public void setMvs0404Volume(int mvs0404Volume) {
		this.mvs0404Volume = mvs0404Volume;
	}
	public boolean isMvs0103Recvd() {
		return mvs0103Recvd;
	}
	public void setMvs0103Recvd(boolean mvs0103Recvd) {
		this.mvs0103Recvd = mvs0103Recvd;
	}	

}
