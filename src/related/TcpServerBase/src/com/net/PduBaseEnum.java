package com.net;

public enum PduBaseEnum {
	startflag ,			//开始标记
	terminal_token ,			//发起者的userid，notify类型而言，是接收者的userid。任何接收自路由服务的数据包均需要解此字段。】
                        //其他情况均无需特别处理。
	commandid ,			//操作码
	seq_id ,			//包头的序列号id是为了让app匹配服务端发的数据和app自己发起的请求的结果。
	                    //例如，app发起查询某群组信息的请求，后端收到请求，向数据库查询，这时候其他用户B给他发了一个IM消息，
	                    //后台转发IM消息给app，app需要明白这个数据报不是他主动请求的回复。
	length ,			//pb长度
	body ,
}
