package com.net;

import YouMai.Basic.YouMaiBasic.Device_Type;
import YouMai.User.YouMaiUser.User_Login;

class Sample extends TcpClientBase{
	
}

public class ClientSample {

	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Sample sample = new Sample()  ;
		sample.SetUrl("192.168.0.8", 8003);
		Thread thread = new Thread(sample);
		thread.start();
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		User_Login.Builder login = User_Login.newBuilder();
		login.setPhone("18211111111");  
		login.setUserId(0);
		login.setDeviceId("deviceid");
		login.setDeviceType(Device_Type.DeviceType_Android); 
		User_Login user_Login = login.build();  
	 
		 
		while (true) {
		sample.SendProto( user_Login, new ReceiveListener() {
			
			@Override
			public void OnRec(PduBase pduBase) {
				// TODO Auto-generated method stub
				System.out.println("dddddddddddddddddddddddddddddddddddddddddddd");
			}
		});
		
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		}
		
	}

}
