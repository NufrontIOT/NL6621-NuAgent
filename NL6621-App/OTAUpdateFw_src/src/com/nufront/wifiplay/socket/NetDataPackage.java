package com.nufront.wifiplay.socket;

public class NetDataPackage {
	String totalsize;
	String reqNum;
	String funType;
	String mesType;
	String exb;
	String exo;
	String exd;
	String resd;
	String appdata;
	public NetDataPackage() {
		// TODO Auto-generated constructor stub
		totalsize = "";
		reqNum = "";
		funType = "";
		mesType = "";
		exb = "";
		exo = "";
		exd = "";
		resd = "";
		appdata = "";
	}
	
	public void Clear(){
		totalsize = "";
		reqNum = "";
		funType = "";
		mesType = "";
		exb = "";
		exo = "";
		exd = "";
		resd = "";
		appdata = "";
	}

	public String getTotalsize() {
		return totalsize;
	}

	public void setTotalsize(String totalsize) {
		this.totalsize = totalsize;
	}

	public String getReqNum() {
		return reqNum;
	}

	public void setReqNum(String reqNum) {
		this.reqNum = reqNum;
	}

	public String getFunType() {
		return funType;
	}

	public void setFunType(String funType) {
		this.funType = funType;
	}

	public String getMesType() {
		return mesType;
	}

	public void setMesType(String mesType) {
		this.mesType = mesType;
	}

	public String getExb() {
		return exb;
	}

	public void setExb(String exb) {
		this.exb = exb;
	}

	public String getExo() {
		return exo;
	}

	public void setExo(String exo) {
		this.exo = exo;
	}

	public String getExd() {
		return exd;
	}

	public void setExd(String exd) {
		this.exd = exd;
	}

	public String getResd() {
		return resd;
	}

	public void setResd(String resd) {
		this.resd = resd;
	}

	public String getAppdata() {
		return appdata;
	}

	public void setAppdata(String appdata) {
		this.appdata = appdata;
	}
	
}	
