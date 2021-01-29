#include "pdu_util.h"
#include "log_util.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <cstring>
#include <arpa/inet.h>

PduUtil::PduUtil()
{
}



int PduUtil::Set_Userid( char* _buf, int _userid){
    int user_id_offset = 4;
    int terminal_token = htonl(_userid);
    memcpy(_buf+user_id_offset,(char*)(&terminal_token), sizeof(int));
//    char *buf = _buf ;
//    int *startflag = (int *)buf;
//    buf += sizeof(int);
//    int *terminal_token = (int*)buf ;
    //_base.terminal_token = ntohl(*terminal_token);

}



/***********************************************************
 * @PDU解析
 *  see the define in file
 *  pdu_base.h
 *
 */
int PduUtil::OnPduParse(char *_buf, int _length, PDUBase &_base /*return value*/ ){
    char *buf = _buf ;
    int *startflag = (int *)buf;
    //printf("pin %d \n", *pin);

    if (_length <= 0)
    {
        return 0;
    }
    if ( ntohl(*startflag) != PDUBase::startflag){
        LOGD("ntohl(*startflag) != PDUBase::startflag");
        return -1;
    }

    buf += sizeof(int);
    int *terminal_token = (int*)buf ;
    _base.terminal_token = ntohl(*terminal_token);

    buf += sizeof(int) ;
    int *commandid = (int*)buf ;
    _base.command_id = ntohl(*commandid) ;

    buf += sizeof(int);
    int *seqid = (int*)buf;
    _base.seq_id = ntohl(*seqid);

    buf += sizeof(int);
    int *length = (int *)buf;
    _base.length = ntohl(*length);


    buf += sizeof(int);

    //not a full package.
    if(buf-_buf +_base.length > _length)
        return 0;

    std::shared_ptr<char> pbody( new char[ _base.length + 1 ] );
    memcpy(pbody.get(), buf , _base.length);
    _base.body = pbody ;


    return buf-_buf +_base.length ;
}







/****************************************************
 *  const static int startflag = 88888888 ;
    int command_id ;
    int seq_id;
    int length ;
    std::shared_ptr<char> body ;

 */
int PduUtil::OnPduPack(PDUBase &_base, char *&outbuf /*this is return value*/){



    int total_len = 0 ;

    int startflag = htonl( PDUBase::startflag );
    total_len += sizeof(int) ;
    int terminal_token = htonl(_base.terminal_token);
    total_len += sizeof(int) ;
    int commandid = htonl(_base.command_id);
    total_len += sizeof(int);
    int seq= htonl( _base.seq_id );
    total_len += sizeof(int);
    int proto_len = htonl( _base.length );
    total_len += sizeof(int);

    total_len += _base.length ;
    char *buf = (char*)malloc(total_len);
    if(buf==NULL)
        return -1;

    int offset = 0;
    memcpy(buf+offset,(char*)(&startflag),sizeof(int) );
    offset += sizeof(int);
    memcpy(buf+offset,(char*)(&terminal_token), sizeof(int));
    offset += sizeof(int);
    memcpy(buf+offset, (char*)(&commandid), sizeof(int) );
    offset += sizeof(int);
    memcpy(buf+offset,(char*)(&seq),sizeof(int) );
    offset += sizeof(int);
    memcpy(buf+offset, (char*)(&proto_len), sizeof(int) );
    offset += sizeof(int);
    memcpy(buf+offset, _base.body.get(),  _base.length  );
    //offset +=  _base.length ;
    //memcpy(buf+offset, (char*)(&endflag), sizeof(int) );
    outbuf = buf;

    LOGT(buf);

    return total_len;

}




