#ifndef CONNECTIONSERVER_H
#define CONNECTIONSERVER_H
#include "route_client.h"
#include "message_publisher.h"
#include "net_base.h"
#include "client_object.h"
#include "dao_client.h"
#include "cache_user.h"
#include "YouMai.Chat.pb.h"
#include <string>
#include <unordered_map>

typedef std::unordered_map<Userid_t, ClientObject> User_Connect_Map_t ;
typedef std::unordered_map<std::string, CacheUser> Phone_CacheUser_Map_t ;

extern User_Connect_Map_t    connect_user;

class ConnectionServer :public NetBase
{
public:
    ConnectionServer();
    virtual void OnRecv(int sockid, PDUBase &_pack) ;

    virtual void OnConn(const char *ip, short port);
 
    User_Connect_Map_t    user_map_;

    inline Socketid_t find_fd_by_userid(Userid_t _userid){
        User_Connect_Map_t::iterator it = user_map_.find(_userid) ;
        if (user_map_.end()!= it ) {
            return it->second.fd_ ;
        }
        return 0;
    }

    void RoutePdu(PDUBase _base);
    int Find_UserCache(const std::string &_phone, CacheUser &_cache_user);
 

private:
 
    std::string mqproxy_front_url_;
    MessagePublisher publisher_ ;

    RouteClient route_client_ ;

    std::string route_server_ip_ ;
    int route_server_port_ ;

    std::string db_proxy_ip_ ;
    int db_proxy_port_ ;
};

#endif // CONNECTIONSERVER_H
