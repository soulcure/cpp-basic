package com.net;

import java.nio.channels.SocketChannel;

class ServerTest extends TcpServer{

	@Override
	public void OnRec(PduBase pduBase, SocketChannel channel) {
		// TODO Auto-generated method stub
		System.out.println("pdu commandid" + pduBase.commandid + "\npduBase.seq_id" + pduBase.seq_id   +"\npduBase.terminal_token"+ pduBase.terminal_token 
				+ "\npduBase.length"+ pduBase.length);
		SendPdu(pduBase, channel);
	}
	
}


public class ServerSample {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ServerTest serverTest = new ServerTest();
		serverTest.Connect("127.0.0.1",8001);
		Thread thread = new Thread(serverTest);
		thread.start();
//		serverTest.run();
	} 

}
