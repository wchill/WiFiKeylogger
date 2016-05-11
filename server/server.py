from twisted.internet.protocol import Factory
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor

import sys
import datetime

class Tee(object):
    def __init__(self, name, mode):
        self.f= open(name, mode)
        self.stdout = sys.stdout
        sys.stdout = self
    def __del__(self):
        sys.stdout = self.stdout
        self.f.close()
    def write(self, data):
        self.f.write(data)
        self.stdout.write(data)
    def flush(self):
        self.f.flush()
        self.stdout.flush()

class EchoServer(LineReceiver):

    def __init__(self, factory):
        self.factory = factory
        self.peer = ''

    def lineReceived(self, data):
        print('[{}] <{}>: {}'.format(datetime.datetime.now(), self.peer, data))
        for peer in self.factory.clients:
            if peer != self:
                peer.message(data)

    def connectionMade(self):
        self.peer = self.getPeerString()
        print('[{}] Client <{}> connected'.format(datetime.datetime.now(), self.peer))
        self.factory.clients.append(self)

    def connectionLost(self, reason):
        print('[{}] Client <{}> disconnected'.format(datetime.datetime.now(), self.peer))
        self.factory.clients.remove(self)
        self.peer = ''

    def getPeerString(self):
        peer = self.transport.getPeer()
        return '{0}:{1}'.format(peer.host, peer.port)

    def message(self, message):
        self.transport.write(message + '\n')

class EchoServerFactory(Factory):

    protocol = EchoServer

    def __init__(self):
        self.clients = []

    def startFactory(self):
        print('Opening log at {}'.format(datetime.datetime.now()))

    def stopFactory(self):
        print('Closing log at {}'.format(datetime.datetime.now()))
        self.log.flush()
        self.log.close()

    def buildProtocol(self, addr):
        return EchoServer(self)

def main():
    t = Tee('log.txt', 'a')
    reactor.listenTCP(31337, EchoServerFactory())
    reactor.listenTCP(31338, EchoServerFactory())
    print('Control server running on 31337')
    print('Data server running on 31338')
    reactor.run()

if __name__ == '__main__':
    main()
