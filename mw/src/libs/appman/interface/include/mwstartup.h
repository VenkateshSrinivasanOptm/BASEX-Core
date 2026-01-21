#pragma once

#include <QObject>
#include <QTimer>
#include <QList>
#include "appmanmessages.h"
#include "appmanmsgif.h"
#include "startupservice.h"
#include <messagehandler.h>
#include <deserializer.h>
#include "servicereadymessage.h"

class CommandMonitor;
class CBusInterface;

#define UI_PROCESS_ID "ui"
#define POWER_SAVING_STATE_ON POWER_SAVING_STATE_ACTIVE
#define PERFORMANCE_BUILD

#ifdef PERFORMANCE_BUILD
#include <QElapsedTimer>
#endif

//class QSemaphore;
class Package;
//class SemWatcher;
struct InitializationMessage;

class MwStartup : public QObject, public MessageHandler, public Deserializer
{
    Q_OBJECT
public:
    MwStartup();
    virtual ~MwStartup();
    void startAll(void);
    virtual MessagePtr deserialize(quint16 messageType, QDataStream &stream);
    virtual bool messageHandler(MessagePtr message);
    virtual bool messageHandler(quint16 requestId,
                                    MessagePtr message);
signals:
    void startMessage(const InitializationMessage& message);
    void askUIToReboot(quint8 reqRestartLevel);
    void forcedReboot();
#ifdef TEST_BUILD
public slots:
    void testInstructionMsgReceived(QByteArray msg);
#endif
private slots:
    void publishedEventReceived(void);
    void subscribedEventReceived(void);
    void allPublished(void);
    void allSubscribed(void);
    void beatTheHeart(void);
    void shutdownEventReceived(ErrorLevel level,QByteArray processId);
    void uiRestartResponse(bool restart);
    void uiPowerNotification(UiPowerNotification state);
    void startupStabilityTimeout();
    void amStartupTimedout();
    void amTerminationTimedout();
    void uiRestartTimeout();
    void forcedRebootRequest();
    void askUIForRebootRequest(quint8 reqRestartLevel);
    void processStartStateChange(const InitializationMessage& message);

private:

    typedef enum {
        AM_STAGE_PUBLISHING,
        AM_STAGE_SUBSCRIBE,
        AM_STAGE_STARTED,
        AM_STAGE_STOPPING
    } StartStage;

    typedef enum {
        AM_POWER_CHANGE_EVENT,
        AM_PACKAGE_MGR_EVENT,
        AM_EXPECTED_CHILD_TERMINATION_EVENT,
        AM_UNEXPECTED_CHILD_TERMINATION_EVENT,
        AM_TIMER_EVENT,
        AM_CHILD_STARTUP_EVENT,
        AM_ALL_TERMINATED_EVENT
    } ActivationEvent;

    void shutdownChildren(ActivationEvent event);
    void selfTerminate();
    void startDeadChildren();
    void allTerminated(void);
    void configureStartNotification(void);


    QList<Package *> packageList;
    StateChange stateChange;

    // The following variables define the internal state of Application Manager
    CommandMonitor *monitor;
    UiPowerNotification powerState;
    CBusInterface &cbus;
    StartStage startStage;
    ErrorLevel errorLevel;
//TODO    RestartLevel restartLevel;
    int restartRequested; //0=> not requested 1-4 => request count - We have to ask 4 times at 30 min intervals before forcing restart
    QTimer uiRestartTimer;
    bool startupStabilityCheck;
    int numToPublish;
    int numToTerminate;
    int numToSubscribe = 2;
    QTimer heartbeatTimer;
    QTimer amStartTimer;
    QTimer amTerminateTimer;
#ifdef PERFORMANCE_BUILD
    QElapsedTimer t;
#endif
};

