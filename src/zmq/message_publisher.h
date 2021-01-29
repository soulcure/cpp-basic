/***********************************************************'
 * author@xiesong
 * songtzu@126.com
 * 4.15.2016.
 */
#ifndef MESSAGE_PUBLISHER_H
#define MESSAGE_PUBLISHER_H

#include "pdu_base.h"
#include "pdu_util.h"
#include "typedef.h"
#include <zmq.hpp>

class MessagePublisher :public PduUtil
{
public:
    MessagePublisher() {}
    /***************************************
     * begin puber.
     * _url:the url for pub.
     */
    int ConnectPub(const char * _url) ;

    /**********************
     * this function will build the topic from pdubase.
     * implement by songtzu@126.com
     * xiesong
     */
    int PublishMessage(PDUBase &_base);

    int PublishMessage(Topic_t _topic, PDUBase &_base);

    int PublishMessage(Topic_t _topic, void *_stream, int _length);

private:
    zmq::socket_t *pub_sock_ ;

    zmq::context_t *context_ ;

};


#endif // MESSAGE_PUBLISHER_H
