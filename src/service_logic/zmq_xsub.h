#ifndef ZMQ_XSUB_H
#define ZMQ_XSUB_H

#include "message_subscribe_base.h"

class Zmq_XSub :public MessageSubscribeBase
{
public:
    Zmq_XSub();

    virtual void OnSubscribe(Topic_t _topic, PDUBase &_pdubase );


};

#endif // ZMQ_XSUB_H
