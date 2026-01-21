#ifndef STARTUPSERVICE_H
#define STARTUPSERVICE_H
/**
 *
 * Performs business logic startup. inits & authenticates STB and pushes messages to comm lib with its initialization
 * state, so that other modules can also start their initialization at their appropriate message.
 *
 * notes:
 * -stbProperties portUpdate is part of Upnp feature, so it is not done here.
 *
 */

#include <message.h>
#include <deserializer.h>
#include <messagehandler.h>
#include <initializationmessage.h>
#include <synchronizeddatamessage.h>
#include <brokerdocument.h>
#include <QList>
#include <QObject>

#define DTVLINEUP "DTVLINEUP_v.3.1"

class CBusInterface;

class StartupServicePrivate:public QObject,
        public Deserializer, public MessageHandler
{
   Q_OBJECT

   public:
    StartupServicePrivate();
    virtual ~StartupServicePrivate();

    // this method will be called from appmanager
    void init(QList<QByteArray> const &mwServiceList, QList<ServicesList> const &brokerDocumentList,
              quint16 minEPGhrs);

    bool authenticateRespHandler(quint16 requestId, MessagePtr message);
    MessagePtr authenticateRespDeserializer(quint16 type, QDataStream &stream);
    bool checkSWVersionRespHandler(quint16 requestId, MessagePtr message);
    MessagePtr checkSWVersionRespDeserializer(quint16 type, QDataStream &stream);
    bool synchronizeUnicastRespHandler(quint16 requestId, MessagePtr message);
    MessagePtr synchronizeUnicastRespDeserializer(quint16 type, QDataStream &stream);
    bool subscriberInfoRespHandler(quint16 requestId, MessagePtr message);
    MessagePtr subscriberInfoRespDeserializer(quint16 type, QDataStream &stream);
    bool autoConfigRespHandler(quint16 requestId, MessagePtr message);
    MessagePtr autoConfigRespDeserializer(quint16 type, QDataStream &stream);
    bool getBrokerDocRespHandler(quint16 requestId, MessagePtr message);
    MessagePtr getBrokerDocRespDeserializer(quint16 type, QDataStream &stream);
    bool getAsynBrokerDocRespHandler(quint16 requestId, MessagePtr message);
    MessagePtr getAsynBrokerDocRespDeserializer(quint16 type, QDataStream &stream);

    bool messageHandler(quint16 requestId, MessagePtr message);
    bool messageHandler(MessagePtr message);
    MessagePtr deserialize(quint16 type, QDataStream &stream);

   private slots:
    void authenticate();
    void getSubscriberInfo();
    void checkSWVersion();
    void autoConfig(QString pin, QString accountNbr, QString stbName);
    void serviceInitialized(QByteArray serviceName);
  //  void retrievalUnicastResp(SynchronizedDataResp::Type error);
    void swCheckReceived(bool packageToInstall);
    void unassigned();
    void startDownloadMulticastDocuments();
    void documentAvailable(QByteArray document);

   private:
    void publishInitialisationPoint(InitializationMessage::Type);
    void startDownloadUnicastDocuments();
    void downloadUnicastDocument();
    void downloadMulticastDocuments(QList<QByteArray> const &brokerDocuments);
    void insertDocuments(QByteArray const &serviceName, QHash<QByteArray, QList<QByteArray> > &services,
                         QSet<QByteArray> &set, QList<QByteArray> &documents);
    CBusInterface &cbus;
    QList<quint8> synchronizeServices;
    quint16 autoConfigRequestId;
    quint16 minEPGhrs;
    QList<ServicesList> brokerDocuments;
    QList<QByteArray> services;
    bool allServicesInitialized;
    bool configured;
    quint8 unicastService;
    bool rcInitialized;
    bool canServiceInitialized = false;
    bool dbServiceInitialized =false;
};


#endif // STARTUPSERVICE_H
