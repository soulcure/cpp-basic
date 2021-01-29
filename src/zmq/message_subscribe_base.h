#ifndef MESSAGE_SUBSCRIBE_BASE_H
#define MESSAGE_SUBSCRIBE_BASE_H

#include "pdu_util.h"
#include "pdu_base.h"
#include <zmq.hpp>
#include "typedef.h"
#include <list>
#include <string>
#include <vector>

class MessageSubscribeBase:public PduUtil{
public:
    MessageSubscribeBase();
    ~MessageSubscribeBase();

    int Connect(const char * _url);

    int SubscribeTopic( Topic_t _topic );
    int SubscribeTopics( std::vector< Topic_t > _topics );
    int UnsubcribeTopic( Topic_t _topic );

    virtual void OnSubscribe(Topic_t _topic, PDUBase & _base ) = 0 ;

protected:
    std::list<Topic_t> topics_ ;
    std::string url_ ;
    zmq::socket_t *subscriber_; // (context, ZMQ_SUB);
    zmq::context_t *context_ ;
private:
    void Run();
};

#endif // MESSAGE_SUBSCRIBE_BASE_H
