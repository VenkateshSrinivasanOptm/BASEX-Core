#include <QProcess>
#include "mwconfig.h"
#include "package.h"
#include "appmanif.h"
#include "mwstartup.h"
#include "commandmonitor.h"
#include "cbusinterface.h"
#include "appmanmessages.h"
#include <QMetaType>
#include <QSharedPointer>
#include <interface/include/mwstartup.h>
#include <initializationmessage.h>
#include <startupservice.h>

#ifdef TEST_BUILD
#include <QList>
#endif

Q_DECLARE_METATYPE (QSharedPointer<InitializationMessage const>)

MwStartup::MwStartup(): cbus(CBusInterface::instance())
{
#ifdef PERFORMANCE_BUILD
    t.start();
#endif
#ifdef TEST_BUILD
    switch(HALPowerSavingInterface::getCurrentPowerSavingLevel())
    {
    case POWER_SAVING_STATE_ON:
        powerState= UI_NOTIFY_POWER_ON;
        break;
    case POWER_SAVING_STATE_STANDBY:
        powerState= UI_NOTIFY_POWER_OFF;
        break;
    case POWER_SAVING_STATE_DEEP_SLEEP:
        powerState= UI_NOTIFY_POWER_DEEP_SLEEP;
        break;
    case POWER_SAVING_STATE_POWER_OFF:
        powerState= UI_NOTIFY_POWER_STANDBY;
        break;
    default:
        break;
    }
#else
    powerState = UI_NOTIFY_POWER_ON;
#endif
    qRegisterMetaType< QSharedPointer<InitializationMessage const> >();
    startStage=AM_STAGE_PUBLISHING;
    errorLevel=EXIT_NO_ERROR;
//    restartLevel=RESTART_UNDEFINED;
    restartRequested=0;
    startupStabilityCheck=false;
    MwConfig::instantiate();
    monitor=new CommandMonitor();
    QTimer::singleShot(REGISTER_SERV_DELAY,[this](){
        if(!cbus.registerService(APPMAN_SERVICE,this,this))
        {
            qDebug() << "WARN - APPMAN_SERVICE is already registered.";
        }
    });

}
MwStartup::~MwStartup()
{
    cbus.unregisterService(APPMAN_SERVICE);
#ifdef TEST_BUILD
    delete monitor;
#endif
}
void MwStartup::startAll()
{
    MwConfig *mwConfig = MwConfig::getInstance();

#ifdef PERFORMANCE_BUILD
    qDebug() <<"Started at " << t.elapsed();
#endif
    // If the HAL informs us that we are in deep sleep or off then we need to immediately terminate
   /*TODO if (powerState== UI_NOTIFY_POWER_DEEP_SLEEP || powerState== UI_NOTIFY_POWER_STANDBY)
    {
        qDebug() << "ERROR - HAL informed AppMan that the current power state needs to be OFF or DEEP SLEEP");
        exit(1);
    }*/
//    sem = new SemWatcher();

    // Acquire our configuration data.
    mwConfig->acquire();

    qDebug() << "Config Data Acquired";

#ifdef TEST_BUILD
    connect(monitor,SIGNAL(testInstructionMsg(QByteArray)),this,SLOT(testInstructionMsgReceived(QByteArray)));
#endif
    monitor->start();

    configureStartNotification();
    connect(&heartbeatTimer, SIGNAL(timeout()), this, SLOT(beatTheHeart()));
    connect(&uiRestartTimer, SIGNAL(timeout()), this, SLOT(uiRestartTimeout()));
    connect(&amStartTimer, SIGNAL(timeout()), this, SLOT(amStartupTimedout()));
    connect(&amTerminateTimer, SIGNAL(timeout()), this, SLOT(amTerminationTimedout()));
    connect(this,SIGNAL(askUIToReboot(quint8)), this, SLOT(askUIForRebootRequest(quint8)));
    connect(this,SIGNAL(forcedReboot()), this, SLOT(forcedRebootRequest()));
    amTerminateTimer.setInterval(mwConfig->getPackageTerminateTimeout());

    // Start the stability check timer
    if (startupStabilityCheck==false)
    {
        QTimer::singleShot(mwConfig->getStabilityTimeout(), this, SLOT(startupStabilityTimeout()));
        startupStabilityCheck=true;
    }

    // Start a timer to check that we startup within a defined period of time
    amStartTimer.start(mwConfig->getAMStartTimeout());

    //Start the bag of processes
    for (int i=0;i<mwConfig->getPackageCount();i++)
    {
        packageList << new Package();
        connect(packageList.last(), SIGNAL(published()), this, SLOT(publishedEventReceived()));
        connect(packageList.last(), SIGNAL(subscribed()), this, SLOT(subscribedEventReceived()));
        connect(packageList.last(), SIGNAL(shutdown(ErrorLevel,QByteArray)), this, SLOT(shutdownEventReceived(ErrorLevel, QByteArray)));
        connect(packageList.last(), SIGNAL(swRestartResponse(bool)), this, SLOT(uiRestartResponse(bool)));
        connect(packageList.last(), SIGNAL(uiPowerChangeNotification(UiPowerNotification)), this, SLOT(uiPowerNotification(UiPowerNotification)));

        packageList.last()->start(mwConfig->getPackageId(i), mwConfig->getPackagePath(i));
    }

    qDebug() <<"Block on the arrival of the published event";
}

bool MwStartup::messageHandler(quint16 /*requestId*/, MessagePtr message)
{
    switch(message->messageType)
    {
    case BLSTARTUP_INTIALIZATION:
    {
        emit startMessage(message.staticCast<InitializationMessage const>()->type);
        return true;
    }
    case APPMAN_SERVICE_ASK_UI_FOR_REBOOT_REQ:
        {
            QSharedPointer<AskUIForRebootRequest const> msg=message.staticCast<AskUIForRebootRequest const>();
            emit askUIToReboot(msg->restartMode);
        }
        break;
    case APPMAN_SERVICE_FORCE_REBOOT_REQ:
        {
            QSharedPointer<ForceRebootForNewSw const> msg=message.staticCast<ForceRebootForNewSw const>();
            emit forcedReboot();
        }
        break;
    default:
        return false;
    }
    return true;
}
bool MwStartup::messageHandler(MessagePtr message)
{
    qDebug() << "ERROR - No Reply received from message: " << message->messageType;
    return false;
}

MessagePtr MwStartup::deserialize(quint16 messageType, QDataStream &stream)
{
        qDebug() <<"StartupServicePrivate::deserialize - deserialize message of type " << messageType;
        Message *message;
        switch(messageType){
        case BLSTARTUP_INTIALIZATION:
        {
            qDebug() <<"MwStartup::deserialize - blStartup state change: " << messageType;
            Message *message = new InitializationMessage();
            message->deserialize(stream);
            return MessagePtr(message);
        }
        case  APPMAN_SERVICE_ASK_UI_FOR_REBOOT_REQ:
            message = new AskUIForRebootRequest();
            message->deserialize(stream);
            return MessagePtr(message);
            break;
        case  APPMAN_SERVICE_FORCE_REBOOT_REQ:
            message = new ForceRebootForNewSw();
            message->deserialize(stream);
            return MessagePtr(message);
            break;
        default:
            qDebug() << "WARNING - Code missing to deserialize message: " << messageType;
            break;
        }
        return MessagePtr();
}

void MwStartup::processStartStateChange(const InitializationMessage& message) {
    switch (message.type) {
        case InitializationMessage::START: {
            if (!cbus.callService(STARTUP_SERV, MessagePtr(new ServiceReadyMessage(APPMAN_SERVICE_NAME))))
                qDebug() <<  "MwStartup::processStartStateChange - Sending message to STARTUP_SERV failed.";
             break;
         }

         default:
            ;
    }
}  // End of processStartStateChange
void MwStartup::forcedRebootRequest()
{
    // Package Manager is asking us to restart the STB for the purpose of installing new software
    qDebug() <<"Force Restart after SW Download";
    errorLevel=STB_LEVEL_ERR;
    startStage=AM_STAGE_STOPPING;
    shutdownChildren(AM_UNEXPECTED_CHILD_TERMINATION_EVENT);
}
void MwStartup::askUIForRebootRequest(quint8 reqRestartLevel)
{
    qDebug() <<"askUIForRebootRequest(" << reqRestartLevel << ")";

    //Ignore any request to restart if we are still publishing or subscribing
    if (startStage==AM_STAGE_PUBLISHING || startStage==AM_STAGE_SUBSCRIBE)
    {
        qDebug() << "WARNING: UI Restart request received from PM whilst in start stage PUBLISH or SUBSCRIBE";
        return;
    }
}

void MwStartup::uiRestartResponse(bool restart)
{
    qDebug() <<"uiRestartResponse " << restart;
    // The UI has informed us whether we can restart after package download
    if (restart==true)
    {
       if (restartRequested>0)
       {
          restartRequested=0;
          uiRestartTimer.stop();
       }
       startStage=AM_STAGE_STOPPING;
       shutdownChildren(AM_UNEXPECTED_CHILD_TERMINATION_EVENT);
       restartRequested=0;
    }
    else
        uiRestartTimer.start(MwConfig::getInstance()->getUIrestartRequestTimeout());
}

void MwStartup::uiRestartTimeout()
{
    qDebug() << "uiRestartTimeout " << restartRequested;
    uiRestartTimer.stop();

}

void MwStartup::beatTheHeart()
{
    Package *package;

    qDebug() <<"Beat the heart";
    fflush(stderr);

    foreach (package,packageList)
    {
        if (!package->sendHeartbeat())
        {
            qDebug() << "ERROR: HEARTBEAT STOPPED on process " << package->getPackageId();
        }
    }
}
void MwStartup::amStartupTimedout()
{
    // Ouch, we did not start in time. We must force a reset
    qDebug() << "ERROR: Timeout on AM startup. Forcing restart of Machine";
    errorLevel=STB_LEVEL_ERR;
    startStage=AM_STAGE_STOPPING;
    shutdownChildren(AM_UNEXPECTED_CHILD_TERMINATION_EVENT);
}


void  MwStartup::startupStabilityTimeout(void)
{
    startupStabilityCheck=false; // We have now been working long enough to consider ourselves as stable.
}


void MwStartup::uiPowerNotification(UiPowerNotification newState)
{
    Q_UNUSED(newState);
}

void MwStartup::allPublished()
{
    Package *package;
    // Obtain the current Power Level
    StateChange powerState = POWER_ON; //TODO Madhur

    // Were we expecting this event
    if (startStage!=AM_STAGE_PUBLISHING)
    {
        //oops
        qDebug() << "ERROR - All Published event received out of sequence" ;
        return;
    }

    qDebug() <<"All published events received" ;

#ifdef PERFORMANCE_BUILD
    qDebug() <<"Time elapsed: " << t.elapsed() << " ms" ;
#endif
    startStage=AM_STAGE_SUBSCRIBE;

    // Configure our next notification to signal that they have subscribed.
    configureStartNotification();

    //Allow the processes to subscribe
    foreach (package,packageList)
    {
        if(package->getProcessState() == PACKAGE_PUBLISHED)
        {
            package->sendSubscribeMessage((char) powerState + '0'); //HACK Madhur
        }

    }

}

void MwStartup::allSubscribed()
{
    MwConfig *mwConfig = MwConfig::getInstance();

    // Were we expecting this event
    if (startStage!=AM_STAGE_SUBSCRIBE)
    {
        //oops
        qDebug() << "ERROR - Subscribe event received out of sequence" ;
        return;
    }

#ifdef PERFORMANCE_BUILD
    qDebug() <<"Time elapsed: " << t.elapsed() << " ms" ;
#endif
    startStage=AM_STAGE_STARTED;

    // Start the startup  business logic
    {
        StartupService::instance().init(mwConfig->mwServNameList,mwConfig->mwServiceList,mwConfig->minEPGhrs);

    }

    // Activate the heartbeat
    qDebug() <<"Heartbeat timer set at " << mwConfig->getHeartbeatPeriod() << " msecs" ;
#ifdef PERFORMANCE_BUILD
    qDebug() <<"Time elapsed: " << t.elapsed() << " ms" ;
#endif
    heartbeatTimer.start(mwConfig->getHeartbeatPeriod());

    // We can now deactivate the startup timeout
    amStartTimer.stop();

#ifdef TEST_BUILD
    MsgManager::testSendMessageToTest(TEST_APPMAN_STARTED_MSG);
#endif
}

void MwStartup::allTerminated()
{
    qDebug() <<"ALL TERMINATED" ;

    // We can now deactivate the timeout to terminate
    amTerminateTimer.stop();

#ifdef TEST_BUILD
    MsgManager::testSendMessageToTest(TEST_APPMAN_ALL_TERMINATED_MSG);
#endif
    // Check that the notification is valid

}

void MwStartup::startDeadChildren()
{
    Package *package;
    errorLevel=EXIT_NO_ERROR;
    MwConfig *mwConfig = MwConfig::getInstance();


    // Start the stability check timer
    if (startupStabilityCheck==false)
    {
        QTimer::singleShot(mwConfig->getStabilityTimeout(), this, SLOT(startupStabilityTimeout()));
        startupStabilityCheck=true;
    }

    // Start a timer to check that we startup within a defined period of time
    amStartTimer.start(mwConfig->getAMStartTimeout());

    // Configure our notification semaphore to tell us when everything has started
    configureStartNotification();

    // Restart the dead children
    foreach (package,packageList)
    {
        switch (package->getProcessState())
        {
            case PACKAGE_DEAD:
                package->restart();
                break;
            default:
                break;
        }
    }
}

void MwStartup::configureStartNotification()
{
    MwConfig *mwConfig = MwConfig::getInstance();

    int affectedChildCount;

    affectedChildCount=mwConfig->getPackageCount();

    if (startStage==AM_STAGE_PUBLISHING)
    {
        numToPublish=affectedChildCount;
    }
    else if (startStage==AM_STAGE_SUBSCRIBE)
        numToSubscribe=affectedChildCount;
}

void MwStartup::publishedEventReceived()
{
    if(numToPublish != 0) // on restart process, numToPublish = 0
    {
        numToPublish--;
    }

    if (numToPublish==0)
    {
        startStage=AM_STAGE_PUBLISHING;
        allPublished();
    }
}

void MwStartup::subscribedEventReceived()
{
    if(numToSubscribe != 0)// on restart process, numToSubscribe = 0
    {
        numToSubscribe--;
    }
    if (numToSubscribe==0)
        allSubscribed();
}

/************************************************************************************
 * shutdownEventReceived
 *
 * A child process has terminated. This may be as requested or due to a problem.
 * This is called from a QT event loop and so
 * cannot be interrupted via other incoming events. This function saves the most severe
 * error level that is report and this will be used later to determine how much to
 * restart.
 * This function calls shutdownChildren to terminate the MW. As each child terminates,
 * more events will arrive. shutdownChildren cannot be interrupted by other incoming
 * and will only send terminations to children not already terminated.
 *
 ************************************************************************************/
void MwStartup::shutdownEventReceived(ErrorLevel level,QByteArray processId)
{
    bool noAction=false;

}

/************************************************************************************
 * shutdownChildren
 *
 * Called to request the shutdown of a subset of children. As each child terminates,
 * more events will arrive. shutdownChildren cannot be interrupted by other incoming
 * and will only send terminations to children not already terminated.
 *
 * If during shutdown, errorLevel indicates an error then this function will ignore the
 * subset and instead terminate all children
 *
 ************************************************************************************/
void MwStartup::shutdownChildren(ActivationEvent event)
{

}

void MwStartup::amTerminationTimedout()
{
    qDebug() << "ERROR: Timeout whilst terminating the children. Restart the STB";
    errorLevel=STB_LEVEL_ERR;
    selfTerminate();
}

void MwStartup::selfTerminate()
{
}

#ifdef TEST_BUILD
void MwStartup::testInstructionMsgReceived(QByteArray msg)
{
    Package *package;
    bool sent=false;
    QList<QByteArray> msgData=msg.split('@');

    qDebug() <<"appman::mwstartup::testInstructionMsgReceived - " << msgData[0] << " for " << msgData[1]);

    // Is the instruction for us?
    if (msgData[0].startsWith(TEST_APPMAN_UNSET_STABILITY_CHECK_MSG))
        startupStabilityCheck=false;
    else if (msgData[0].startsWith(TEST_APPMAN_FORCE_RESTART_MSG))
        forcedRebootRequest();
    else if (msgData[0].startsWith(TEST_APPMAN_UI_RESTART_MSG_TYPE))
    {
        if (msgData[0].startsWith(TEST_APPMAN_UI_RESTART_STB_MSG))
            emit askUIForRebootRequest(RESTART_STB);
        else if (msgData[0].startsWith(TEST_APPMAN_UI_RESTART_UI_ONLY_MSG))
            emit askUIForRebootRequest(RESTART_UI_ONLY);
        else if (msgData[0].startsWith(TEST_APPMAN_UI_RESTART_MW_AND_UI_MSG))
            emit askUIForRebootRequest(RESTART_MW_AND_UI);
        else if (msgData[0].startsWith(TEST_APPMAN_UI_RESTART_MW_ONLY_MSG))
            emit askUIForRebootRequest(RESTART_MW_ONLY);
        else if (msgData[0].startsWith(TEST_APPMAN_UI_RESTART_APPMAN_MSG))
            emit askUIForRebootRequest(RESTART_APPMAN);
    }
    else
    {
        // Determine the recepient child process
        if (msgData.count()>1 && msgData[1].length()>0)
        {
            foreach (package,packageList)
            {
                QByteArray packageId=package->getPackageId();
                if (packageId==msgData[1])
                {
                    package->testNotifyInstructionMsgReceived(msgData[0]);
                    sent=true;
                    break;
                }
            }
        }
        else
        {
            qDebug() << "TEST ERROR - Invalid message received from Test Process :" << msg);
            exit(1);
        }
        if (!sent)
        {
            qDebug() <<"appman::mwstartup::testInstructionMsgReceived - " << msgData[0] << " for " << msgData[1]);
            exit(1);
        }
    }
}
#endif
