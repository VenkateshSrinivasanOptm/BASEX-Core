#ifndef CANSERVICE_H
#define CANSERVICE_H

#include "subscriberIntf.h"
#include <cbusinterface.h>
#include <deserializer.h>
#include <messagehandler.h>
#include <canlib.h>
#include <appmanmsgif.h>
#include "canparser.h"
#include <QMap>
#include <QTimer>
#include <QList>


#define MWNAMESTR "canservice"
#define MSVERSTR "1.0"

class InitializationMessage;
class Canservice : public QObject,
                   public Deserializer,
                   public MessageHandler,
                   public SubscriberIntf {
    Q_OBJECT
public:
    static Canservice &getInstance();
    /**
   * this method is not used
   */
    virtual bool messageHandler(MessagePtr message);
    /**
   * message handler
   */
    virtual bool messageHandler(quint16 requestId, MessagePtr message);
    /**
   * deserialization for incoming messages
   */
    virtual MessagePtr deserialize(quint16 messageType, QDataStream &data);

    /**
   * This method is called when a  message is received .
   * Since this method is called in the thread of the communication library
   * this method should return immediately
   **/
    virtual bool subscriberMessageHandler(MessagePtr message);

    /**
   * this method is called when an incoming mesage needs to be deserialized
   **/
    virtual MessagePtr subscriberDeserialize(quint16 messageType,
                                             QDataStream &stream);
    /**
   * callback function to get the response for the request( requestId )
   */
    bool callback(quint16 requestId, MessagePtr reply);

    /*! Forwards sendReply to CBus */
    void sendReply(quint16 requestId, MessagePtr const &message);

    /*! Forwards callService (= noreply message to a service) to CBus */
    bool callService(quint8 serviceName, MessagePtr const &message);

      /* initialisation method for startup sequence */
    void initialise(void);

signals:

public slots:
    /* heartbeat method for application manager */
    void heartbeat();
    /* subscribe method for startup sequence */
    void establishCommunication(void);
    /* handle poweer rstart notication method for starequestIdrtup sequence */
    void handlePowerRestartNotification(StateChange state);

signals:
    // Startup Services BLSTARTUP_INTIALIZATION message received
    void startupService(QSharedPointer<InitializationMessage const>);

public slots:
    void processStart(QSharedPointer<InitializationMessage const>);

private:
    void writeBSS_Control_ParamToCAN();

private:
    Canservice();
    virtual ~Canservice();
    Q_DISABLE_COPY(Canservice);

    CBusInterface& cBus;
    Canlib& m_canParser;
    QSharedPointer<CallBack> callBack;
    void writeCANMessage(QByteArray canData, uint canMsg);
};

#endif // CANSERVICE_H
