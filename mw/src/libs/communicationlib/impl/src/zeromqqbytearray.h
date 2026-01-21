#ifndef ZEROMQQBYTEARRAY_H
#define ZEROMQQBYTEARRAY_H

#include<QByteArray>

/**
  * this struct is used for sending data over 0mq using Zero-copy
  **/

struct ZeroMqQByteArray
{
    ZeroMqQByteArray(QByteArray const &_data): data(_data) {}
    QByteArray data;
};

#endif // ZEROMQQBYTEARRAY_H
