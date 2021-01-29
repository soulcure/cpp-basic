#include "route_client.h"
#include "YouMai.Loadbalance.pb.h"
#include "YouMai.Basic.pb.h"
#include "YouMai.Chat.pb.h"
#include "log_util.h"
#include "main.h"

using namespace com::proto::loadbalance ;
using namespace com::proto::basic;
using namespace com::proto::chat;

RouteClient::RouteClient(){

}


void RouteClient::RegistService(const char * _ip, int _port){
    Regist_CommunicationService regist ;
    regist.set_ip(_ip);
    regist.set_port(_port);
    SendProto(regist,  REGIST_COMMUNICATIONSERVICE, 0);
}


//
void RouteClient::OnConnect(){
    LOGT("service com has estenbished connection to route server");
}


/******************************
 * songtzu@126.com
 * find the target users.
 * route the msg to target.
 */
void RouteClient::OnRecv(PDUBase &_base){
print_pdu(_base);
    switch(_base.command_id){ 
        default:
        {
            LOGD("Unknown command");
        }
        break;
     }
}
