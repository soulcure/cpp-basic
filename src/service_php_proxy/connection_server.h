#ifndef CONNECTIONSERVER_H
#define CONNECTIONSERVER_H

#include "net_base.h"
#include "message_publisher.h"

class ConnectionServer :public NetBase
{
public:
    int terminal_token_;
    ConnectionServer();
    virtual void OnRecv(int sockid, PDUBase &_pack) ;

private:
    MessagePublisher publisher_ ;

};

#endif // CONNECTIONSERVER_H
