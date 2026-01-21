#include "request2.h"

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

Request2::Request2():
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

bool Request2::messageHandler(quint16 requestId, MessagePtr message)
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
        ErrorResp * resp = new ErrorResp();
        resp->m_ErrorType=OCI_ERROR_TYPE_OTHER;
        resp->m_ErrorInfo.errorCode=-1602;
        cbus.sendReply(requestId, MessagePtr( resp));
        step=3;
        return true;
      }
      error(__LINE__);
      return true;
    default:
      return false;
  }
}

bool Request2::messageHandler(MessagePtr message)
{
}

MessagePtr Request2::deserialize(quint16 type, QDataStream &stream)
{
    return MessagePtr();
}

MessagePtr Request2::subscriberDeserialize(quint16 messageType, QDataStream &stream)
{
    return MessagePtr();
}

bool Request2::subscriberMessageHandler(MessagePtr message)
{
    QSharedPointer<InitializationMessage const> resp=message.staticCast<InitializationMessage const>();
    qDebug() << __FUNCTION__ << " incoming event " << resp->type;
    switch(resp->type)
    {
        case InitializationMessage::UNCONFIGURED:
          if(unconfigured)  error(__LINE__);
          else unconfigured=true;
          return true;

        case InitializationMessage::START:
          return true;
        default:
          error(__LINE__);
          return false;
     }
}


void Request2::error(quint16 line)
{
    QString s(QString::number(line));
    QFAIL(s.toStdString().c_str());
}

void Request2::test()
{
    QTest::qWait(2000);
    unconfigured = false;
    StartupServicePrivate st;
    step=1;
    QList<QByteArray> services;
    QList<ServicesList> documents;
    services << DTV_SERV_NAME;
    st.init(services,documents,24);
    QTest::qWait(5000);
    qDebug() << " steps " << step;
    QVERIFY(step==3);
    qDebug() << " unconfigured " << unconfigured;
    QVERIFY(unconfigured);
}

