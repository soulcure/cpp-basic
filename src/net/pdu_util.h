#ifndef PDUUTIL_H
#define PDUUTIL_H

#include "pdu_base.h"


class PduUtil
{
public:
    PduUtil();


    /***********************************************************
     * @PDU解析
     * if, default pdu did not met your requirement, override this function.
     * if, error format, return -1.
     * if, has package, return readed package.
     * if, does not have a full package, return 0.
     * if, IT'S OVERRIDED, OnPduPack also need to be override.
     */
    virtual int OnPduParse(char *_buf, int _length, PDUBase &_base /*return value*/ );


    /***********************************************************
     * @pdu封包,
     */
    virtual int OnPduPack(PDUBase &_base, char *&buf);

    int Set_Userid( char* _buf, int _userid);

};

#endif // PDUUTIL_H
