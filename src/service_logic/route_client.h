#ifndef ROUTE_CLIENT_H
#define ROUTE_CLIENT_H

#include "route_client_base.h"

class RouteClient:public RouteClientBase
{
public:
    RouteClient();
    virtual void OnRecv(PDUBase &_base);

};

#endif // ROUTE_CLIENT_H
