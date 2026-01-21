#ifndef SENDOBJECT_H
#define SENDOBJECT_H

#include "message.h"

#include<QSharedPointer>

struct SendObject
{
   quint8 type;
   QString module;
   quint8 serviceName;
   MessagePtr msg;
   quint16 requestId;

   SendObject(quint8 _type, QString const &_module, quint8 _serviceName, MessagePtr const &_msg,quint16 _requestId):
           type(_type), module(_module), serviceName(_serviceName), msg(_msg), requestId(_requestId) {}
   SendObject(quint8 _type, QString const &_module, quint8 _serviceName, MessagePtr const &_msg):
           type(_type), module(_module), serviceName(_serviceName), msg(_msg), requestId(0) {}
   SendObject(quint8 _type,MessagePtr const &_msg):
           type(_type), module(QString()), serviceName(0), msg(_msg), requestId(0) {}
   SendObject(quint8 _type, QString const &_module, MessagePtr const &_msg, quint16 _requestId):
           type(_type), module(_module), serviceName(0), msg(_msg), requestId(_requestId) {}
   SendObject(quint8 _type,quint8 _serviceName):
           type(_type), module(QString()), serviceName(_serviceName), msg(MessagePtr()), requestId(0) {}
   SendObject(quint8 _type,quint8 _serviceName, MessagePtr const &_msg, quint16 _requestId):
           type(_type), module(QString()), serviceName(_serviceName), msg(_msg), requestId(_requestId) {}
   SendObject(quint8 _type,MessagePtr const &_msg, quint16 _requestId):
           type(_type), module(QString()), serviceName(0), msg(_msg), requestId(_requestId) {}
   SendObject(quint8 _type): type(_type) {}

   virtual ~SendObject() {}
};

typedef QSharedPointer<SendObject const> SendObjectPtr;

#endif // SENDOBJECT_H
