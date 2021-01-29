#ifndef LOADBALANCEOBJECT_H
#define LOADBALANCEOBJECT_H
#include "typedef.h"

#include <string>
#include <algorithm>

class LoadBalanceObject
{
public:
    LoadBalanceObject();

    std::string ip_ ;
    int port_;
    int current_balanced_user_num_ ;
    Socketid_t sockfd_ ;

    std::string token_ ;

    int regist_timestamp_ ;

    bool operator > (const LoadBalanceObject& _lbo){
        return this->current_balanced_user_num_ > _lbo.current_balanced_user_num_ ;
    }

    bool operator < (const LoadBalanceObject& _lbo ){
        return this->current_balanced_user_num_< _lbo.current_balanced_user_num_ ;
    }
//    //bool operator==(const family&)
//    bool operator !=(const LoadBalanceObject& _lbo) const
//    {
//        return (this->sockfd_ == _lbo.sockfd_ );
//    }

    bool operator==(const LoadBalanceObject &_lbo) const {
        return _lbo.sockfd_ == sockfd_;
    }

};

#endif // LOADBALANCEOBJECT_H
