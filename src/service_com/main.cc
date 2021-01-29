/**
 * author@xiesong.4.11.2016
 * songtzu@126.com
 */

#include "connection_server.h"
#include "message_publisher.h"
#include "config_read.h" 
#include "log_util.h"

#include "static_config.h"
#include "zmq.hpp"
#include "zhelpers.hpp"
#include "tcp_client.h"
#include "net_base.h"
#include "main.h"
#include "static_config.h"

#include <stdio.h>
#include <iostream>
#include <thread>               // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>   // std::condition_variable

std::mutex mtx;                 //lock for comsumer&product model.
std::condition_variable cv;     //condition for comsumer&product.

using namespace std;
using namespace com::proto::user;
using namespace com::proto::basic;
 
ConnectionServer connect_server;
// clienttest cli;
ThreadPool thread_pool(CONNECTPOOL_AND_THREADPOOL_NUMBER);
DBConnectionPool db_connect_pool;
/**
 * this is message queue proxy for
 *  sub & pub model.
 *
 */
int
main(int argc, char **argv){ 

    ConfigRead public_config(GLOBAL_CONFIG_URL) ;
    db_connect_pool.Init_Pool(public_config.readString(CONFIG_SECTION_DBPROXY, CONFIG_SECT_DBPROXY_KEY_IP),
                                                  public_config.readInt(CONFIG_SECTION_DBPROXY, CONFIG_SECT_DBPROXY_KEY_PORT) , CONNECTPOOL_AND_THREADPOOL_NUMBER);
    ConfigRead private_config("./config.d") ;

    connect_server.StartServer(private_config.readString("connection","ip").c_str(), private_config.readInt("connection","port"));


    return 0;

}




