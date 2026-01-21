#ifndef ZEROMQPUBLISHSERVER_H
#define ZEROMQPUBLISHSERVER_H

#include "zeromqbasicclient.h"

class ZeroMqPublishServer:public ZeroMqBasicClient
{
public:
    ZeroMqPublishServer(QString const &_address, zmq::context_t &_ctx);
    void send(QByteArray const &data) const;

};

#endif // ZEROMQPUBLISHSERVER_H
