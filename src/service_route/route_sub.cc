#include "route_sub.h"

#include "route_server.h"
#include "log_util.h"
#include "time_util.h"
#include "YouMai.Route.pb.h"
#include "YouMai.Basic.pb.h"
#include "YouMai.User.pb.h"
#include <iostream>

using namespace std;
using namespace com::proto::basic;
using namespace com::proto::user ;


RouteSub::RouteSub()
{

}


void RouteSub::OnSubscribe(Topic_t _topic, PDUBase &_pdubase ){

}




void RouteSub::ProcessUserLogin(int sockid,PDUBase &_pack){
    User_Login user_login ;
    user_login.ParseFromArray(_pack.body.get(), _pack.length);

}
