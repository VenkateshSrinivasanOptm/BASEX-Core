#include "startupserviceprivate.h"

#include<QDebug>

#include<cbusinterface.h>
#include<comlibservices.h>
#include<callbackgen.h>
//#include<OCIMessages.h>
#include<autoconfigmessage.h>
#include<documentavailablemessage.h>
#include<servicereadymessage.h>
#include<unassignedmessage.h>
#include <QTimer>

StartupServicePrivate::StartupServicePrivate():
       cbus(CBusInterface::instance()),
       autoConfigRequestId(0),
       allServicesInitialized(false), configured(false),
       rcInitialized(false), canServiceInitialized(false),
       dbServiceInitialized(false)
{

//   qRegisterMetaType< SynchronizedDataResp::Type>();
   cbus.registerService(STARTUP_SERV,this,this);
}

StartupServicePrivate::~StartupServicePrivate()
{
   cbus.unregisterService(STARTUP_SERV);
}

void StartupServicePrivate::init(QList<QByteArray> const &mwServiceList,
                                 QList<ServicesList> const &brokerDocumentList,
                                 quint16 _minEPGhrs)
{
  minEPGhrs = _minEPGhrs;
  brokerDocuments=brokerDocumentList;
  services=mwServiceList;
  qDebug() << "Madhur services " << services;
  publishInitialisationPoint(InitializationMessage::START);
  // publish a start

  //start download request for globalinstall and groups documents
  QList<QByteArray> documents;
  documents.append("GLOBALINSTALL");
  documents.append("GROUPS");
  //TODO :- send request to download config files
  qDebug() << __FUNCTION__ << ":" << __LINE__ << " call MMDDF_WRITER for download GLOBALINSTALL and GROUPS";
  //if(!cbus.callServiceWithCallback(DOWNLOAD_SERV, MessagePtr(new GetBrokerDocumentsSynchronousReq(documents)),
    //                          QSharedPointer<CallBack> (new CallBackGen<StartupServicePrivate>(this,
      //                                                       &StartupServicePrivate::getBrokerDocRespHandler,
        //                                                     &StartupServicePrivate::getBrokerDocRespDeserializer))))
  //{
    //  qDebug() << __FUNCTION__ << ":" << __LINE__ << " MMDDF_WRITER service not available ";
     // publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
  ///}
}

//slots
void StartupServicePrivate::documentAvailable(QByteArray document)
{
/*
    if(document==DTVLINEUP)
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << " DTVLINEUP  downloaded";
        publishInitialisationPoint(InitializationMessage::DTV_LINEUP_READY);
    }
    else
    if(document=="PIPEDSCHEDULE_small")
    {
         qDebug() << __FUNCTION__ << ":" << __LINE__ << " PIPEDSCHEDULE_small  downloaded";
         publishInitialisationPoint(InitializationMessage::EPG_READY);
    }
*/
}

void StartupServicePrivate::authenticate()
{
   qDebug() << __FUNCTION__ << ":" << __LINE__ <<  " call OCI SERVICE for authenticate ";
   /*TODO if(!cbus.callServiceWithCallback(OCI_SERV, MessagePtr(new Authenticate()),
                              QSharedPointer<CallBack> (new CallBackGen<StartupServicePrivate>(this,
                                                             &StartupServicePrivate::authenticateRespHandler,
                                                             &StartupServicePrivate::authenticateRespDeserializer))))
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << " OCI SERVICE not available ";
        publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
    }*/
}

void StartupServicePrivate::getSubscriberInfo()
{
    configured=true;
/*    qDebug() << __FUNCTION__ << ":" << __LINE__ <<  " call OCI WRITER for subscriberInfo" ;
    if(!cbus.callServiceWithCallback(OCI_WRITER_SERV, MessagePtr(new SubscriberInfoMessage()),
                              QSharedPointer<CallBack> (new CallBackGen<StartupServicePrivate>(this,
                                                             &StartupServicePrivate::subscriberInfoRespHandler,
                                                             &StartupServicePrivate::subscriberInfoRespDeserializer))))
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << " OCI_WRITER_SERV not available ";
        publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
    }
*/
}

void StartupServicePrivate::checkSWVersion()
{
    if(!configured)
    {
        qDebug() << __FUNCTION__ << __LINE__ << " stb is not configured, stop startup" ;
        return;
    }

    qDebug() << __FUNCTION__ << ":" << __LINE__ << " call PACKAGE MANAGER for SW update check ";
    /*if(!cbus.callServiceWithCallback(PACKAGE_MANAGER_SERV, MessagePtr(new CheckForNewSTBSwRequest()),
                              QSharedPointer<CallBack> (new CallBackGen<StartupServicePrivate>(this,
                                                             &StartupServicePrivate::checkSWVersionRespHandler,
                                                             &StartupServicePrivate::checkSWVersionRespDeserializer))))
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << " PACKAGE_MANAGER_SERV not available ";
        publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
    }*/
}

void StartupServicePrivate::autoConfig(QString pin, QString accountNbr, QString stbName)
{
   qDebug() << __FUNCTION__ << ":" << __LINE__ << " call OCI SERVICE for autoconfig";
/*   if(!cbus.callServiceWithCallback(OCI_SERV, MessagePtr(new GetAutoConfig(pin,accountNbr,stbName)),
                              QSharedPointer<CallBack> (new CallBackGen<StartupServicePrivate>(this,
                                                             &StartupServicePrivate::autoConfigRespHandler,
                                                             &StartupServicePrivate::autoConfigRespDeserializer))))
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << " OCI_SERV not available ";
        publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
    }
*/
}


void StartupServicePrivate::serviceInitialized(QByteArray serviceName)
{
   services.removeAll(serviceName);
   if(services.isEmpty())
   {
       // all services are initialized
       allServicesInitialized=true;
       qDebug() << __FUNCTION__ << ":" << __LINE__ <<  " initialization finished";
       publishInitialisationPoint(InitializationMessage::INITIALIZATION_FINISHED);
   }

   // check if we need to publish some READY events

   if(serviceName==CAN_SERV_NAME)
   {
       canServiceInitialized=true;
       publishInitialisationPoint(InitializationMessage::CAN_READY);
   }
   else
   if(serviceName==DB_SERV_NAME)
   {
       dbServiceInitialized=true;
       publishInitialisationPoint(InitializationMessage::DB_READY);
   }
}

/*
void StartupServicePrivate::retrievalUnicastResp(SynchronizedDataResp::Type error)
{
    if(!configured)
    {
        qDebug() << __FUNCTION__ << __LINE__ << " stb is not configured, stop startup" ;
        return;
    }

    if(error==SynchronizedDataResp::NO_ERROR)
    {
       switch(unicastService)
       {
         case DTV_WRITER_SERV:  
            publishInitialisationPoint(InitializationMessage::DTV_DATA_READY);
            dtvDataReady=true;
            if(rcInitialized && dtvServiceInitialized) publishInitialisationPoint(InitializationMessage::DTV_READY);
            break;
         case PVR_WRITER_SERV:
             publishInitialisationPoint(InitializationMessage::PVR_DATA_READY);
             pvrDataReady=true;
             if(rcInitialized && pvrServiceInitialized) publishInitialisationPoint(InitializationMessage::PVR_READY);
             break;
         case ON_DEMAND_WRITER_SERV:
             publishInitialisationPoint(InitializationMessage::OD_DATA_READY);
             odDataReady=true;
             if(rcInitialized && ondemandServiceInitialized) publishInitialisationPoint(InitializationMessage::OD_READY);
             break;
         case OCI_WRITER_SERV:
             publishInitialisationPoint(InitializationMessage::CONTACT_MANAGEMENT_READY);
             break;
         default:
             qDebug() << __FUNCTION__ << ":" << __LINE__ << " we have an unknown service to publish "
             << unicastService;
      }
    }
    else
    {
       qDebug() << __FUNCTION__ << ":" << __LINE__ << "we have a error during the unicast " << error;
                //TODO do we have to publsih an unexpected error and stop
    }

    if(!synchronizeServices.empty())
    {
          downloadUnicastDocument(); // download next unicast document
    }
    else
    {
          qDebug() << __FUNCTION__ << ":" << __LINE__ << "we download all the unicast documents";
    }
}
*/
void StartupServicePrivate::swCheckReceived(bool packageToInstall)
{
    if(packageToInstall)
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << " we have a SW update, stop startup ";
        publishInitialisationPoint(InitializationMessage::SW_UPDATE);
    }
    else
    {
        if(!configured)
        {
        qDebug() << __FUNCTION__ << __LINE__ << " stb is not configured, stop startup" ;
        return;
        }

        qDebug() << __FUNCTION__ << ":" << __LINE__ << " publish configured event ";
        publishInitialisationPoint(InitializationMessage::CONFIGURED);
        startDownloadUnicastDocuments();
    }
}

void StartupServicePrivate::unassigned()
{
    if(configured)
    {
        configured=false;
        publishInitialisationPoint(InitializationMessage::UNCONFIGURED);
    }
}

//incoming replies
bool StartupServicePrivate::authenticateRespHandler(quint16 requestId, MessagePtr message)
{
    switch(message->messageType)
    {
     /*   case OCI_SERV_AUTHENTICATE_RESP:
        {
            qDebug() << __FUNCTION__ << ":" << __LINE__ << "authencation response received from OCI_SERVICE";
            QMetaObject::invokeMethod(this,"getSubscriberInfo", Qt::QueuedConnection);
            return true;
        }
        case OCI_SERV_ERROR_RESP:
        {
            QSharedPointer<ErrorResp const> resp = message.staticCast<ErrorResp const>();
            qDebug() << __FUNCTION__ << ":" << __LINE__ << " we have an error response from TM with error type "
                     << resp->m_ErrorType
                     << " error msg " << resp->m_ErrorInfo.errorMsg ;
            if(resp->m_ErrorType==OCI_ERROR_TYPE_HTTP || resp->m_ErrorType==OCI_ERROR_TYPE_NETWORK)
            {
                publishInitialisationPoint(InitializationMessage::NETWORK_ERROR);
            }
            else if(resp->m_ErrorType!=OCI_ERROR_TYPE_OTHER)
            {
                publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
            }
            else if(resp->m_ErrorInfo.errorCode==-1602)
            {
                publishInitialisationPoint(InitializationMessage::UNCONFIGURED);
            }
            else
            {
                qDebug() << __FUNCTION__ << ":" << __LINE__ << ": " << __LINE__ << "unknown error " << resp->m_ErrorInfo.errorCode;
                publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
            }
            return true;
        }*/
        default:
          return false;
    }
}

MessagePtr StartupServicePrivate::authenticateRespDeserializer(quint16 type, QDataStream &stream)
{
    switch(type)
    {
        /*case OCI_SERV_AUTHENTICATE_RESP:
        {
            Message *msg = new AuthenticateResp();
            msg->deserialize(stream);
            return MessagePtr(msg);
        }
        case OCI_SERV_ERROR_RESP:
        {
           Message *msg = new ErrorResp();
           msg->deserialize(stream);
           return MessagePtr(msg);
        }*/
        default:
            return MessagePtr();
    }
}

bool StartupServicePrivate::checkSWVersionRespHandler(quint16 requestId, MessagePtr message)
{
    switch(message->messageType)
    {
        /*case PACKAGE_MANAGER_SERV_CHECK_FOR_NEW_SW_RESP:
        {
            QMetaObject::invokeMethod(this,"swCheckReceived", Qt::QueuedConnection,
                                      Q_ARG(bool,message.staticCast< CheckForNewSTBSwResponse const>()->packageToInstall));
            return true;
        }*/
        default:
            return false;
    }
}

MessagePtr StartupServicePrivate::checkSWVersionRespDeserializer(quint16 type, QDataStream &stream)
{
   switch(type)
    {
      /*  case PACKAGE_MANAGER_SERV_CHECK_FOR_NEW_SW_RESP:
        {
            Message *msg = new CheckForNewSTBSwResponse();
            msg->deserialize(stream);
            return MessagePtr(msg);
        }*/
        default:
            return MessagePtr();
    }
}

bool StartupServicePrivate::synchronizeUnicastRespHandler(quint16 requestId, MessagePtr message)
{
    switch(message->messageType)
    {
       /* case BLSTARTUP_SYNCHRONIZED_UNICAST_DOCUMENTS_RESP:
        {
             QSharedPointer<SynchronizedDataResp const> resp= message.staticCast<SynchronizedDataResp const>();
             QMetaObject::invokeMethod(this,"retrievalUnicastResp",Qt::QueuedConnection,
                                       Q_ARG(SynchronizedDataResp::Type,resp->error));
             return true;
        }*/
        default:
             return false;
    }
}

MessagePtr StartupServicePrivate::synchronizeUnicastRespDeserializer(quint16 type, QDataStream &stream)
{
    switch(type)
    {
      /*  case BLSTARTUP_SYNCHRONIZED_UNICAST_DOCUMENTS_RESP:
        {
            Message *msg=new SynchronizedDataResp();
            msg->deserialize(stream);
            return MessagePtr(msg);
        }*/
        default:
           return MessagePtr();
    }
}

bool StartupServicePrivate::subscriberInfoRespHandler(quint16 requestId, MessagePtr message)
{
    switch(message->messageType)
    {
        /*case OCI_WRITER_SERV_SUBSRIBER_INFO_RESP:
        {
            QSharedPointer<SubscriberInfoResponse const> response = message.staticCast<SubscriberInfoResponse const>();
            if(response->errorCode==200)  // indicates that we have no error
            {
               QMetaObject::invokeMethod(this,"checkSWVersion",Qt::QueuedConnection);
            }
            else
            {
                //TODO -> after chackin since change in OCI_WRITER needed !!!
            }
        }*/
        default:
            return false;
     }
}

MessagePtr StartupServicePrivate::subscriberInfoRespDeserializer(quint16 type, QDataStream &stream)
{
    switch(type)
    {
     /*   case OCI_WRITER_SERV_SUBSRIBER_INFO_RESP:
        {
          Message *msg= new SubscriberInfoResponse();
          msg->deserialize(stream);
          return MessagePtr(msg);
        }*/
        default:
          return MessagePtr();
    }
}

bool StartupServicePrivate::autoConfigRespHandler(quint16 requestId, MessagePtr message)
{
   switch(message->messageType)
    {
        /*case OCI_SERV_GET_AUTOCONFIG_RESP:
        {
           qDebug() << __FUNCTION__ << ":" << __LINE__  << ": autoconfig done";
           cbus.sendReply(autoConfigRequestId, MessagePtr(new AutoConfigResponse(AutoConfigResponse::NO_ERROR)));
           QMetaObject::invokeMethod(this,"getSubscriberInfo",Qt::QueuedConnection);  // continue getting subscriberInfo
           return true;
        }
        case OCI_SERV_ERROR_RESP:
        {
            QSharedPointer<ErrorResp const> resp = message.staticCast<ErrorResp const>();
            AutoConfigResponse::Type code;
            if(resp->m_ErrorType==OCI_ERROR_TYPE_HTTP || resp->m_ErrorType==OCI_ERROR_TYPE_NETWORK)
            {
                qDebug() << __FUNCTION__ << ":" << __LINE__  << ": network errror during autoconfig error type :"
                         << resp->m_ErrorType;
              //  publishInitialisationPoint(InitializationMessage::NETWORK_ERROR);
                code=AutoConfigResponse::NETWORK_ERROR;
            }
            else
            {
                publishInitialisationPoint(InitializationMessage::AUTHENTICATION_ERROR);
                switch(resp->m_ErrorInfo.errorCode)
                {
                  case -7:
                    qDebug() << __FUNCTION__ << ":" << __LINE__  << " invalid pin";
                    code=AutoConfigResponse::INVALID_PIN;
                    break;
                  case -4:
                    qDebug() << __FUNCTION__ << ":" << __LINE__  << " invalid account number";
                    code=AutoConfigResponse::INVALID_ACCOUNT_NUMBER;
                    break;
                  case -80:
                    qDebug() << __FUNCTION__ << ":" << __LINE__ << " account locked";
                    code=AutoConfigResponse::ACCOUNT_LOCKED;
                    break;
                  case -27:
                  case -28:
                  case -29:
                  case -51:
                  case -53:
                  case -64:
                  case -77:
                    qDebug() << __FUNCTION__ << ":" << __LINE__ << "error "
                             << resp->m_ErrorInfo.errorCode << ": " << resp->m_ErrorInfo.errorMsg;
                    code=AutoConfigResponse::REGISTRATION_FAILED;
                    break;
                  default:
                    qDebug() << __FUNCTION__ << ":" << __LINE__ << " unknown error "
                             << resp->m_ErrorInfo.errorCode << " " << resp->m_ErrorInfo.errorMsg;
                    code=AutoConfigResponse::UNKNOWN_ERROR;
                    break;
                 }
            }
            cbus.sendReply(autoConfigRequestId, MessagePtr(new AutoConfigResponse(code)));
            return true;
        }*/
        default:
            return false;
   }
}

MessagePtr StartupServicePrivate::autoConfigRespDeserializer(quint16 type, QDataStream &stream)
{
    switch(type)
    {/*
        case OCI_SERV_GET_AUTOCONFIG_RESP:
        {
            Message *msg = new GetAutoconfigResp();
            msg->deserialize(stream);
            return MessagePtr(msg);
        }
        case OCI_SERV_ERROR_RESP:
        {
            Message *msg = new ErrorResp();
            msg->deserialize(stream);
            return MessagePtr(msg);
        }*/
        default:
            return MessagePtr();
   }
}

bool StartupServicePrivate::getBrokerDocRespHandler(quint16 requestId, MessagePtr message)
{
    switch(message->messageType)
    {/*
        case MMDDF_WRITER_SERV_GET_BROKER_DOC_SYNCHRONOUS_RESP:
           switch(message.staticCast<GetBrokerDocumentsSynchronousResp const>()->response)
           {
               case GetBrokerDocumentsSynchronousResp::GBDR_SUCCESS:
               {

                     qDebug() << __FUNCTION__ << ":" << __LINE__ << " GLOBALINSTALL and GROUPS downloaded";
                     QMetaObject::invokeMethod(this,"authenticate", Qt::QueuedConnection);
                     break;
              }
              case  GetBrokerDocumentsSynchronousResp::GBDR_NETWORK_ERROR:
                     publishInitialisationPoint(InitializationMessage::NETWORK_ERROR);
                     break;
              default:
                     publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
                     break;
           }
           return true;*/
        default:
            return false;
    }
}


MessagePtr StartupServicePrivate::getBrokerDocRespDeserializer(quint16 type, QDataStream &stream)
{
    switch(type)
    {
       /* case MMDDF_WRITER_SERV_GET_BROKER_DOC_SYNCHRONOUS_RESP:
        {
            Message *msg = new GetBrokerDocumentsSynchronousResp();
            msg->deserialize(stream);
            return MessagePtr(msg);
        }*/
        default:
            return MessagePtr();
    }
}

bool StartupServicePrivate::getAsynBrokerDocRespHandler(quint16 requestId, MessagePtr message)
{
    switch(message->messageType)
    {
      /*  case MMDDF_WRITER_SERV_GET_BROKER_DOC_ASYNCHRONOUS_RESP:
           QMetaObject::invokeMethod(this,"startDownloadMulticastDocuments",Qt::QueuedConnection);
           return true;
        default:*/
           return false;
    }
}


MessagePtr StartupServicePrivate::getAsynBrokerDocRespDeserializer(quint16 type, QDataStream &stream)
{
    switch(type)
    {
       /* case MMDDF_WRITER_SERV_GET_BROKER_DOC_ASYNCHRONOUS_RESP:
        {
            Message *msg = new GetBrokerDocumentsAsynchronousResp();
            msg->deserialize(stream);
            return MessagePtr(msg);
        }*/
        default:
            return MessagePtr();
    }
}

// methods for incomming calls
bool StartupServicePrivate::messageHandler(quint16 requestId, MessagePtr message)
{
    switch(message->messageType)
    {
        /*case BLSTARTUP_AUTOCONFIG_REQ:  // we have a autoconfig request from UI
        {
          autoConfigRequestId=requestId;
          QSharedPointer<AutoConfigReq const> req=message.staticCast<AutoConfigReq const>();
          QMetaObject::invokeMethod(this,"autoConfig",Qt::QueuedConnection,Q_ARG(QString,req->pin),
                                          Q_ARG(QString,req->accountNumber), Q_ARG(QString,req->stbName));
          return true;
        }*/
        default:
          return false;
     }
}

bool StartupServicePrivate::messageHandler(MessagePtr message)
{
   switch(message->messageType)
   {
       case BLSTARTUP_DOCUMENT_AVAILABLE:
       {
          QSharedPointer<DocumentAvailable const> resp = message.staticCast<DocumentAvailable const>();
          QMetaObject::invokeMethod(this,"documentAvailable", Qt::QueuedConnection,Q_ARG(QByteArray,resp->document));
          qDebug() << "document available " << resp->document;
          return true;
       }
       case BLSTARTUP_SERVICEREADY:
       {
          QSharedPointer<ServiceReadyMessage const> resp = message.staticCast<ServiceReadyMessage const>();
          QMetaObject::invokeMethod(this,"serviceInitialized",Qt::QueuedConnection,Q_ARG(QByteArray,resp->serviceName));
          return true;
       }
       case BLSTARTUP_UNASSIGNED:
       {
          QMetaObject::invokeMethod(this,"unassigned", Qt::QueuedConnection);
          return true;
       }
       default:
          return false;
   }
}

MessagePtr StartupServicePrivate::deserialize(quint16 type, QDataStream &stream)
{
    switch(type)
    {
        /*case BLSTARTUP_AUTOCONFIG_REQ:  // we have a autoconfig request from UI
        {
          Message *msg = new AutoConfigReq();
          msg->deserialize(stream);
          return MessagePtr(msg);
        }*/
        case BLSTARTUP_DOCUMENT_AVAILABLE:
        {
          Message *msg = new DocumentAvailable();
          msg->deserialize(stream);
          return MessagePtr(msg);
        }
        case BLSTARTUP_SERVICEREADY:
        {
          Message *msg = new ServiceReadyMessage();
          msg->deserialize(stream);
          return MessagePtr(msg);
         }
        case BLSTARTUP_UNASSIGNED:
        {
          Message *msg = new UnAssignedMessage();
          return MessagePtr(msg);
         }
        default:
          return MessagePtr();
     }
}


// private methods

void StartupServicePrivate::publishInitialisationPoint(InitializationMessage::Type type)
{
    QTimer::singleShot(REGISTER_SERV_DELAY*5, [this, type](){
        qDebug() << "----------------------Madhur StartupServicePrivate::publishInitialisationPoint --------------------" << type;
        cbus.push(MessagePtr( new InitializationMessage(type)));
    });
}

void StartupServicePrivate::downloadUnicastDocument()
{/*
  bool nok;
   do
   {
        if(!synchronizeServices.empty())
        {
           unicastService = synchronizeServices.takeFirst();
           qDebug()<< __FUNCTION__ << " try to contact "<< unicastService << " for unicast ";

           if(!cbus.callServiceWithCallback(unicastService, MessagePtr(new SynchronizedDataReq()),
                              QSharedPointer<CallBack> (new CallBackGen<StartupServicePrivate>(this,
                                                             &StartupServicePrivate::synchronizeUnicastRespHandler,
                                                             &StartupServicePrivate::synchronizeUnicastRespDeserializer))))
            {
                  qDebug() << __FUNCTION__ << ":" << __LINE__ << "service " << unicastService  << " not available";
                  nok=true;
            }
           else
           {
               nok=false;
           }
       }
        else
        {
          qDebug() << __FUNCTION__ << ":" << __LINE__ << "we download all the unicast documents";
          nok=false;
       }
  }while(nok);
*/
}

void StartupServicePrivate::downloadMulticastDocuments(QList<QByteArray> const &brokerDocuments)
{

   // send a no-reply request for downloading the multicast documents
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " call MMDDF WRITE for start donwloading multicast documents";
/*   if(!cbus.callService(MMDDF_WRITER_SERV, MessagePtr(new StartDownloadDocuments(brokerDocuments))))
   {
       qDebug() << __FUNCTION__ << ":" << __LINE__ << " can't connect to service MMDDF_WRITER";
       publishInitialisationPoint(InitializationMessage::UNEXPECTED_ERROR);
   }
*/
}

void StartupServicePrivate::startDownloadUnicastDocuments()
{
    /*synchronizeServices.clear();
    synchronizeServices.append(DTV_WRITER_SERV);
    SettingsQuerier db;
    quint8 bootPref=db.getBootPref();
    switch(bootPref)
    {
         case SettingsKeyValue::DTV_PVR_VOD:
         {
           synchronizeServices.append(PVR_WRITER_SERV);
           synchronizeServices.append(ON_DEMAND_WRITER_SERV);
         }
         case SettingsKeyValue::DTV_VOD_PVR:
         {
           synchronizeServices.append(ON_DEMAND_WRITER_SERV);
           synchronizeServices.append(PVR_WRITER_SERV);
         }
     }
    synchronizeServices.append(OCI_WRITER_SERV);
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " start download unicast document";
    downloadUnicastDocument();*/
}

void StartupServicePrivate::startDownloadMulticastDocuments()
{
  /*  QHash<QByteArray, QList<QByteArray> > services;
    foreach( ServicesList const &s, brokerDocuments)
    {
        QMultiMap<quint8, QByteArray> docs;
        foreach(BrokerDocument const &d,s.documents)
        {
            docs.insert(d.docPriority,d.docName);
        }
        services.insert(s.serviceName,docs.values());
    }

    QSet<QByteArray> set;
    set.insert("GLOBALINSTALL");
    set.insert("GROUPS");
    set.insert(DTVLINEUP);
    set.insert("PIPEDSCHEDULE");

    QList<QByteArray> documents;
    insertDocuments(RESOURCE_CONTROLLER_SERV_NAME,services,set,documents);
    insertDocuments(DTV_SERV_NAME,services,set,documents);
    SettingsQuerier db;
    quint8 bootPref=db.getBootPref();
    switch(bootPref)
    {
         case SettingsKeyValue::DTV_PVR_VOD:
         {
           insertDocuments(PVR_SERV_NAME,services,set,documents);
           insertDocuments(OD_SERV_NAME,services,set,documents);
         }
         case SettingsKeyValue::DTV_VOD_PVR:
         {
           insertDocuments(OD_SERV_NAME,services,set,documents);
           insertDocuments(PVR_SERV_NAME,services,set,documents);
         }
     }
    foreach(QByteArray const &name,services.keys())
    {
        insertDocuments(name,services,set,documents);
    }
    documents.append("GLOBALINSTALL");
    documents.append("GROUPS");
    documents.append(DTVLINEUP);
    documents.append("PIPEDSCHEDULE");
    downloadMulticastDocuments(documents);
*/
}

void StartupServicePrivate::insertDocuments(QByteArray const &serviceName, QHash<QByteArray, QList<QByteArray> > &services,
                                            QSet<QByteArray> &set, QList<QByteArray> &documents)
{
    QHash<QByteArray, QList<QByteArray> >::const_iterator list=services.constFind(serviceName);
    if(list!=services.constEnd())
    {
        foreach(QByteArray const &doc,list.value())
        {
            if(!set.contains(doc))
            {
                documents.append(doc);
                set.insert(doc);
            }
        }
    }
    services.remove(serviceName);
}

