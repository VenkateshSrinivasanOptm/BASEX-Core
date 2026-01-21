#include "request.h"

#include <cbusinterface.h>
#include <comlibservices.h>
#include <OCIMessages.h>
#include <ociwritemessages.h>
#include <pacmanmessages.h>
#include <synchronizeddatamessage.h>
#include <mmddf_writer_startup_messages.h>
#include <startupserviceprivate.h>
#include <brokerdocument.h>

#include<QDebug>

Request::Request():
        cbus(CBusInterface::instance())
{
     cbus.registerService(MMDDF_WRITER_SERV,this,this);
     cbus.registerService(OCI_SERV,this,this);
     cbus.registerService(OCI_WRITER_SERV,this,this);
     cbus.registerService(DTV_WRITER_SERV,this,this);
     cbus.registerService(PACKAGE_MANAGER_SERV,this,this);
     QList<quint16> types;
     types.append(BLSTARTUP_INTIALIZATION);
     cbus.subscribe(this,types);
}

Request::~Request()
{
    cbus.unregisterService(MMDDF_WRITER_SERV);
    cbus.unregisterService(OCI_SERV);
    cbus.unregisterService(OCI_WRITER_SERV);
    cbus.unregisterService(DTV_WRITER_SERV);
    cbus.unregisterService(PACKAGE_MANAGER_SERV);
    cbus.unsubscribe(this);
}

bool Request::messageHandler(quint16 requestId, MessagePtr message)
{
   switch(message->messageType)
   {
    case MMDDF_WRITER_SERV_GET_BROKER_DOC_SYNCHRONOUS_REQ:
      qDebug() << __FUNCTION__ << " we have a MMDDF_WRITER_SERV_GET_BROKER_DOC_SYNCHRONOUS_REQ";
      if(step==1)
      {
          step=2;
          cbus.sendReply(requestId, MessagePtr( new GetBrokerDocumentsSynchronousResp(GetBrokerDocumentsSynchronousResp::GBDR_SUCCESS)));
          return true;
      }
      error(__LINE__);
      return true;
    case OCI_SERV_AUTHENTICATE_REQ:
      qDebug() << __FUNCTION__ << " we have a OCI_SERV_AUTHENTICATE_REQ";
      if(step==2)
      {
        cbus.sendReply(requestId, MessagePtr( new AuthenticateResp()));
        step=3;
        return true;
      }
      error(__LINE__);
      return true;
    case  OCI_WRITER_SERV_GET_SUBSCRIBER_INFO:
      qDebug() << __FUNCTION__ << " we have a OCI_WRITER_SERV_GET_SUBSCRIBER_INFO";
      if(step==3)
      {
       step=4;
       cbus.sendReply(requestId, MessagePtr( new SubscriberInfoResponse("",200)));
       return true;
      }
      error(__LINE__);
      return true;
    case PACKAGE_MANAGER_SERV_CHECK_FOR_NEW_SW_REQ:
      qDebug() << __FUNCTION__ << " we have a PACKAGE_MANAGER_SERV_CHECK_FOR_NEW_SW_REQ";
      if(step==4)
      {
          step=5;
          cbus.sendReply(requestId, MessagePtr( new CheckForNewSTBSwResponse(false)));
          return true;
      }
      error(__LINE__);
      return true;
    case BLSTARTUP_SYNCHRONIZED_UNICAST_DOCUMENTS_REQ:
      qDebug() << __FUNCTION__ << " we have a BLSTARTUP_SYNCHRONIZED_UNICAST_DOCUMENTS_REQ";
      if(step==5 || step==6)
      {
          step=6;
          cbus.sendReply(requestId, MessagePtr( new SynchronizedDataResp(SynchronizedDataResp::NO_ERROR)));
          return true;
      }
      error(__LINE__);
      return true;
    default:
      return false;
  }
}

bool Request::messageHandler(MessagePtr message)
{
}

MessagePtr Request::deserialize(quint16 type, QDataStream &stream)
{
    return MessagePtr();
}

MessagePtr Request::subscriberDeserialize(quint16 messageType, QDataStream &stream)
{
    return MessagePtr();
}

bool Request::subscriberMessageHandler(MessagePtr message)
{
    QSharedPointer<InitializationMessage const> resp=message.staticCast<InitializationMessage const>();
    qDebug() << __FUNCTION__ << " incoming event " << resp->type;
    switch(resp->type)
    {
        case InitializationMessage::CONFIGURED:
          if(configured)  error(__LINE__);
          else configured=true;
          return true;

        case InitializationMessage::DTV_LINEUP_READY:
          if(dtvlineup)  error(__LINE__);
          else dtvlineup=true;
          return true;

        case InitializationMessage::EPG_READY:
          qDebug() << __FUNCTION__ <<" epg ready";
          if(epg)  error(__LINE__);
          else epg=true;
          return true;
        default:
          return false;
     }
}


void Request::error(quint16 line)
{
    QString s(QString::number(line));
    QFAIL(s.toStdString().c_str());
}

void Request::test()
{
    QTest::qWait(2000);
    configured = false;
    epg=false;
    dtvlineup=false;
    StartupServicePrivate st;
    step=1;
    QList<QByteArray> services;
    QList<ServicesList> documents;
    services << DTV_SERV_NAME;
    st.init(services,documents,24);
    QTest::qWait(5000);
    qDebug() << " steps " << step;
    QVERIFY(step==6);
    qDebug() << " configured " << configured;
    QVERIFY(configured);
    qDebug() << " epg " << epg;
    QVERIFY(epg);
    qDebug() << " dtvlineup " << dtvlineup;
    QVERIFY(dtvlineup);
}
