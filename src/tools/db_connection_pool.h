/***********************
 * @xiesong
 * songtzu@126.com
 * 5.6.2016
 * this is database proxy connection pool.
 * this is thread safe class.
 */
#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include "block_tcp_client.h"
#include "pdu_base.h"
#include <list>
#include <string>
#include <google/protobuf/message.h>
#include <unordered_map>
#include <mutex>

typedef std::unordered_map<int, BlockTcpClient*> Busy_Connection_Map_t ;
class DBConnectionPool
{
public:
    DBConnectionPool();

    ~DBConnectionPool();

    int SendProto(::google::protobuf::Message &_msg,  int _commandid, bool _has_read = true);

    /**************************************
     * this should pass in a seq_id , which is return when you Call SendProto().
     * this is rather important.
     */
    int Read(int seq_id, PDUBase &base);

    int Init_Pool(std::string _ip, int _port, int _num);

private:


    BlockTcpClient* AllocatConnection();
    // FreeConnection(BlockTcpClient*);


    Busy_Connection_Map_t busy_connection_map_ ;

    std::list<BlockTcpClient*> free_connection_list_ ;

    int seq_id_ ;

    std::string db_proxy_ip_;
    int db_proxy_port_ ;

    std::mutex mutex_ ;

};

#endif // DBCONNECTIONPOOL_H
