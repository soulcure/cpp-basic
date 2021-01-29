package com.net;

import YouMai.Basic.YouMaiBasic.Device_Type;
import YouMai.User.YouMaiUser.User_Login;

class Sample extends TcpClientBase{
	
}

public class ClientSample {

	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Sample sample = new Sample()  ;
		sample.SetUrl("127.0.0.1", 8001);
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
	 
		 
		
		sample.SendProto( user_Login, new ReceiveListener() {
			
			@Override
			public void OnRec(PduBase pduBase) {
				// TODO Auto-generated method stub
				System.out.println("dddddddddddddddddddddddddddddddddddddddddddd");
			}
		});
		
	}

}
