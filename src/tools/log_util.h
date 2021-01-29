#ifndef H_LOG_UTIL_H
#define H_LOG_UTIL_H
#include "pdu_base.h"
enum LogLevel{

    Level_Debug ,
    Level_Trace ,
    Level_Info ,
    Level_Warning ,
    Level_Error ,
    Level_Fatal ,

} ;

void LogImp(  LogLevel l,  const char*file, int line, const  char*msg, ...) ;

void log_setlevel(LogLevel);

// #define LOGD(x) LogImp(Level_Debug, __FILE__, __LINE__, x)

// #define LOGT(x) LogImp(Level_Trace, __FILE__, __LINE__, x);

void print_pdu(PDUBase &_pdu);

#define LOGD(x, ...) LogImp(Level_Debug, __FILE__, __LINE__,  x , ## __VA_ARGS__)

#define LOGT(x, ...) LogImp(Level_Trace, __FILE__, __LINE__,  x , ## __VA_ARGS__)

#endif // H_LOG_UTIL_H
