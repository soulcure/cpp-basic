#include "db_connection_pool.h"
#include"log_util.h"

DBConnectionPool::DBConnectionPool(){
//    seq_id_ = 0;
}



DBConnectionPool::~DBConnectionPool(){

    while (!this->free_connection_list_.empty()){
        BlockTcpClient * client = free_connection_list_.back();
        delete client ;
        free_connection_list_.pop_back();
    }
}

int DBConnectionPool::SendProto(::google::protobuf::Message &_msg,
                                      int _commandid , bool _has_read ){
    std::lock_guard<std::mutex> lck (mutex_);
    ++seq_id_ ;
    if(this->seq_id_<=0){
        this->seq_id_ = 1;
    }
    //allocat a connect to run this task.
    BlockTcpClient* client = free_connection_list_.front();

    if (client == NULL){
        LOGD("client == NULL");
        return 0;
    }
    client->SendProto(_msg, _commandid, this->seq_id_);

    if(_has_read){
        busy_connection_map_.insert({this->seq_id_, client});
        free_connection_list_.pop_front();
    }
    return seq_id_ ;
}

/**************************************
 * this should pass in a seq_id , which is return when you Call SendProto().
 * this is rather important.
 */
int DBConnectionPool::Read(int seq_id, PDUBase &base){
    Busy_Connection_Map_t::const_iterator got = busy_connection_map_.find (seq_id);

    if ( got == this->busy_connection_map_.end() ){
        LOGD("got = this->busy_connection_map_.end()");
        return -1 ;
    }
    BlockTcpClient *client =  got->second ;
    int val = client->Read(base);
    busy_connection_map_.erase(seq_id);

    {   //keep this bracket. it's rather important.
        std::lock_guard<std::mutex> lck (mutex_);
        this->free_connection_list_.push_back(client);
    }   //keep this bracket. it's rather important.

    return val ;
}



int DBConnectionPool::Init_Pool(std::string _ip, int _port, int _num){
    this->db_proxy_ip_ = _ip ;
    this->db_proxy_port_ = _port ;

    for(int i = 0 ;i<_num; ++i){
        BlockTcpClient *client = new BlockTcpClient();
        client->Connect(this->db_proxy_ip_.c_str(), this->db_proxy_port_);
        this->free_connection_list_.push_back(client);
    }

}
