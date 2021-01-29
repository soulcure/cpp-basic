/*******************************************************
 * @author.xiesong
 * @time.4.20.2016
 * songtzu@126.com
 *
 * this component has a xsub client.
 *      which recv request
 * and a route client. which will route infomation to router.
 * BOTH OF  that are IMPORTANT.
 */
#include "log_util.h"
#include "logic_tools.h"
#include "route_client.h"
#include "string_split.h"
#include "config_read.h"
#include "zmq_xsub.h"
#include <zmq.hpp>
#include "zhelpers.hpp"
#include <thread>
#include <assert.h>
#include <vector>

using namespace std;

Zmq_XSub zmq_xsub;

RouteClient route_client ;

ConfigRead reader ;
ConfigRead public_config ;

int
main(int argc, char ** argv){

    public_config.SetFilePath("../public.d");

    reader.SetFilePath("./config.d");
    string xsub_url = public_config.readString("mq_proxy","backend");
    LOGD("the url is:%s; %d", xsub_url.c_str(), xsub_url.length());
    string topics_str = reader.readString("xpub_proxy", "topics") ;
    string route_ip = public_config.readString("route","ip") ;
    int route_port = public_config.readInt("route", "port");
LOGT(" xsub_url:%s\n topics:%s\n routeip:%s\n routeport:%d\n", xsub_url.c_str(), topics_str.c_str(), route_ip.c_str(), route_port);
    if(xsub_url.empty() || topics_str.empty()){
        LOGT("empty string , exit program") ;
    }
    //assert( xsub_url.empty() ) ;
    //assert( topics_str.empty() ) ;
    vector<Topic_t> topic_vectors ;
   // songtzu::split(topics_str, ',', topic_vectors) ;
    songtzu::convert_str_to_topics(topics_str, topic_vectors);
    zmq_xsub.Connect(xsub_url.c_str());
LOGD("LLLLLLLLLLLLLLLLLLLLLLLLL");
//    route_client.Connect(route_ip.c_str(), route_port);

    // zmq_xsub.SubscribeTopic("");
    zmq_xsub.SubscribeTopics(topic_vectors);
    route_client.Connect(route_ip.c_str(), route_port);

    string end ;
    cin >> end ;
}
