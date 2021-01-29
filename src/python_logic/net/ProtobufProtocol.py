from struct import pack, unpack
from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor


class ProtobufProtocol(Protocol):
    _buffer = b""
    _headsize = 4*4
    _start = 123456789
    def dataReceived(self, data):
        self._buffer = self._buffer + data
        print ("recieved info")
        while True:
            if len(self._buffer) >= 4:
                head, = unpack(">I",self._buffer[0:4])
                if (head!=self._start):
                    self._buffer = b""
                    print("don't has right head", head)
                    return
                token, = unpack(">I", self._buffer[4:8])

                commandid, = unpack(">I", self._buffer[8:12])
                print ("commandid get ", commandid)
                if(commandid==1):
                    print ("commandid is 1",commandid)

                seqid, =  unpack(">I", self._buffer[12:16])
                length, = unpack(">I", self._buffer[16:20])
                if len(self._buffer)>= length + self._headsize:
                    # has a full pack.
                    pack_buf = self._buffer[20:20+length]
                    print ("len is ", len(pack_buf))
                    self._buffer = self._buffer[20+length:]
                    print ("full pack")
                    self.OnDispatchControler(commandid, seqid, pack_buf)



    def OnDispatchControler(self, commandid, seqid, packbuf):
        print (commandid, "\n", seqid, "\n", packbuf)
        raise NotImplementedError


    def SendProtobuf(self,terminalid, commandid, seqid, proto):
        buffer = pack(">I",self._start) + pack(">I", terminalid) + pack(">I", commandid) + pack(">I", seqid) +pack(">I", proto.ByteSize()) + proto.SerializeToString()
        print (buffer,"length is \n", len(buffer))
        self.transport.write(buffer);