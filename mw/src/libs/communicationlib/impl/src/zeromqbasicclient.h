#ifndef ZEROMQBASICCLIENT_H
#define ZEROMQBASICCLIENT_H

#include<zmq.hpp>
#include<QString>

class ZeroMqBasicClient
{
public:
    ZeroMqBasicClient(zmq::context_t &ctx, QString const &address, quint8 socketType);
    virtual ~ZeroMqBasicClient(){}
    virtual void send(QByteArray const &data) const=0;
    static void deleteData(void *data,void *hint);

protected:
    mutable zmq::socket_t s;

};

#endif // ZEROMQBASICCLIENT_H
