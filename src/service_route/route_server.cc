#include "route_server.h"
#include "log_util.h"

#include "time_util.h"
#include "YouMai.Route.pb.h"
#include "YouMai.Basic.pb.h"
#include "static_config.h"
#include "YouMai.User.pb.h"
#include "YouMai.Loadbalance.pb.h"
#include "config_read.h"

#include <iostream>
#include <algorithm>
//using namespace std;
using namespace com::proto::loadbalance ;
using namespace com::proto::basic;
using namespace com::proto::user ;
using namespace com::proto::route ;

RouteServer::RouteServer(){
}

void RouteServer::ProcessRouteMessage(PDUBase &_pack){
    std::cout <<time(NULL)<< ":RouteServer 通用转发"  <<std::endl;
    Route_Req carrier ;
    std::cout << "root package len:" << _pack.length << std::endl;
    if( !carrier.ParseFromArray(_pack.body.get(), _pack.length) )
        return ;

    std::cout <<"carrier.act_code:"<< carrier.commandid() << std::endl;
//    cout <<"carrier.carry_length:" << carrier.route_obj().size()<< endl;
    std::shared_ptr<char> pbody(new char[carrier.route_obj().size()]) ;
    //base64_decode( pbody.get(),carrier.b64_body().c_str(), carrier.b64_body().length() );
    memcpy(pbody.get(), carrier.route_obj().data(), carrier.route_obj().size() );

    memset(trans_buffer_, 0, trans_buffer_size_);
    memcpy(trans_buffer_, carrier.route_obj().data(), carrier.route_obj().size());
    //_pack.body = pbody ;

//    PDUBase trans_pdu ;
//    OnPduParse(carrier.route_obj().data(),carrier.route_obj().size(), trans_pdu);
    for( int loop = 0; loop<carrier.targets_size(); ++loop){
        Userid_t targetid = carrier.targets(loop);
//        cout << "route to :"<<targetid<<endl;
        //路由app
        int fd = Find_Sockid( targetid );
        if(fd > 0) {
//            _pack.terminal_token = targetid ;
//            trans_pdu.terminal_token = targetid ;
//            Send(fd, trans_pdu);
            Set_Userid(trans_buffer_, targetid);
            Send(fd, trans_buffer_, carrier.route_obj().size() );
//            Send(fd, _pack);
        }

    }
//    cout << "route finished" << endl;

}

/*
 *
 *
 */
void RouteServer::ProcessTransSendMessage(PDUBase &_pack){

        Socketid_t sock= Find_Sockid(_pack.terminal_token);
        if (sock != 0 && Send(sock, _pack)){
        }
}

void RouteServer::ProcessUserLogin(int sockid,PDUBase &_pack){
    User_Login user_login ;
    user_login.ParseFromArray(_pack.body.get(), _pack.length);
    Route_UserFd_Map_t::iterator it = this->user_map_.find(user_login.user_id()) ;
    if(/*check multi login conflict*/it!= this->user_map_.end()) {
        Multi_Device_Kicked_Notify kick_notify ;

        kick_notify.set_timestamp(songtzu::timestamp_int());
        kick_notify.set_new_device(user_login.device_type());
        PDUBase reply ;
//        reply.
    }
    if(true /* check authority from database */){
        RouteObject route_object ;
        route_object.device_id_ = user_login.device_id();
        route_object.device_type_ = user_login.device_type();
        route_object.fd_ = sockid ;
        route_object.online_status_ = USER_ONLINE_CONNECTED;
        route_object.userid_ = user_login.user_id();
        this->user_map_.insert({route_object.userid_, route_object});
    }
}

void RouteServer::ProcessRegistService(int _sockid, PDUBase & _pack){
    Regist_CommunicationService regist ;
    regist.ParseFromArray(_pack.body.get(), _pack.length);
    LoadBalanceObject obj ;
    obj.ip_ = regist.ip();
    obj.port_ = regist.port();
    obj.sockfd_ = _sockid ;
    obj.regist_timestamp_ = songtzu::timestamp_int();
    load_balance_.push_back(obj);
    //ascending sort
    load_balance_.sort();
}

/*******************************************
 * ip allocat request process.
 *      if user has connect to any server before, just allocat the same server.
 *          check if it is the same or not.
 *          if it is the same device. ignore.
 *          else dispatch multi device kick notify to the old user..
 *      elseif allocat mini balanced server.
 * songtzu@126.com
 * 5.5.2016
 */
void RouteServer::ProcessRequestIP(int _sockid, PDUBase & _pack ){
    Request_IP request;

    Route_UserFd_Map_t::iterator it = user_map_.find( request.user_id() );
    if(it!=user_map_.end()){
        //got it.
        RouteObject routeobj = it->second ;
        if(routeobj.device_id_!= request.device_id()){
            //multi device kick notify
            Multi_Device_Kicked_Notify kicked_notify ;
            kicked_notify.set_new_device(request.device_type());
            SendProto(routeobj.fd_, kicked_notify, MULTI_DEVICE_KICKED_NOTIFY, 0, routeobj.userid_);
        }
        //return ack.
        LoadBalanceObject search_obj ;
        search_obj.sockfd_ = routeobj.fd_ ;
        Request_IP_Ack ack ;
        Load_Balance_Map_t::iterator loadobj_iterator = std::find(load_balance_.begin(), load_balance_.end(), search_obj);
        ack.set_ip(loadobj_iterator->ip_);
        ack.set_port(loadobj_iterator->port_);
        std::shared_ptr<char> new_body(new char[ack.ByteSize()]);
        ack.SerializeToArray(new_body.get() , ack.ByteSize());
        _pack.body = new_body ;
        _pack.command_id = REQUEST_IP_ACK ;
        Send(_sockid, _pack);
    }else{
        LoadBalanceObject lbo = load_balance_.front();
        Request_IP_Ack ack ;
        ack.set_ip(lbo.ip_);
        ack.set_port(lbo.port_);
        std::shared_ptr<char> new_body(new char[ack.ByteSize()]);
        ack.SerializeToArray(new_body.get() , ack.ByteSize());
        _pack.body = new_body ;
        _pack.command_id = REQUEST_IP_ACK ;
        Send(_sockid, _pack);
    }

}

Socketid_t RouteServer::Find_Sockid(Userid_t _userid){
    Route_UserFd_Map_t::const_iterator it = user_map_.find(_userid);
    if(it!=user_map_.end()){
        return  it->second.fd_ ;
    }

    return 0;
}

int RouteServer::SendProto(int sockid, ::google::protobuf::Message &_msg,  int _commandid,int _seq_id, Userid_t _user_id){
    PDUBase pdu_base ;
    std::shared_ptr<char> body(new char[_msg.ByteSize()]);
    _msg.SerializeToArray(body.get(), _msg.ByteSize());
    pdu_base.body = body ;
    pdu_base.length = _msg.ByteSize();
    pdu_base.terminal_token = _user_id ;
    pdu_base.command_id = _commandid ;
    pdu_base.seq_id = _seq_id ;
    Send(sockid, pdu_base);
}

void RouteServer::OnRecv(int sockid, PDUBase &_pack){

print_pdu(_pack);
    switch (_pack.command_id) {
    case ROUTE_REQ:
        ProcessRouteMessage(_pack);
        break;
    case IMCHAT_PERSONAL:
        ProcessTransSendMessage(_pack);
    break;
    case USER_LOGIN:
        ProcessUserLogin(sockid, _pack);\
        break;
    case REGIST_COMMUNICATIONSERVICE:
        LOGT("REGIST_COMMUNICATIONSERVICE");
        ProcessRegistService(sockid, _pack);
        //a connect service has connect to this route server.
        break;
    case REQUEST_IP:

    default:
        LOGD("not supported format");
        break;
    }


}
