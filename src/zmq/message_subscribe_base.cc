#include "message_subscribe_base.h"
#include "log_util.h"
#include <iostream>
#include <thread>

using namespace std;


MessageSubscribeBase::~MessageSubscribeBase(){
    delete context_ ;
    delete subscriber_ ;
}
MessageSubscribeBase::MessageSubscribeBase(){
    context_ = nullptr ;
    subscriber_ = nullptr ;
}

int MessageSubscribeBase::Connect(const char * _url){

    context_ = new zmq::context_t(1);
      //  Socket to talk to server
    std::cout << "Collecting updates from weather server...\n" << std::endl;
    subscriber_ = new zmq::socket_t(*context_, ZMQ_SUB);
    subscriber_->connect(_url);


    for(list<Topic_t>::iterator topic = topics_.begin(); topic!=topics_.end();++topic){

//        char *topic_str = new char[sizeof(Topic_t)] ;
//        memcpy(topic_str, &(*it), sizeof(Topic_t) );
        subscriber_->setsockopt(ZMQ_SUBSCRIBE, topic->c_str(),  topic->length()) ;
//        delete topic_str ;
    }
    thread run( &MessageSubscribeBase::Run, this);
    run.detach();
    return 0;
}


int MessageSubscribeBase::SubscribeTopics(vector<Topic_t> _topics){
LOGD("SubscribeTopics");
    if(subscriber_==nullptr || !subscriber_->connected()){
        LOGT("subscribe topic failed, due to not connected");
        return -1;
    }
LOGD("TTTTTTTTTTTTTTTTTTTTTTT");
    for (vector<Topic_t>::iterator topic = _topics.begin() ; topic != _topics.end(); ++topic){
        std::cout << ' ' << *topic << std::endl;
        subscriber_->setsockopt(ZMQ_SUBSCRIBE, topic->c_str(), topic->length() ) ;
    }
LOGD("EEEEEEEEEEEEEEEEEEEEEEE");
    return 0;
}

int MessageSubscribeBase::SubscribeTopic( Topic_t _topic ){
    topics_.push_back(_topic);
    if(subscriber_!=nullptr){
        subscriber_->setsockopt(ZMQ_SUBSCRIBE, _topic.c_str(), _topic.length() ) ;
     }
}

int MessageSubscribeBase::UnsubcribeTopic( Topic_t _topic ){
    topics_.remove(_topic);
    if(subscriber_!=nullptr){
        subscriber_->setsockopt(ZMQ_UNSUBSCRIBE, _topic.c_str(), _topic.length() ) ;
     }
}



void MessageSubscribeBase::Run(){
    while(true){
        zmq::message_t msg;
        subscriber_->recv(&msg);
        char topic_cstr[TOPIC_LENGTH+1] = {'\0'} ;
       // std::cout <<"size is" << msg.size() << " \n zipcode"<< zipcode<< std::endl;
        Topic_t recieve_topic ;
        memcpy(topic_cstr,msg.data(),  TOPIC_LENGTH );
        recieve_topic = topic_cstr ;
        PDUBase pdu ;
        cout<<msg.size()<<endl;
        OnPduParse( ((char*)msg.data())+ TOPIC_LENGTH, msg.size()- TOPIC_LENGTH, pdu);
        OnSubscribe(recieve_topic, pdu);
    }
}

/*
void MessageSubscribeBase::OnSubscribe(Topic_t _topic, PDUBase &_pdubase ){

}
*/
