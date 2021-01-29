#include "route_client.h"

RouteClient::RouteClient()
{

}

void RouteClient::OnRecv(PDUBase &_base){
    printf( "%d \n", _base.command_id );
    //find user, send it to user.
}
