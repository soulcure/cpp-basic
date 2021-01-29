package com.net;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public abstract class PduUtil implements Runnable  {

	public abstract void OnRec(PduBase pduBase , SocketChannel channel);

//	public int ParsePdu(ByteBuffer buffer, SocketChannel channel) {
//		
//	}
	
	public int ParsePdu(ByteBuffer buffer, SocketChannel channel) {
		if (buffer.limit()>PduBase.pdu_basic_length) {
			//has start flag
//			System.out.println(PduBaseEnum.startflag.ordinal()*PduBase.pdu_basic_length + "\n");
			int begin = buffer.getInt(PduBaseEnum.startflag.ordinal()*PduBase.pdu_basic_length); 
//			System.out.println("begin is " + begin + "\n");
			if ( begin
					!= PduBase.startflag) {
				System.out.println("header error...\n");
				buffer.clear();
				return -1;
			}
		}else {	//did not contain a start flag yet.continue read.
            System.out.println("did not has full start flag");
            buffer.position(buffer.limit());
            buffer.limit(buffer.capacity());
            return 0;
        }
		
		if (buffer.limit()>= PduBaseEnum.length.ordinal()*PduBase.pdu_basic_length) {
			//has full header
//			 System.out.println("完整的包头 action" + buffer.getInt(PduBaseEnum.commandid.ordinal()*PduBase.pdu_basic_length));
			 int bodyLength = buffer
                        .getInt(PduBaseEnum.length.ordinal() * PduBase.pdu_basic_length);

             int totalLength = bodyLength
                        + (1 + PduBaseEnum.length.ordinal() )* PduBase.pdu_basic_length;
             if (totalLength <= buffer.limit()) {
            	 //has a full pack.
            	 byte[] packByte = new byte[totalLength];
//            	 System.out.println("2222222222 limit" + buffer.limit() + "pos" + buffer.position()+"to"+totalLength);
            	 buffer.get(packByte);
            	 PduBase pduBase = BuildPdu(packByte);
            	 OnRec( pduBase, channel );
//            	 System.out.println("begin to clean pos:"+buffer.position()+"lim:"+buffer.limit());
            	 buffer.compact();// 清理已读内容。
            	 //read to read.
            	 buffer.flip();
            	 return totalLength;
            	 
             }else {
            	System.out.println("缓存中尚没读取整个包的全部数据");
               
                buffer.position(buffer.limit());
                buffer.limit(buffer.capacity());
                return 0;
			}
			
		}else {
			System.out.println(" not a full header");
            buffer.position(buffer.limit());
            buffer.limit(buffer.capacity());
            return -2;
		}
	}
	
	private PduBase BuildPdu(byte[] bytes) {
		PduBase units =new PduBase();
		ByteBuffer buffer = ByteBuffer.allocate(bytes.length);
		buffer.put(bytes);
		buffer.flip();
		
		int length = buffer.getInt(PduBaseEnum.length.ordinal() * PduBase.pdu_basic_length) ;	
		units.terminal_token = buffer.getInt(PduBaseEnum.terminal_token.ordinal()* PduBase.pdu_basic_length) ;	
		units.commandid = buffer.getInt(PduBaseEnum.commandid.ordinal() * PduBase.pdu_basic_length ) ;
			
		units.seq_id = buffer.getInt(PduBaseEnum.seq_id.ordinal() * PduBase.pdu_basic_length)  ;
		units.length = (int) length ;
		units.body = new byte[length]; 
		buffer.position(PduBaseEnum.body.ordinal()*PduBase.pdu_basic_length);
//		System.out.println("length is " + length +  "\n" + buffer.position()+"limit" +buffer.limit());
		buffer.get(units.body, 0 ,  length);
 
		return units;
 
	}
	
	public byte[] SerializePdu(PduBase pduBase) {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		DataOutputStream dos = new DataOutputStream(bos);
		try {
			dos.writeInt(PduBase.startflag);
			dos.writeInt(pduBase.terminal_token);
			dos.writeInt(pduBase.commandid);
			dos.writeInt(pduBase.seq_id);
			dos.writeInt(pduBase.length);
			dos.write(pduBase.body); 
			return bos.toByteArray();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}
	}
	
	
}
