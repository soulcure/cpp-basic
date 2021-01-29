#include "connection_server.h"
#include "logic_tools.h"
#include "config_read.h"
#include "log_util.h"
#include "static_config.h"
#include "main.h"
#include "time_util.h"
#include "string_split.h"
#include "db_connection_pool.h"

#include <iostream>

using namespace std;

ConnectionServer::ConnectionServer(){

    ConfigRead public_config(GLOBAL_CONFIG_URL);
    this->mqproxy_front_url_ = public_config.readString(CONFIG_SECTION_MQPROXY, CONFIG_SECT_MQPROXY_KEY_FRONT);

    this->route_server_ip_ = public_config.readString(CONFIG_SECTION_ROUTE,CONFIG_SECT_ROUTE_KEY_IP);
    this->route_server_port_  = public_config.readInt(CONFIG_SECTION_ROUTE, CONFIG_SECT_ROUTE_KEY_PORT);
    this->db_proxy_ip_ = public_config.readString(CONFIG_SECTION_DBPROXY, CONFIG_SECT_DBPROXY_KEY_IP);
    this->db_proxy_port_ = public_config.readInt(CONFIG_SECTION_DBPROXY, CONFIG_SECT_DBPROXY_KEY_PORT);
    if(this->mqproxy_front_url_.empty() || route_server_ip_.empty() || route_server_port_ == 0 ){
        LOGT("system error, config file error") ;
        exit(0);
    }


    publisher_.ConnectPub(this->mqproxy_front_url_.c_str());

    int val = route_client_.Connect( route_server_ip_.c_str(), route_server_port_ );

    if(!val){
        route_client_.RegistService(this->ip.c_str(), this->port);
    }


}

void ConnectionServer::OnConn(const char *ip, short port){

}



void ConnectionServer::OnRecv(int sockid, PDUBase &_pack)
{
   
    print_pdu(_pack);
    switch (_pack.command_id) {

    default:
        LOGD("Unknown command");
        publisher_.PublishMessage(_pack);
        break;
    }
    
}


/****************************************
 * this function process the route client to user.
 */
void ConnectionServer::RoutePdu(PDUBase _base){
    Socketid_t fd = find_fd_by_userid(_base.terminal_token);
    if(fd){
        Send(fd, _base);
    }
}

