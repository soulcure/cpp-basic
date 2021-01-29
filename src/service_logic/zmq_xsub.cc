#include "zmq_xsub.h"
#include "YouMai.Basic.pb.h"
#include "YouMai.User.pb.h"
#include "log_util.h"

using namespace com::proto::basic;
using namespace com::proto::user;

Zmq_XSub::Zmq_XSub()
{

}

void Zmq_XSub::OnSubscribe(Topic_t _topic, PDUBase &_pdubase ){

    LOGT("OnSubscribe call back.\n");
    LOGT("%s", _topic.c_str());
    print_pdu(_pdubase);

    switch (_pdubase.command_id) {
        case 100001:
        {

            Heart_Beat heart ;
            heart.ParseFromArray(_pdubase.body.get(), _pdubase.length );
            //LOGD("HEART BEAT %d", heart.user_id());

            Heart_Beat_Ack ack;
            //ack.set_user_id(heart.user_id());
            _pdubase.command_id = HEART_BEAT_ACK ;
            std::shared_ptr<char> new_body (new char[ack.ByteSize()]);
            ack.SerializeToArray(new_body.get(), ack.ByteSize());
           // memcpy(new_body.get(), ack.SerializeToArray(new_body.get(), ack.ByteSize()), ack.ByteSize());
            _pdubase.body = new_body ;
           // Send(sockid, _pdubase) ;
        }
        break;
    }
}
