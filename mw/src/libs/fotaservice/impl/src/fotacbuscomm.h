#ifndef FOTACBUSCOMM_H
#define FOTACBUSCOMM_H

#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include <appmanif.h>
#include <initializationmessage.h>

class CBusInterface;

enum Target_Device
{
    CCB_0 = 0x01,
    BCB_0 = 0x02,
    BCB_1 = 0x03,
    BCB_2 = 0x04,

    POD_BMS_START = 0x10,
    POD_BMS_END = 0x2F,

    POD_DCCOM_START = 0x30,
    POD_DCCOM_END = 0x4F
};
Q_DECLARE_METATYPE(Target_Device)

enum FlashingState
{
    // Start and End States
    FlashingState_Start = 0x0A,
    FlashingState_End = 0x0F,

    // Precondition Check States
    FlashingState_PreconditionCheck = 0x10,
    FlashingState_SystemCheck_Fail = 0x11,
    FlashingState_SystemHWCheck_Fail = 0x12,
    FlashingState_SystemSWCheck_Fail = 0x13,
    FlashingState_CommunicationCheck_Fail = 0x14,
    FlashingState_AuthenticationCheck = 0x15,
    FlashingState_PreconditionCheck_Reserved = 0x16,

    // Flashing Process States [1]
    FlashingState_FlashingProcess_1 = 0x20,
    FlashingState_FlashCheck_Fail = 0x21,
    FlashingState_FlashErase_Fail = 0x22,
    FlashingState_FlashingProcess_1_Reserved_1 = 0x23,
    FlashingState_FlashingProcess_1_Reserved_2 = 0x24,
    FlashingState_FlashingProcess_1_Reserved_3 = 0x25,

    // Flashing Process States [2]
    FlashingState_FlashingProcess_2 = 0x30,
    FlashingState_TransferFail = 0x31,
    FlashingState_Communication_Fail = 0x32,
    FlashingState_FlashingProcess_2_Reserved_1 = 0x33,
    FlashingState_FlashingProcess_2_Reserved_2 = 0x34,
    FlashingState_FlashingProcess_2_Reserved_3 = 0x35,

    // Postcondition Check States
    FlashingState_PostconditionCheck = 0x40,
    FlashingState_FlashAuthenticityCheck_Fail = 0x41,
    FlashingState_FlashIntegrityCheck_Fail = 0x42,
    FlashingState_FlashHWCheck_Fail = 0x43,
    FlashingState_FlashSWCheck_Fail = 0x44,
    FlashingState_PostconditionCheck_Fail = 0x45,
    FlashingState_PostsystemCheck_Fail = 0x46,
    FlashingState_PostconditionCheck_Reserved = 0x47,

    // Flashing Errors
    FlashingState_Error_Timeout = 0xFF,
    FlashingState_Error_1 = 0xFE,
    FlashingState_Error_2 = 0xFD,
    FlashingState_Error_3 = 0xFC
};
Q_DECLARE_METATYPE(FlashingState)


class FotaCBusComm : public QObject,
                   public MessageHandler,
                   public Deserializer,
                   public SubscriberIntf
{
    Q_OBJECT

public:

    FotaCBusComm();
    virtual ~FotaCBusComm();

    /*! To initialize the CBus and subscribe it to the Startup services */
    void initialise();

    /*! Method to deserialize messages transmitted by the communication lirary */
    virtual MessagePtr deserialize(quint16 messageType, QDataStream &stream);

    /*! Methods handles messages received from Resource Controller */
    virtual bool messageHandler(MessagePtr message);

    /*! Method handles received messages transmitted by the communication lirary
   */
    virtual bool messageHandler(quint16 requestId, MessagePtr message);

    /*! Method to deserialize the startup message: BLSTARTUP_INTIALIZATION */
    MessagePtr subscriberDeserialize(quint16 messageType, QDataStream &stream);

    /*! Method to handle incoming startup message: BLSTARTUP_INTIALIZATION */
    bool subscriberMessageHandler(MessagePtr message);

    /*! Forwards sendReply to CBus */
    void sendReply(quint16 requestId, MessagePtr const &message);

    /*! Forwards callService (= noreply message to a service) to CBus */
    bool callService(quint8 serviceName, MessagePtr const &message);

    /*! Forwards push (= publish message to all the subscribed services) to CBus
   */

public slots:

    void push(MessagePtr const &message);
    /* heartbeat method for application manager */
    void heartbeat();
    /* subscribe method for startup sequence */
    void establishCommunication(void);
    /* handle poweer rstart notication method for starequestIdrtup sequence */
    void handlePowerRestartNotification(StateChange state);

signals:

    // void writeConfig(quint16 messageType, const ServerConfig& data);  // for future use based on teh struct
    void startupService(QSharedPointer<InitializationMessage const>);

private:
    CBusInterface &cbus;
};

#endif // fotaCBUSCOMM_H
