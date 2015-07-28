package com.nufront.wifiplay.util;



import java.util.Date;

public class RetrySendCommand {
	private String deviceId;
	private short commandId;
	private int retryTimes;
	private Date firstSendTime;
	private byte[] bytes;
	
	
	public String getDeviceId() {
		return deviceId;
	}
	public void setDeviceId(String deviceId) {
		this.deviceId = deviceId;
	}
	public short getCommandId() {
		return commandId;
	}
	public void setCommandId(short commandId) {
		this.commandId = commandId;
	}
	public int getRetryTimes() {
		return retryTimes;
	}
	public void setRetryTimes(int retryTimes) {
		this.retryTimes = retryTimes;
	}
	public Date getFirstSendTime() {
		return firstSendTime;
	}
	public void setFirstSendTime(Date firstSendTime) {
		this.firstSendTime = firstSendTime;
	}
	public byte[] getBytes() {
		return bytes;
	}
	public void setBytes(byte[] bytes) {
		this.bytes = bytes;
	}
	
	

}
