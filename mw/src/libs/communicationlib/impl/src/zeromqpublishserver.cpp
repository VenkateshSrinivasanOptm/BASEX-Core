#include "zeromqpublishserver.h"
#include "zeromqqbytearray.h"

#include<QDebug>

ZeroMqPublishServer::ZeroMqPublishServer(QString const &_address, zmq::context_t &_ctx):
    ZeroMqBasicClient(_ctx,_address,ZMQ_PUB)
{    
}

void ZeroMqPublishServer::send(const QByteArray &data) const
{
#ifdef DEBUG
    qDebug() << "push a message using 0mq message size " << data.size();
#endif

    zmq::message_t msg((void *)(data.data()),data.size(),deleteData,
                       static_cast<void*>(new ZeroMqQByteArray(data)));
    s.send(msg);
}

