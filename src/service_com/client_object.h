#ifndef CLIENT_OBJECT_H
#define CLIENT_OBJECT_H

#include "typedef.h"
#include "YouMai.Basic.pb.h"
#include "YouMai.User.pb.h"

#include <list>
#include <string>

using namespace com::proto::user;
using namespace com::proto::basic;

#include <unordered_map>
typedef std::unordered_map<std::string, char> User_BlackList_Map_t ;
enum BlackWhiteSetting {
    BLACK_LIST = 1,
    WHITE_LIST = 2,
};

class ClientObject
{
public:
    ClientObject();

    /*****************************
     * this is the user socket fd
     *
     */
    Socketid_t fd_ ;

    /*************************************
     * this is the user id .
     */
    Userid_t userid_ ;

    /**********************
     * this is the device_id.
     * standards for a device.
     */
    Device_ID_t device_id_ ;


    Device_Type device_type_;
//     YouMai::Basic::Device_Type device_type_ ;

    /**********************
     * this is the user balcklist.
     * key: dst phone
     * value:  0:white list     1:dst phone is own black bright     2: dst phone The other party does not allow themselves to see
     */
   User_BlackList_Map_t blacklist_map_;

   /*******************
    * set val
    *       blackwhite_setting_ = blackwhite_setting_ | BLACK_LIST
    * check val
    *       bool  = blackwhite_setting_ & BLACK_LIST
    */
   int blackwhite_setting_ ;
   std::string phone_;

    int     port_;
    std::string ip_;
};

#endif
