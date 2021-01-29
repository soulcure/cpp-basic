#include "connection_server.h"
#include "YouMai.Basic.pb.h"
#include "YouMai.User.pb.h"
#include "logic_tools.h"
#include "config_read.h"
#include "log_util.h"


#include <iostream>

using namespace std;
using namespace com::proto;
using namespace com::proto::user;

ConnectionServer::ConnectionServer(){

    ConfigRead read("../public.d");

    publisher_.ConnectPub(read.readString("mq_proxy", "front").c_str());
}

void ConnectionServer::OnRecv(int sockid, PDUBase &_pack)
{
    LOGD("SUBCLASS ->ONRECV");
    print_pdu(_pack);
    _pack.terminal_token = terminal_token_;

    if(_pack.command_id> BONDERY_MQ)
    {
        //this should route to MQ. to let it get furture process.
        cout<<songtzu::build_topic(_pack.command_id)<<endl;
        this->publisher_.PublishMessage(songtzu::build_topic(_pack.command_id), _pack);
    }
    else
    {
        switch (_pack.command_id) {
        case HEART_BEAT:
        {
            Heart_Beat heart ;
            heart.ParseFromArray(_pack.body.get(), _pack.length );
            Heart_Beat_Ack ack;
            ack.set_user_id(heart.user_id());
            _pack.command_id = HEART_BEAT_ACK ;
            std::shared_ptr<char> new_body (new char[ack.ByteSize()]);
            ack.SerializeToArray(new_body.get(), ack.ByteSize());
           // memcpy(new_body.get(), ack.SerializeToArray(new_body.get(), ack.ByteSize()), ack.ByteSize());
            _pack.body = new_body ;
            Send(sockid, _pack) ;
        }
        break;

        case ROUTE_REQ:
        {

        }
        break;

        case USER_LOGIN:
        {
            User_Login login;
            login.ParseFromArray(_pack.body.get(),_pack.length);
            std::cout << login.user_id() << login.pwd() << login.phone() << login.device_id() << login.device_type() << std::endl;
        }
         break;

        case MULTI_DEVICE_KICKED_NOTIFY:
        {
            int a = 0;
        }
        break;

        default:
        {
            LOGD("Unknown command");
        }
        break;
        }
    }
}
