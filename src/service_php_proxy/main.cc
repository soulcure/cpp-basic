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
#include <zmq.hpp>
#include "zhelpers.hpp"
#include <thread>
#include <assert.h>
#include <vector>
#include "connection_server.h"
#include "YouMai.Basic.pb.h"
#include "YouMai.User.pb.h"

using namespace std;
using namespace com::proto::user;
using namespace com::proto;

ConnectionServer connect_server ;

RouteClient route_client ;

ConfigRead reader ;
ConfigRead public_config ;

int
main(int argc, char ** argv){

    public_config.SetFilePath("../public.d");

    string route_ip = public_config.readString("route","ip") ;
    int route_port = public_config.readInt("route", "port");

    route_client.Connect(route_ip.c_str(), route_port);

    reader.SetFilePath("./config.d");
    connect_server.terminal_token_ = reader.readInt("terminal", "terminal_token");
    connect_server.StartServer(reader.readString("connection","ip").c_str(), reader.readInt("connection","port"));

}
