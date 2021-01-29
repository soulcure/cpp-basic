/**************************************
 * author@xiesong
 * songtzu@126.com
 * 4.15.2016
 */
#ifndef ROUTE_CLIENT_H
#define ROUTE_CLIENT_H

#include "typedef.h"
#include "pdu_base.h"
#include "pdu_util.h"
#include <zmq.hpp>

#include <list>
#include <string>

class RouteClientBase :public PduUtil
{
public:
    RouteClientBase();

    RouteClientBase(const char _url);

    void set_url(std::string &_url);

    int Connect();

    virtual void OnRecv(PDUBase &_base);

    /***********************************
     * this will route a  message to a single user(target.)
     */
    int RouteTarget(PDUBase &_base, Userid_t _userid);

    /************************************
     * this will route a message to a set of users(targets).
     */
    int RouteTargets(PDUBase &_base, std::list<Userid_t> &_userid);

private:
    std::string url_ ;

    void Run();


};

#endif // ROUTE_CLIENT_H
