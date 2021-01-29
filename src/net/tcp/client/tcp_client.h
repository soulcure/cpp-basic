/*************************************************
 * author@xiesong
 * songtzu@126.com
 * 4.12.2016
 * copyright reserved.
 */
#ifndef H_TCPCLIENT_H
#define H_TCPCLIENT_H

#include "pdu_base.h"
#include "pdu_util.h"
#include <google/protobuf/message.h>
#include <string>

class TcpClient:public PduUtil
{
public:
    TcpClient();
    ~TcpClient();

    int Send(PDUBase &_base);

    int SendProto(::google::protobuf::Message &_msg,  int _commandid, int _seq_id);


    int Connect(const char *_ip, int _port );
    virtual void OnRecv(PDUBase &_base) = 0 ;
    virtual void OnConnect();

    /***************************************
     * this is a thread exec.
     * similar to java.
     */
    void Run();

private:
    const int resendnumlimit = 3;   //发送失败重发次数限制
    int sockfd ;
protected:
    int port;

    std::string ip ;

};

#endif // TCPCLIENT_H

