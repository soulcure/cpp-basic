#include "route_client.h"
#include <thread>

using namespace std;

RouteClientBase::RouteClientBase(){

}

RouteClientBase::RouteClientBase(const char _url){
    url_ = _url ;
}

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


void RouteClientBase::Run(){
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect ("tcp://localhost:5555");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
        //  Get the reply.
        zmq::message_t reply;

        socket.recv (&reply);
        PDUBase base ;
        if( OnPduParse(reply.data(), reply.size(),base) >0 ){
            OnRecv(base);
        }
        //std::cout << "Received World " << request_nbr << std::endl;
    }
}


int RouteClientBase::RouteTarget(PDUBase &_base, Userid_t _userid){

}

int RouteClientBase::RouteTargets(PDUBase &_base, std::list<Userid_t> &_userid){

}
