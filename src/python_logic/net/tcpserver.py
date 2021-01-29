#!/usr/bin/env python

# author@xiesong
# songtzu@126.com
# 4.17.2016
from struct import pack, unpack
from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor
from ProtobufProtocol import ProtobufProtocol
import sys
### Protocol Implementation




class DispacthRoute(ProtobufProtocol):
    def OnDispatchControler(self, commandid, seqid, packbuf):
        print(commandid,seqid)
        #user = User_pb2.User()
        #user.ParseFromString(packbuf)
        #print (user.__str__(),"the dict is ")
        #control = Controller()
        #control.SaveTestUser(user)
        #print user

def main():
    f = Factory()
    f.protocol = DispacthRoute
    reactor.listenTCP(8000, f)
    reactor.run()

if __name__ == '__main__':
    main()