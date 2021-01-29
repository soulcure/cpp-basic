#include "log_util.h"
#include <stdio.h>
#include <stdarg.h>

#define BUF_MAX 1024

int vs_log_stub(LogLevel l,  const char*file, int line, const char* msg, va_list ap)
{
    int bytes = 0;
    char data[BUF_MAX] = "";

    bytes += snprintf(data+bytes, BUF_MAX-1-bytes, "%02d; %04d; %s  \n",l , line, file);

    if (bytes < BUF_MAX-1)
    {
       bytes += vsnprintf(data+bytes, BUF_MAX-1-bytes, msg, ap);
    }

    printf( "%s\n", data);

    return 1;
}

void LogImp(LogLevel l,  const char*file, int line, const char* msg, ...) {
    va_list ap;
    int ret_val = 0;

    va_start(ap,msg);
    ret_val = vs_log_stub(l, file, line, msg, ap);
    va_end(ap);
  //  printf("file:%sline:%d %d  msg:%s\n", file, line, l,  msg);
}


void print_pdu(PDUBase &_pdu){
    printf("command_id is:%d,\nlength is:%d,\nseq_id is:%d,\nterminal_token is:%d\n", _pdu.command_id,
                                _pdu.length, _pdu.seq_id, _pdu.terminal_token);
}
