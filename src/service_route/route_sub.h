#ifndef ROUTE_SUB_H
#define ROUTE_SUB_H
#include "message_subscribe_base.h"

class RouteSub :public MessageSubscribeBase
{
public:
    RouteSub();

    virtual void OnSubscribe(Topic_t _topic, PDUBase &_pdubase );

    void ProcessUserLogin(int sockid,PDUBase &_pack);

};

#endif // ROUTE_SUB_H
