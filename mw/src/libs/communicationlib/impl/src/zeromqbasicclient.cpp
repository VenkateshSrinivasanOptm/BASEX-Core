#include "zeromqbasicclient.h"
#include "zeromqqbytearray.h"

#include <QDebug>
#include <iostream>

ZeroMqBasicClient::ZeroMqBasicClient(zmq::context_t &ctx, QString const &address, quint8 socketType):
    s(ctx,socketType)
{
    switch(socketType)
    {
    case ZMQ_PUB:
        qDebug() << "bind to address" << address;
        s.bind(address.toLatin1().data());
        break;
    case  ZMQ_PUSH:
        qDebug() << "connect to address " << address;
        s.connect(address.toLatin1().data());
        break;
    }
}


void ZeroMqBasicClient::deleteData(void *data, void *qbytedata)
{
#ifdef DEBUG
    qDebug() << " delete message";
#endif
    delete static_cast<ZeroMqQByteArray*>(qbytedata);
}

