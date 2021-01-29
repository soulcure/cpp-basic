/**
 * author@xiesong.4.11.2016
 * songtzu@126.com
 */

#include "config_read.h"
#include "log_util.h"

#include "zmq.hpp"
#include "zhelpers.hpp"
#include "tcp_client.h"
#include "net_base.h"


#include <stdio.h>
#include <iostream>
#include <thread>               // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>   // std::condition_variable

std::mutex mtx;                 //lock for comsumer&product model.
std::condition_variable cv;     //condition for comsumer&product.

using namespace std;




/**
 * this is message queue proxy for
 *  sub & pub model.
 * http://zguide.zeromq.org/page:all
 * http://stackoverflow.com/questions/18570810/how-to-implement-pub-sub-network-with-a-proxy-by-using-xpub-and-xsub-in-zeromqc
 */
int
main(int argc, char **argv){

    ConfigRead read("../public.d");

    zmq::context_t context(1);
    //  Socket facing clients
    //  This is where the weather server sits
    zmq::socket_t frontend (context, ZMQ_XSUB);
    string url = read.readString("mq_proxy","front").c_str() ;
LOGT("the url is %s",url.c_str());
    frontend.bind(read.readString("mq_proxy","front").c_str());

    //  Socket facing services
    //  This is our public endpoint for subscribers
    zmq::socket_t backend (context, ZMQ_XPUB);
    read.readString("mq_proxy","backend").c_str();
    LOGT( "the backend url is %s", read.readString("mq_proxy","backend").c_str()) ;
    backend.bind(read.readString("mq_proxy","backend").c_str());
    //  Start the proxy
    zmq::proxy(static_cast<void *>(frontend), static_cast<void *>(backend), nullptr);

    return 0;


  //  start_mq(read.readString("mq","url"));

}
