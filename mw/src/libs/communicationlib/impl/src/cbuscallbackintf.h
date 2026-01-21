#ifndef CBUSCALLBACKINTF_H
#define CBUSCALLBACKINTF_H

#include"message.h"

#include<QByteArray>
#include<QString>


/**
  * this class defines the methods that has to be called when a message is received from another module
  **/
class CBusCallBackIntf
{
  public:
    CBusCallBackIntf(){}
    virtual ~CBusCallBackIntf(){}
    virtual void registerExternalService(quint8 service, QString const &moduleName)=0;
    virtual void unregisterExternalService(quint8 service)=0;
    virtual void inCommingNoReplyCall(quint8 service, QDataStream &stream)=0;
    virtual void inCommingAsyncCall(QString const &rmodule, quint8 service, quint16 requestId, QDataStream &stream)=0;
    virtual void inCommingReply(quint16 requestId, QDataStream &stream)=0;
    virtual void inCommingPublish(QDataStream &stream)=0;
    virtual void inCommingGetServices(QString const &moduleName)=0;
    virtual void inCommingServices(QString const &moduleName, QList<quint8> services)=0;
};

#endif // CBUSCALLBACKINTF_H
