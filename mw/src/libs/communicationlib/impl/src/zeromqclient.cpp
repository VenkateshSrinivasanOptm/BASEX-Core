#include "zeromqclient.h"
#include "zeromqqbytearray.h"

#include <QDebug>


ZeroMqClient::ZeroMqClient(zmq::context_t &_ctx, QString const &_address):
        ZeroMqBasicClient(_ctx,_address,ZMQ_PUSH)
{
}


void ZeroMqClient::send(QByteArray const &data) const
{


    zmq::message_t msg((void *)(data.data()),data.size(),deleteData,
                       static_cast<void*>(new ZeroMqQByteArray(data)));
    try {
      s.send(msg);
   }
   catch(zmq::error_t t)
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << "error send request : "
                 << QString(t.what());
    }
}


