#include "route_client_base.h"
#include "log_util.h"

#include "YouMai.Route.pb.h"
#include "YouMai.Basic.pb.h"
#include <thread>
#include <iostream>
using namespace std;

using namespace com::proto::route;
using namespace com::proto::basic;

RouteClientBase::RouteClientBase(){

}
/*
RouteClientBase::RouteClientBase(const char _url){
    url_ = _url ;
}
*/
/*
void RouteClientBase::set_url(std::string &_url){
    url_ = _url;
}


int RouteClientBase::Connect(){
    if(!url_.empty()){
        thread run(&RouteClientBase::Run, this) ;
        run.detach();
        return 0 ;
    }else{
        return -1 ;
    }

}

*/
//void RouteClientBase::Run(){
//    zmq::context_t context (1);
//    zmq::socket_t socket (context, ZMQ_REQ);

//    std::cout << "Connecting to hello world server..." << std::endl;
//    socket.connect ("tcp://localhost:5555");

//    //  Do 10 requests, waiting each time for a response
//    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
//        //  Get the reply.
//        zmq::message_t reply;
//        reply.

//        socket.recv (&reply);
//        PDUBase base ;
//        if( OnPduParse(reply.data(), reply.size(),base) >0 ){
//            OnRecv(base);
//        }
//        //std::cout << "Received World " << request_nbr << std::endl;
//    }
//}


int RouteClientBase::RouteTarget(PDUBase &_base, Userid_t _userid){
    Route_Req route_req ;
    route_req.add_targets(_userid);
    route_req.set_commandid(_base.command_id);
    char * binary_obj = NULL;
    int len = 0 ;
    len = OnPduPack(_base, binary_obj ) ;
    if(  len > 0 ){
        route_req.set_route_obj( binary_obj, len);
        SendProto(route_req, ROUTE_REQ, 0) ;
        free(binary_obj);
        return 0 ;
    }else{
        return -1 ;
    }

}

int RouteClientBase::RouteTargets(PDUBase &_base, std::list<Userid_t> &_userid){

    Route_Req route_req ;
    for(std::list<Userid_t>::iterator it= _userid.begin(); it!=_userid.end(); ++it){
        route_req.add_targets( *it );
    }

    route_req.set_commandid(_base.command_id);
    char * binary_obj = NULL;
    int len = 0 ;
    len = OnPduPack(_base, binary_obj ) ;
    if(  len > 0 ){
        route_req.set_route_obj( binary_obj, len);
        SendProto(route_req, ROUTE_REQ, 0) ;
        free(binary_obj);
        return 0 ;
    }else{
        return -1 ;
    }

}

void RouteClientBase::OnRecv(PDUBase &_base){
    LOGT("route got one item\n");
}
