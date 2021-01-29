#!/usr/bin/env python

# author@xiesong
# songtzu@126.com
# 4.18.2016

from __future__ import print_function

from twisted.internet import task
from twisted.internet.defer import Deferred
from twisted.internet.protocol import Protocol, ClientFactory
from twisted.protocols.basic import LineReceiver

from ProtobufProtocol import ProtobufProtocol
import sys
import User_pb2



class DispacthRoute(ProtobufProtocol):

    def OnDispatchControler(self, commandid, seqid, packbuf):
        print (commandid, "\n", seqid, "\n", packbuf)
        print ("OnDispath")

    def connectionMade(self):
        print ("connectionMade")
        #self.transport.write("dhello shit.")
        user = User_pb2.User()
        user.user_name = "jackson"
        user.user_id = 1
        user.email = "songtzu@mail.com"
        print(user.SerializeToString())
        self.SendProtobuf(11,222,333,user)


class ClientFactory(ClientFactory):
    protocol = DispacthRoute

    def __init__(self):
        self.done = Deferred()

    def clientConnectionFailed(self, connector, reason):
        print('connection failed:', reason.getErrorMessage())
        self.done.errback(reason)

    def clientConnectionLost(self, connector, reason):
        print('connection lost:', reason.getErrorMessage())
        self.done.callback(None)

    def send(self, command_id, seq_id, proto):
        self.protocol.SendProtobuf(command_id, seq_id, proto)


class ClientBase():
    factory = ClientFactory()

    def __init__(self, ip , port):
        self.__ip = ip
        self.__port = port
        task.react(self.Connect(), self)

    def connect(self, reactor):
        reactor.connectTCP(self.__ip, self.__port, self.factory)


def main(reactor):
    factory = ClientFactory()
    reactor.connectTCP('192.168.0.146', 8001, factory)
    print ("dwdoadoli")
    return factory.done

# def main(reactor):
    # factory =

if __name__ == '__main__':
    task.react(main)
    print( "ddddd")

    #client = ClientBase('192.168.0.146', 8001)