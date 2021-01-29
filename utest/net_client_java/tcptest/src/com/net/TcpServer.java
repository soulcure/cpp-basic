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
	// ��������С
	private static final int BufferSize = 10240;
	// ��ʱʱ�䣬��λ����
	private static final int TimeOut = 3000;
	// ���ؼ����˿�
	private int ListenPort ;
	
	private ServerSocketChannel listenerChannel ;
	private Selector selector;
	private ConcurrentHashMap <Integer /*user_token*/, SocketChannel > user_map_ ;
	private ConcurrentHashMap <SocketAddress , ByteBuffer > recv_buffers  = new ConcurrentHashMap<>();;
	public  void Connect(String _ip,int _port)   {
//		recv_buffers
		ListenPort = _port ;
		// ����ѡ����
		try {
			selector = Selector.open();
			// �򿪼����ŵ�
			listenerChannel = ServerSocketChannel.open();
			// �뱾�ض˿ڰ�
			listenerChannel.socket().bind(new InetSocketAddress(_ip,ListenPort));
			// ����Ϊ������ģʽ
			listenerChannel.configureBlocking(false);
			// ��ѡ�����󶨵������ŵ�,ֻ�з������ŵ��ſ���ע��ѡ����.����ע�������ָ�����ŵ����Խ���Accept����
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
		// ����ѭ��,�ȴ�IO
		
		while (true) {
			// �ȴ�ĳ�ŵ�����(��ʱ)
			try {
				if (selector.select(TimeOut) == 0) {// ����ע��ͨ������������ע��� IO
													// �������Խ���ʱ���ú������أ�������Ӧ��
													// SelectionKey ���� selected-key
													// set
//					System.out.print("wait\n");
					continue;
				}
				
				// ȡ�õ�����.selectedKeys()�а�����ÿ��׼����ĳһI/O�������ŵ���SelectionKey
				// Selected-key Iterator ����������ͨ�� select() ������⵽���Խ��� IO ������ channel
				// ��������Ͽ���ͨ�� selectedKeys() �õ�
				Iterator<SelectionKey> keyIter = selector.selectedKeys().iterator();
				while (keyIter.hasNext()) {
//					System.out.println("keyIter.hasNext()\n");
					SelectionKey key = keyIter.next();
					keyIter.remove();
//					try {
						if (key.isAcceptable()) {
							// �пͻ�����������ʱ
							// ���ش����˼���ͨ�������ܿͻ��˽������ӵ����󣬲����� SocketChannel ����  
					        SocketChannel clientChannel = ((ServerSocketChannel) key.channel())  
					                .accept();  
					        // ������ʽ  
					        clientChannel.configureBlocking(false);  
					        // ע�ᵽselector  
//					        clientChannel.register(key.selector(), SelectionKey.OP_READ,  
//					                ByteBuffer.allocate(BufferSize));  
					        clientChannel.register(key.selector(), SelectionKey.OP_READ);
						}
						if (key.isReadable()) {// �ж��Ƿ������ݷ��͹���
							
							// �ӿͻ��˶�ȡ����
							// �����ͻ���ͨ�ŵ��ŵ�  
					        SocketChannel clientChannel = (SocketChannel) key.channel();
					        // �õ�����ջ�����  
					        ByteBuffer total_buffer = recv_buffers.get(clientChannel.getRemoteAddress());
					        if (total_buffer==null) {
					        	total_buffer = ByteBuffer.allocate(BufferSize);
							}
					        recv_buffers.put(clientChannel.getRemoteAddress(), total_buffer);
//					        ByteBuffer buffer = (ByteBuffer) key.attachment();  
//					        buffer.clear();  
					        // ��ȡ��Ϣ��ö�ȡ���ֽ���  
					        int bytesRead =0;
					        try{
					        	bytesRead = clientChannel.read(total_buffer);
					        }catch(Exception exception){
					        	bytesRead = -1;
//					        	System.out.println(bytesRead);
					        }
					        if (bytesRead == -1) {  
					            // û�ж�ȡ�����ݵ����  
					            clientChannel.close();
					            key.cancel();
					            recv_buffers.remove(clientChannel);
//					            System.out.println("-1-1--1-1-1-1-1-1\n");
//					            keyIter.remove();
					        } else {  
					            // ��������׼��Ϊ���ݴ���״̬  
					        	total_buffer.flip();
					            int readResult = 0 ;
					            while ( (readResult = ParsePdu(total_buffer, clientChannel) ) >0) {
									//loop parse.
								}
					            key.interestOps(SelectionKey.OP_READ | SelectionKey.OP_WRITE);  
					            //�ж���ʼ���
					        }
						}
						if (key.isValid() && key.isWritable()) {// �ж��Ƿ���Ч�����Է��͸��ͻ���
							// �ͻ��˿�дʱ
							//protocol.handleWrite(key);
						}
						 
//					} catch (IOException ex) {
//						// ����IO�쳣����ͻ��˶Ͽ����ӣ�ʱ�Ƴ�������ļ�
//						System.out.println("IOException ex\n");
//						System.out.println("length is " +length +"\n");
////						keyIter.remove();
////						continue;
//					}
					// �Ƴ�������ļ�
//					System.out.println("remove processed key");
//					keyIter.remove();
				}
//				System.out.println("loop ����");
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				System.out.println("e.printStackTrace" + e.toString() + "\n");
				e.printStackTrace();
			}
		
		}
	}

}


 

