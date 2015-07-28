package com.nufront.util.socket;
import java.io.*;
import java.net.*;

/**
 * Copyright 2007 GuangZhou Cotel Co. Ltd.
 * All right reserved.    
 * UDP�ͻ��˳������ڶԷ���˷������ݣ������շ���˵Ļ�Ӧ��Ϣ.
 * @author hzz
 * @version 1.0 
 */
public class UdpClientSocket {
    private byte[] buffer = new byte[1024];

    private DatagramSocket ds = null;

    /**
     * ���캯��������UDP�ͻ���
     * @throws Exception
     */
    public UdpClientSocket() throws Exception {
        ds = new DatagramSocket();
    }
    
    /**
     * ���ó�ʱʱ�䣬�÷���������bind����֮��ʹ��.
     * @param timeout ��ʱʱ��
     * @throws Exception
     * @author hzz
     */
    public final void setSoTimeout(final int timeout) throws Exception {
        ds.setSoTimeout(timeout);
    }

    /**
     * ��ó�ʱʱ��.
     * @return ���س�ʱʱ��
     * @throws Exception
     * @author hzz
     */
    public final int getSoTimeout() throws Exception {
        return ds.getSoTimeout();
    }

    public final DatagramSocket getSocket() {
        return ds;
    }

    /**
     * ��ָ���ķ���˷���������Ϣ.
     * @param host ������������ַ
     * @param port ����˶˿�
     * @param bytes ���͵�������Ϣ
     * @return ���ع��������ݱ�
     * @throws IOException
     * @author hzz
     */
    public final DatagramPacket send(final String host, final int port,
            final byte[] bytes) throws IOException {
        DatagramPacket dp = new DatagramPacket(bytes, bytes.length, InetAddress
                .getByName(host), port);
        ds.send(dp);
        return dp;
    }

    /**
     * ���մ�ָ���ķ���˷��ص�����.
     * @param lhost ���������
     * @param lport ����˶˿�
     * @return ���ش�ָ���ķ���˷��ص�����.
     * @throws Exception
     * @author hzz
     */
    public final String receive(final String lhost, final int lport)
            throws Exception {
        DatagramPacket dp = new DatagramPacket(buffer, buffer.length);
        ds.receive(dp);
        String info = new String(dp.getData(), 0, dp.getLength());
        return info;
    }

    /**
     * �ر�udp����.
     * @author hzz
     * 
     */
    public final void close() {
        try {
            ds.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

   
}