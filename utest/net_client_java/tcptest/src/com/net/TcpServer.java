package com.net;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Date;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;


public abstract class TcpServer extends PduUtil{
	// 缓冲区大小
	private static final int BufferSize = 10240;
	// 超时时间，单位毫秒
	private static final int TimeOut = 3000;
	// 本地监听端口
	private int ListenPort ;
	
	private ServerSocketChannel listenerChannel ;
	private Selector selector;
	private ConcurrentHashMap <Integer /*user_token*/, SocketChannel > user_map_ ;
	private ConcurrentHashMap <SocketAddress , ByteBuffer > recv_buffers  = new ConcurrentHashMap<>();;
	public  void Connect(String _ip,int _port)   {
//		recv_buffers
		ListenPort = _port ;
		// 创建选择器
		try {
			selector = Selector.open();
			// 打开监听信道
			listenerChannel = ServerSocketChannel.open();
			// 与本地端口绑定
			listenerChannel.socket().bind(new InetSocketAddress(_ip,ListenPort));
			// 设置为非阻塞模式
			listenerChannel.configureBlocking(false);
			// 将选择器绑定到监听信道,只有非阻塞信道才可以注册选择器.并在注册过程中指出该信道可以进行Accept操作
			listenerChannel.register(selector, SelectionKey.OP_ACCEPT);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("TCPSERVER ERROR" + e.toString());
		}
		
	}
	
	
	public void SendPdu(PduBase pduBase, SocketChannel channel) {
		byte[] bytes = SerializePdu(pduBase);
		ByteBuffer buffer = ByteBuffer.wrap( bytes );  
		try {
			channel.write( buffer );
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}  
	}


	@Override
	public void run() {
		// TODO Auto-generated method stub
		// 反复循环,等待IO
		
		while (true) {
			// 等待某信道就绪(或超时)
			try {
				if (selector.select(TimeOut) == 0) {// 监听注册通道，当其中有注册的 IO
													// 操作可以进行时，该函数返回，并将对应的
													// SelectionKey 加入 selected-key
													// set
//					System.out.print("wait\n");
					continue;
				}
				
				// 取得迭代器.selectedKeys()中包含了每个准备好某一I/O操作的信道的SelectionKey
				// Selected-key Iterator 代表了所有通过 select() 方法监测到可以进行 IO 操作的 channel
				// ，这个集合可以通过 selectedKeys() 拿到
				Iterator<SelectionKey> keyIter = selector.selectedKeys().iterator();
				while (keyIter.hasNext()) {
//					System.out.println("keyIter.hasNext()\n");
					SelectionKey key = keyIter.next();
					keyIter.remove();
//					try {
						if (key.isAcceptable()) {
							// 有客户端连接请求时
							// 返回创建此键的通道，接受客户端建立连接的请求，并返回 SocketChannel 对象  
					        SocketChannel clientChannel = ((ServerSocketChannel) key.channel())  
					                .accept();  
					        // 非阻塞式  
					        clientChannel.configureBlocking(false);  
					        // 注册到selector  
//					        clientChannel.register(key.selector(), SelectionKey.OP_READ,  
//					                ByteBuffer.allocate(BufferSize));  
					        clientChannel.register(key.selector(), SelectionKey.OP_READ);
						}
						if (key.isReadable()) {// 判断是否有数据发送过来
							
							// 从客户端读取数据
							// 获得与客户端通信的信道  
					        SocketChannel clientChannel = (SocketChannel) key.channel();
					        // 得到并清空缓冲区  
					        ByteBuffer total_buffer = recv_buffers.get(clientChannel.getRemoteAddress());
					        if (total_buffer==null) {
					        	total_buffer = ByteBuffer.allocate(BufferSize);
							}
					        recv_buffers.put(clientChannel.getRemoteAddress(), total_buffer);
//					        ByteBuffer buffer = (ByteBuffer) key.attachment();  
//					        buffer.clear();  
					        // 读取信息获得读取的字节数  
					        int bytesRead =0;
					        try{
					        	bytesRead = clientChannel.read(total_buffer);
					        }catch(Exception exception){
					        	bytesRead = -1;
//					        	System.out.println(bytesRead);
					        }
					        if (bytesRead == -1) {  
					            // 没有读取到内容的情况  
					            clientChannel.close();
					            key.cancel();
					            recv_buffers.remove(clientChannel);
//					            System.out.println("-1-1--1-1-1-1-1-1\n");
//					            keyIter.remove();
					        } else {  
					            // 将缓冲区准备为数据传出状态  
					        	total_buffer.flip();
					            int readResult = 0 ;
					            while ( (readResult = ParsePdu(total_buffer, clientChannel) ) >0) {
									//loop parse.
								}
					            key.interestOps(SelectionKey.OP_READ | SelectionKey.OP_WRITE);  
					            //判断起始标记
					        }
						}
						if (key.isValid() && key.isWritable()) {// 判断是否有效及可以发送给客户端
							// 客户端可写时
							//protocol.handleWrite(key);
						}
						 
//					} catch (IOException ex) {
//						// 出现IO异常（如客户端断开连接）时移除处理过的键
//						System.out.println("IOException ex\n");
//						System.out.println("length is " +length +"\n");
////						keyIter.remove();
////						continue;
//					}
					// 移除处理过的键
//					System.out.println("remove processed key");
//					keyIter.remove();
				}
//				System.out.println("loop 访问");
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				System.out.println("e.printStackTrace" + e.toString() + "\n");
				e.printStackTrace();
			}
		
		}
	}

}


 

