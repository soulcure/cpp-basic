#include "message_publisher.h"
#include "logic_tools.h"
#include"log_util.h"

#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>






int MessagePublisher::ConnectPub(const char * _url) {
    //  Prepare our context and publisher
    context_ = new zmq::context_t(1);
//    zmq::socket_t publisher (context, ZMQ_PUB);
    pub_sock_ = new zmq::socket_t(*context_, ZMQ_PUB);
    pub_sock_->connect(_url);
//    pub_sock_->bind("ipc://weather.ipc");				// Not usable on Windows.

    //  Initialize random number generator
    srandom ((unsigned) time (NULL));

    return 0;
}

int MessagePublisher::PublishMessage(PDUBase &_base){
    PublishMessage(songtzu::build_topic(_base.command_id), _base);
}

int MessagePublisher::PublishMessage(Topic_t _topic, PDUBase &_base){
    char * buf= NULL;
    int len = OnPduPack(_base, buf);
    if( NULL==buf ||len<=0 )
        return -1;
    zmq::message_t message( TOPIC_LENGTH + len );
    memcpy( (char*)message.data(), _topic.c_str(), TOPIC_LENGTH);
    memcpy(((char*)message.data())+TOPIC_LENGTH, buf, len);
    pub_sock_->send(message);

    if(buf!=NULL)
        free(buf);
    return 0 ;
}


int MessagePublisher::PublishMessage(Topic_t _topic, void *_stream, int _length){

    zmq::message_t message( _topic.length() + _length );

    memcpy(message.data(),  _topic.c_str() , _topic.length() );
    memcpy( ( (char*)message.data() ) + _topic.length() ,_stream , _length ) ;

    pub_sock_->send(message);
    return 1;

}

