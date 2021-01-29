#ifndef MAIN_H
#define MAIN_H
#include "connection_server.h"
#include "ThreadPool.h"
#include "db_connection_pool.h"


typedef std::unordered_map<Userid_t, ClientObject> User_Connect_Map_t ;

//extern User_Connect_Map_t    connect_user_map;

#define CONNECTPOOL_AND_THREADPOOL_NUMBER 20

extern ConnectionServer connect_server ;
extern DBConnectionPool db_connect_pool;
extern ThreadPool thread_pool;



#endif
