#include"zeromqincommingmessage.h"
#include"cbuscallbackintf.h"
#include"definitions.h"

#include<QDataStream>
#include<QDebug>
#include<performance.h>

ZeroMqIncommingMessage::ZeroMqIncommingMessage(CBusCallBackIntf &_cbus):
    cbus(_cbus)
{

}

/**
  * the format of an incomming message is
  * 1. Register :    register rservice rmodulename
  * 2. Unregister:  unregister rservice
  * 3. noreplycall: noreply service data
  * 4. async:  async  service rmodulename id data
  * 5. reply: reply id data
  * 6. registerservices: registerservices services
  * 7. getservices: getservices rmodulename
  **/

void ZeroMqIncommingMessage::handleZmqIncommingMessage(QByteArray const &data)
{
#ifdef DEBUG
    qDebug() << " message size " << data.size();
#endif
    QDataStream stream(data);

    quint8 requestType;
    quint8 service;
    QString moduleName;
    quint16 requestId;
    stream >> requestType;



    switch(requestType)
    {
    case REGISTER:

        stream >> service >> moduleName;
        cbus.registerExternalService(service,moduleName);
        break;

    case UNREGISTER:
        stream >> service;
        cbus.unregisterExternalService(service);
        break;

    case NOREPLYCALL:
        TRACE("CBUS: start incomming call with no reply");
        stream >> service;
        cbus.inCommingNoReplyCall(service,stream);
        TRACE("CBUS: end incomming call with no reply");
        break;

    case EXTERNALASYNCALL:
        TRACE("CBUS: start incomming call with reply");
        stream >> service >> moduleName >> requestId;
#ifdef DEBUG
        qDebug() << " service " << service << " module " << moduleName << "id " << requestId;
#endif
        cbus.inCommingAsyncCall(moduleName, service,requestId,stream);
        TRACE("CBUS: end incomming call with reply");
        break;

    case EXTERNALREPLY:
        TRACE("CBUS: start incomming reply");
        stream >> requestId;
        cbus.inCommingReply(requestId,stream);
        TRACE("CBUS: end incomming reply");
        break;

    case REGISTERSERVICES:
    {
        QList<quint8> services;
        stream >> moduleName >> services;
#ifdef DEBUG
        qDebug() << "incomming services from module " << moduleName;
#endif
        cbus.inCommingServices(moduleName,services);
        break;
    }

    case GETSERVICES:
        stream >> moduleName;
        cbus.inCommingGetServices(moduleName);
        break;

    default:
        //TODO log error
        break;
    }
}

void ZeroMqIncommingMessage::handleZmqIncommingPublishedMessage(QByteArray const &data)
{

    TRACE("CBUS: incomming publish event");
    QDataStream stream(data);
    cbus.inCommingPublish(stream);
}
