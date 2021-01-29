/***************************************************
 * author@xiesong
 * time@4.20.2016
 * songtzu@126.com
 *
 */
#ifndef ROUTEOBJECT_H
#define ROUTEOBJECT_H

#include "typedef.h"

#include "YouMai.Basic.pb.h"

enum USER_ONLINE_STATUS{
    USER_ONLINE_CONNECTED = 0 ,
    USER_ONLINE_OFFLINE = 1,

};

class RouteObject
{
public:
    RouteObject();

    /*****************************
     * this is the socket fd a server connected to a route, which server user connected,
     *
     */
    Socketid_t fd_ ;

    /*************************************
     * this is the user id .
     */
    Userid_t userid_ ;

    /**********************
     * mark if user is online or not.
     */
    USER_ONLINE_STATUS online_status_ ;

    /**********************
     * this is the device_id.
     * standards for a device.
     */
    Device_ID_t device_id_ ;

    com::proto::basic::Device_Type device_type_ ;



};

#endif // ROUTEOBJECT_H
