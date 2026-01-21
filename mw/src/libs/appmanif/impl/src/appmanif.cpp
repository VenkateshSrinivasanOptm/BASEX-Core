#include <QMetaType>
#include <QDebug>
#include "appmanif.h"
#include "eventwatcher.h"

#ifdef TEST_BUILD
#include "appmantest.h"
#endif

static StateChange currPowerState=POWER_RESET_STATE_UNKNOWN;

AppManIf::AppManIf()/*: m_pLogger(getLogger("appmanif"))*/
{
#ifdef TEST_BUILD
    stopHeartbeat=false;
    hang_on_terminate=false;
    hang_on_subscribed=false;
    permit_restart=true;
#endif
    event = new EventWatcher();
    qRegisterMetaType<stdioMessage>("stdioMessage");
    if (!connect(event, SIGNAL(eventFired(stdioMessage)), this, SLOT(handleMessage(stdioMessage))))
    {
        qDebug() << "ERROR - Failed signal slot connection for stdio with AppMan" ;
    }
    event->start();
}

AppManIf::~AppManIf()
{
    delete event;
}


AppManIf *AppManIf::getInstance() {
    static AppManIf instance;
    return &instance;
}

void AppManIf::notifyPublished(const char *lappName, const char *lappVersion)
{
    //Published Event Notification
    appName=lappName;
    appVersion=lappVersion;
    MsgManager::sendPublished(lappName,lappVersion);
}

void AppManIf::initialise()
{
}
void AppManIf::sendHeartbeatResponse()
{
#ifdef TEST_BUILD
    if (!stopHeartbeat)
#endif
    //Heartbeat to prove we are alive
    MsgManager::returnHeartbeat();
}
void AppManIf::communicationEstablished()
{
#ifdef TEST_BUILD
            if (hang_on_subscribed)
                sleep(1000); //Long enough to cause a big problem
#endif
    //Communication Established (Subscribed) Event Notification
    MsgManager::sendCommunicationEstablished();
}
void AppManIf::uiPowerChangeNotification(UiPowerNotification state)
{
    MsgManager::sendUiPowerChangeNotification(state);
}
StateChange AppManIf::getCurrentPowerState()
{
    return currPowerState;
}

void AppManIf::swRestartResponse(bool response)
{
    MsgManager::sendUiRestartResponse(response);
}

void AppManIf::handleMessage(stdioMessage msg)
{
    MsgManager message(msg);


    switch(message.msgId())
    {
        case MESSAGE_SUBSCRIBE: // subscribe message
            // collect the current power state
            currPowerState=message.msgNotificationState();
            emit establishCommunication();
            break;
        case MESSAGE_TERMINATE:  //restart notification
            event->exit(0);
#ifdef TEST_BUILD
            if (hang_on_terminate)
                sleep(1000); //Long enough to cause a big problem
#endif
        case MESSAGE_NOTIFY:  //Power notification, no restart
            {
                StateChange state=message.msgNotificationState();

                // maintain the current power state for any notification of a power change.
                if (state==POWER_ON || state==POWER_OFF || state==POWER_DEEP_SLEEP || state==POWER_STANDBY)
                    currPowerState=state;

                if (state!=POWER_RESET_STATE_UNKNOWN)
                    emit notifyStateChange(state);
                else
                {
                    emit notifyException(MW_UI_LEVEL_ERR);
                }
            }
            break;
        case MESSAGE_ASK_RESTART:
            break;
        case MESSAGE_HEARTBEAT:
            emit heartbeatNotification();
            break;
        case MESSAGE_UNKNOWN_TYPE:
        default:
#ifdef TEST_BUILD
            if (!testNotificationMessage(msg.msg))
#endif
            {
               qDebug() << "ERROR dont know how to process msg "  << (char) message.msgId() << "msg " << message.serialize().msg;
               //qDebug() << "ERROR dont know how to process msg " << (char) message.msgId() << "msg " << message.serialize().msg;
               emit notifyException(MW_UI_LEVEL_ERR);
            }
            break;
    }
}

void AppManIf::handleMsgException(ErrorLevel errLev)
{
    emit notifyException(errLev);
}

#ifdef TEST_BUILD
bool AppManIf::testNotificationMessage(QByteArray msg)
{
    qDebug() << "child::appManIf::testNotificationMessage " << msg << "\n";
    fflush(stderr);
    if (msg.startsWith(TEST_APPMAN_MSG))
    {
        fflush(stderr);
        //This is a test message
        if (msg.startsWith(TEST_APPMAN_POWER_OFF_MSG))
        {
            // We are the UI and the test wants us to notify a power change to off
            uiPowerChangeNotification(UI_NOTIFY_POWER_OFF);
        }
        if (msg.startsWith(TEST_APPMAN_POWER_DEEP_SLEEP_MSG))
        {
            // We are the UI and the test wants us to notify a power change to deep sleep
            uiPowerChangeNotification(UI_NOTIFY_POWER_DEEP_SLEEP);
        }
        if (msg.startsWith(TEST_APPMAN_POWER_ON_MSG))
        {
            // We are the UI and the test wants us to notify a power change to on
            uiPowerChangeNotification(UI_NOTIFY_POWER_ON);
        }
        if (msg.startsWith(TEST_APPMAN_POWER_STANDBY_MSG))
        {
            // We are the UI and the test wants us to notify a power change to standby
            uiPowerChangeNotification(UI_NOTIFY_POWER_STANDBY);
        }
        if (msg.startsWith(TEST_APPMAN_STOP_HEARTBEAT_MSG))
        {
            stopHeartbeat=true;
        }

        if (msg.startsWith(TEST_APPMAN_CHILD_DIE_WITH_MW_RESTART_MSG))
        {
            emit notifyException(MW_UI_LEVEL_ERR);
        }

        if (msg.startsWith(TEST_APPMAN_CHILD_DIE_WITH_STB_RESTART_MSG))
        {
            emit notifyException(STB_LEVEL_ERR);
        }

        if (msg.startsWith(TEST_APPMAN_CHILD_DIE_WITH_SUCCESS_MSG))
        {
            emit notifyException(EXIT_NO_ERROR);
        }

        if (msg.startsWith(TEST_APPMAN_HANG_ON_SUBSCRIBED_MSG))
        {
            hang_on_subscribed=true;
        }

        if (msg.startsWith(TEST_APPMAN_HANG_ON_TERMINATE_MSG))
        {
            hang_on_terminate=true;
        }
        if (msg.startsWith(TEST_APPMAN_UI_TO_PERMIT_RESTART_MSG))
        {
            permit_restart=true;
        }
        if (msg.startsWith(TEST_APPMAN_UI_TO_DENY_RESTART_MSG))
        {
            qDebug() << "Deny Restart";
            permit_restart=false;
        }
        return true;
    }
    else
        return false;
}
#endif
