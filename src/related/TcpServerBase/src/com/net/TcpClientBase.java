package com.net;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import com.google.protobuf.GeneratedMessage;
import com.google.protobuf.GeneratedMessageLite;

import YouMai.Basic.YouMaiBasic.COMMANDID;
import YouMai.Basic.YouMaiBasic.Device_Type;
import YouMai.Basic.YouMaiBasic.Heart_Beat;
import YouMai.User.YouMaiUser.User_Login;

/****************************************************************
 * @author xiesong
 * @email songtzu@126.com
 * @version 0.1
 * @time 4.19.2016
 * <p>this is the base class for tcp client.</p>
 */
public class TcpClientBase extends PduUtil{

	private String ip;
    private int port;
    private int user_id = 0;
    private static final int READ_BUFFER_SIZE = 0x100000;
    private static final int WRITE_BUFFER_SIZE = 0x100000;
    private static final int HEART_BEAT_INTERVAL = 5*60;// 3000; //心跳间隔
    private static final int MAX_HEARTBEAT = 5;// 最大心跳数（超过此心跳数判断为超时。启动重连）
    private boolean isConnected = false;
    private SocketChannel socketChannel = null;
    private int seq_num = 0;
    private ConcurrentHashMap<Integer, ReceiveListener> callbackqueue;
    private ByteBuffer readBuffer = ByteBuffer.allocateDirect(READ_BUFFER_SIZE); // 1Mb
    private ByteBuffer writeBuffer = ByteBuffer.allocateDirect(WRITE_BUFFER_SIZE); // 1Mb

    private ScheduledExecutorService heartBeatScheduled; 
    private int heartBeatCount = 0;
    private Lock sendLock = new ReentrantLock();// 锁
    private Heart_Beat heart;

    public TcpClientBase() {
		// TODO Auto-generated constructor stub
    	callbackqueue = new ConcurrentHashMap<>();
    	Heart_Beat.Builder heart_builder =  Heart_Beat.newBuilder();

		heart_builder.setUserId(0);
		heart = heart_builder.build();  
	}
    public TcpClientBase(String _ip, int _port) {
		ip= _ip;
		port = _port;
		callbackqueue = new ConcurrentHashMap<>();
	}
    

    private void HeatBeat() {
        if (!isConnected) { 
            return;
        } 
        ++heartBeatCount;
        if (heartBeatCount > MAX_HEARTBEAT) {
            // 断线，重连
            System.out.println("重连");
            Connect();
        }
        try {
             
           SendProto(heart, new ReceiveListener() {
				@Override
				public void OnRec(PduBase pduBase) {
					// TODO Auto-generated method stub
                    heartBeatCount = 0;
                    System.out.println("心跳响应");
					
				}
            });
        } catch (Exception e) {
            // TODO: handle exception
            System.out.println("心跳"); 
        }
    }
    
    private void StartHeartBeat() {
        heartBeatScheduled = Executors.newScheduledThreadPool(1);
        heartBeatScheduled.scheduleAtFixedRate(new Runnable() {
            public void run() {
                HeatBeat();
            }
        }, 5, HEART_BEAT_INTERVAL, TimeUnit.SECONDS);
    }
    
    private boolean Connect() {
        try {
            if (socketChannel == null || !socketChannel.isConnected()) {
                socketChannel = SocketChannel.open();
                socketChannel.configureBlocking(true);
                socketChannel.socket().setSendBufferSize(WRITE_BUFFER_SIZE); // 1Mb
                socketChannel.socket().setReceiveBufferSize(READ_BUFFER_SIZE); // 1Mb
                socketChannel.socket().setKeepAlive(true);
                socketChannel.socket().setReuseAddress(true);
                socketChannel.socket().setSoLinger(false, 0);
                socketChannel.socket().setSoTimeout(0);
                socketChannel.socket().setTcpNoDelay(true);
                SocketAddress socketAddress = new InetSocketAddress(ip, port);
                System.out.println(ip + port);
                socketChannel.connect(socketAddress);
                while (!socketChannel.finishConnect()) {
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }
                if (socketChannel.isConnected()) {
                    isConnected = true;
                    StartHeartBeat();
                }
            }

        } catch (IOException e) {
            // TODO Auto-generated catch block
            System.out.println("IOException" + e.toString());
            e.printStackTrace();
        }
        return isConnected;
    }
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		Connect();

		while (true) {
			try {
				recvData();
			} catch (Exception ex) {
				System.out.println("Exception run..." + ex.toString());
			}
		}
	}

	@Override
	public void OnRec(PduBase pduBase, SocketChannel channel) {
		// TODO Auto-generated method stub
		int key = pduBase.seq_id;
		System.out.println("OnRec 111111");
		// pduBase.seq_id = 0;
		callbackqueue.get(key).OnRec(pduBase);
		callbackqueue.remove(key);
	}
	
	private void recvData() {
		readBuffer.clear();// 将position设置到缓存最尾。准备读。
		try {
			while ((socketChannel.read(readBuffer)) > 0) {
				readBuffer.flip();
				while (ParsePdu(readBuffer, null) > 0) {
					System.out.println("read while loop");
				}

			}
		} catch (Exception exception) {
			System.out.println(exception.toString());
		}
	}
	
	public void SetUrl(String _ip, int _port) {
		this.port = _port ;
		ip =  _ip;
	}
	
	public int SendByte(byte[] bytes, ReceiveListener _callback ) { 
        if (socketChannel == null || !socketChannel.isConnected()) {
            System.out.println("not connected.");
            return -1;
        } 
        sendLock.lock();
        writeBuffer.put(bytes);
        // try direct write of what's in the buffer to free up space
        writeBuffer.flip();
        if (writeBuffer.remaining() > 0) {
            try {
                while (writeBuffer.hasRemaining() && (socketChannel.write(writeBuffer)) > 0)
                    ;
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        // 回调处理加入hash表
        if (_callback != null) {
            callbackqueue.put(seq_num, _callback);
            System.out.println("添加callback");
        } else {
            System.out.println("meiyou添加callback");
        }
        writeBuffer.clear();
        sendLock.unlock();
        return 0;
	}
	
	public int SendPdu(PduBase pduBase, ReceiveListener _callback) {
		byte[] bytes = SerializePdu(pduBase);
		return SendByte(bytes, _callback);
	}
	
	public int SendProto(GeneratedMessage _msg, ReceiveListener _callback){
		PduBase pduBase = new PduBase() ;
		pduBase.commandid = getActCode(_msg.getClass().getSimpleName().toUpperCase());
		pduBase.body = _msg.toByteArray();
		pduBase.length = _msg.getSerializedSize();
		pduBase.seq_id = ++seq_num;
		pduBase.terminal_token = 0 ;
		return SendPdu(pduBase, _callback);
	}
	
	public int getActCode(String _className){
//		System.out.println(_className  );
			return COMMANDID.valueOf(_className).getNumber();
	}
	
	
	

}
