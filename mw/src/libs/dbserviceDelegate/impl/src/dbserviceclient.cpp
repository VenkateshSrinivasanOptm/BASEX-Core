#include "dbserviceclient.h"
#include <QDebug>
#include <callbackgen.h>
#include <comlibservices.h>

DbServiceClient::DbServiceClient() : cbus(CBusInterface::instance()) {}

MessagePtr DbServiceClient::subscriberDeserialize(quint16 messageType,
                                                  QDataStream &stream) {
  qDebug() << __FUNCTION__ << " incoming message " << messageType;
  Message *message(0);
  switch (messageType) {
  // TODO
  default:
    break;
  }

  if (message == NULL) {
    qDebug() << __FUNCTION__
             << " DID YOU NOT FORGET TO IMPLEMENT THE DESERIALIZE FOR THIS "
                "TYPE OF MSG ? ";
    return MessagePtr();
  } else {
    message->deserialize(stream);
    return MessagePtr(message);
  }
}

bool DbServiceClient::callback(quint16 requestId, MessagePtr reply) {
  qDebug() << __FUNCTION__ << " callback received for requestId " << requestId
           << ", message type " << reply->messageType;

  switch (reply->messageType) {
  // TODO return true if messageType is captured
  default:
    break;
  }
  return false;
}

MessagePtr DbServiceClient::deserialize(quint16 messageType,
                                        QDataStream &data) {
  qDebug() << __FUNCTION__ << " starting deserialising for message type "
           << messageType;

  Message *message(0);
  switch (messageType) {
  // TODO create response andassign to message
  default:
    break;
  }
  if (message == NULL) {
    qDebug() << __FUNCTION__
             << " DID YOU NOT FORGET TO IMPLEMENT THE DESERIALIZE FOR THIS "
                "TYPE OF MSG ? ";
    return MessagePtr();
  } else {
    message->deserialize(data);
    return MessagePtr(message);
  }
}

bool DbServiceClient::subscriberMessageHandler(MessagePtr message) {
  qDebug() << __FUNCTION__ << " incoming message " << message->messageType;
  switch (message->messageType) {
    // TODO
  default:
    break;
  }
  return false;
}

void DbServiceClient::writeConfigData(const QString &data) {
  // send message to db service.
  qDebug() << __FUNCTION__ << " sending tune message over cbus";
  // Message *message = new WriteCANDataRequest(data);
  QSharedPointer<CallBack> callBack =
      QSharedPointer<CallBack>(new CallBackGen<DbServiceClient>(
          this, &DbServiceClient::callback, &DbServiceClient::deserialize));
  // quint16 resultr =
      // cbus.callServiceWithCallback(DB_SERV, MessagePtr(message), callBack);
  // qDebug() << __FUNCTION__ << " cbus returns: " << resultr;
}
