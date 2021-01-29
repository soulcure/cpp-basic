#ifndef ROUTECLIENT_H
#define ROUTECLIENT_H

#include "route_client_base.h"

class RouteClient:public RouteClientBase
{
public:
    RouteClient();
    virtual void OnRecv(PDUBase &_base);
    virtual void OnConnect();
    void RegistService(const char * _ip, int _port);
};

#endif // ROUTECLIENT_H
