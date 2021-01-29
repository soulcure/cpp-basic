#include "typedef.h"
#include "logic_tools.h"
#include "string_split.h"
#include "log_util.h"
#include "route_server.h"
#include "config_read.h"
#include "route_sub.h"

#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "zhelpers.hpp"
#include <assert.h>

using namespace std ;



RouteServer routeserver ;


RouteSub route_sub;


int main () {

    ConfigRead reader ;

    reader.SetFilePath("./config.d");

    string topics = reader.readString("xpub_proxy", "topics") ;

    vector<string> topic_vectors ;
  //  songtzu::split(topics, ',',topic_vectors);
    songtzu::convert_str_to_topics(topics, topic_vectors);
    reader.SetFilePath("../public.d");
    string url = reader.readString("mq_proxy","backend").c_str() ;
    cout <<"length is "<< url.length() << url<<endl ;
    route_sub.Connect( reader.readString("mq_proxy","backend").c_str());

    route_sub.SubscribeTopics(topic_vectors);


    string route_ip = reader.readString("route", "ip");
    //assert(!route_ip.empty());
    int route_port = reader.readInt("route", "port");
    //assert(0!=route_port);
    if(route_ip.empty() || route_port==0)
    {
        LOGT("config error , exit program \n");
        exit(0);
    }

    LOGT("begin\n");
    routeserver.StartServer( route_ip, route_port );

    LOGT("finished\n");
    return 0;

}
