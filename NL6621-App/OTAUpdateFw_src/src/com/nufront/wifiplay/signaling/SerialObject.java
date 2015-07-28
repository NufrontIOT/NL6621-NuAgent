/**
 * 
 */
package com.nufront.wifiplay.signaling;

import java.io.Serializable;

/**
 * @author Administrator
 *
 */
public class SerialObject implements Serializable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -5500713429104222443L;
	
	private Object serialObject;
	
	public SerialObject(Object o) {
		serialObject = o;
	}

	/**
	 * @return the serialObject
	 */
	public final Object getSerialObject() {
		return serialObject;
	}

	/**
	 * @param serialObject the serialObject to set
	 */
	public final void setSerialObject(Object serialObject) {
		this.serialObject = serialObject;
	}
	
	
	
}
