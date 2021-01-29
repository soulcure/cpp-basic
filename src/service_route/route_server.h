#ifndef ROUTESERVER_H
#define ROUTESERVER_H

#include "net_base.h"

#include "route_object.h"
#include "typedef.h"
#include "load_balance_object.h"

#include <google/protobuf/message.h>

#include <unordered_map>
#include <list>

typedef std::unordered_map<Userid_t,RouteObject> Route_UserFd_Map_t ;
typedef std::list<LoadBalanceObject> Load_Balance_Map_t ;

class RouteServer : public NetBase
{
public:
    RouteServer();

    virtual void OnRecv(int sockid, PDUBase &_pack);

    int SendProto(int sockid,::google::protobuf::Message &_msg,  int _commandid,int _seq_id, Userid_t _user_id);

private:
    /*unordered_map<Userid_t, Socketid_t>*/
    Route_UserFd_Map_t user_map_ ;

    Load_Balance_Map_t load_balance_ ;

    Socketid_t Find_Sockid(Userid_t _userid);

    void ProcessRouteMessage(PDUBase &_pack);

    void ProcessTransSendMessage(PDUBase &_pack);

    void ProcessUserLogin(int sockid,PDUBase &_pack);

    void ProcessRegistService(int _sockid, PDUBase & _pack);

    void ProcessRequestIP(int _sockid, PDUBase & _pack );
    const static int trans_buffer_size_ = 1024*100;
    char trans_buffer_[trans_buffer_size_ ];

};

#endif // ROUTESERVER_H
