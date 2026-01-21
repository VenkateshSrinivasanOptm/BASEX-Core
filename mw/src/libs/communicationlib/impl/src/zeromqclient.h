#ifndef ZEROMQCLIENT_H
#define ZEROMQCLIENT_H

#include"zeromqbasicclient.h"


class ZeroMqClient: public ZeroMqBasicClient
{
public:
    ZeroMqClient(zmq::context_t &ctx, QString const &address);
    virtual ~ZeroMqClient(){}
    void send(QByteArray const &data) const;
};

#endif // ZEROMQCLIENT_H
