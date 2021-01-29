#ifndef CACHEUSER_H
#define CACHEUSER_H
#include "typedef.h"
#include "YouMai.Basic.pb.h"
#include <string>


class CacheUser
{
public:
    CacheUser();
    Userid_t user_id_;
    com::proto::basic::USER_TYPE user_type_;
    std::string nick_name_;
    int db_id_;
};

#endif // CACHEUSER_H
