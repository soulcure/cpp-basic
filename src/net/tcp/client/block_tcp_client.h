/*************************
 * songtzu@126.com
 * xiesong.
 * block tcp client.
 */
#ifndef BLOCKTCPCLIENT_H
#define BLOCKTCPCLIENT_H
#include <google/protobuf/message.h>
#include <string>

#include "pdu_base.h"
#include "pdu_util.h"

class BlockTcpClient:public PduUtil
{
public:
    BlockTcpClient();
    ~BlockTcpClient();

    int Send(PDUBase &_base);

    int SendProto(::google::protobuf::Message &_msg,  int _commandid, int _seq_id);


    int Connect(const char *_ip, int _port );
     int Read(PDUBase &base);


private:
     int sockfd ;
protected:
    int port;

    std::string ip ;

    const int resendnumlimit = 3 ;



};

#endif // BLOCKTCPCLIENT_H
