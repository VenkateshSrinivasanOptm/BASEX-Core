#include "mwapp.h"

MwApp::MwApp()
{
}
MwApp::~MwApp()
{
}
/************************************************************************************
 * initialise
 *
 * When your process is first forked it will then be instructed to initialise. It is
 * here that you must publish your interfaces and send back the notification when completed
 * you will then go idle until you are signalled to subscribe
 *
 ************************************************************************************/
void MwApp::initialise()
{
    startup=AppManIf::getInstance();
    connect(startup, SIGNAL(establishCommunication()), this, SLOT(subscribe()));
    connect(startup, SIGNAL(notifyStateChange(StateChange)), this, SLOT(handlePowerRestartNotification(StateChange)));
    connect(startup, SIGNAL(notifyException(ErrorLevel)), this, SLOT(handleStartupException(ErrorLevel)));
    connect(startup, SIGNAL(heartbeatNotification()), this, SLOT(heartbeat()));

    startup->initialise();

    StateChange currPower=AppManIf::getCurrentPowerState();
    if (currPower!=POWER_RESET_STATE_UNKNOWN)
    {
        qDebug() << "ERROR - currPower error";
        return;
    }

    // Publish
    // ...

    startup->notifyPublished("mwNameStr","msVerStr"); //Published

}

void MwApp::heartbeat()
{
    startup->sendHeartbeatResponse();
}

/************************************************************************************
 * subscribe
 *
 * Here you can subscribe and notify when you have finished. When you exit this method
 * your module will go idle until it receives a request. Prior to exiting you may also
 * wish to initiate anything required of you.
 ************************************************************************************/
void MwApp::subscribe(void)
{
    StateChange currPower=AppManIf::getCurrentPowerState();

    qDebug() << "Start power state == " << currPower;

    // Subscribe
    // ...
    startup->communicationEstablished();   //Subscribed

    // STARTUP COMPLETE.
    // The code will now drop to the event loop awaiting work for your module or you
    // should add code to initiate something yourself.
    qDebug() << "started\n";
}

/************************************************************************************
 * handlePowerRestartNotification
 *
 * At anytime you may be asked to restart due to software upgrade or system error
 * If you are the UI then you may also be notified that the MW is restarting MW_OTHER and
 * you may receive notification when restarting has completed. Please refer to the
 * Application Manager Design document for further information on this.
 *
 * At anytime you may be informed of a power change to standby, off or deep sleep
 *
 * What you implement for these notifications is personal to you. Below is a minimal example.
 *
 ************************************************************************************/
void MwApp::handlePowerRestartNotification(StateChange state)
{
    switch (state)
    {
        case POWER_ON:           // We have come from standby to on.
            qDebug() << "handlePowerRestartNotification(POWER_ON)\n";
            break;
        case POWER_STANDBY:      // power notification that we are going to standby, no restart
            qDebug() << "handlePowerRestartNotification(POWER_STANDBY)\n";
            break;
        case RESTART_OTHER:      // notification that other MW modules are having to restart, no restart
            qDebug() << "handlePowerRestartNotification(RESTART_OTHER)\n";
            break;
        case RESTARTED_MW_ACTIVE: // notification that the MW has completed a restart and all components are online.
            qDebug() << "handlePowerRestartNotification(RESTARTED_MW_ACTIVE)\n";
            break;
        case POWER_OFF:          // Power notification that the box has been switched off. termination required
            qDebug() << "handlePowerRestartNotification(POWER_OFF)\n";
            exit(0);
        case POWER_DEEP_SLEEP:   // Power notification that the box has been set to deep sleep. termination required
            qDebug() << "handlePowerRestartNotification(POWER_DEEP_SLEEP)\n";
            exit(0);
        case RESTART:            // Notification that you need to restart due to new software or error. termination required.
            qDebug() << "handlePowerRestartNotification(RESTART)\n";
            exit(0);
        default:
            qDebug() << "ERROR: handlePowerRestartNotification(" << state << ")\n";
            exit(STB_LEVEL_ERR);
    }
}
/************************************************************************************
 * handleStartupException
 *
 * The Startup API might error. If so then it will notify you via this handler and
 * you need to act on this by shutting down. The restart error level can be one of
 *    MW_UI_LEVEL_ERR - The UI and MW need to be restarted
 *    STB_LEVEL_ERR - The error requires a reboot of the STB
 * When you shut down you should exit with the same error level. If during shutdown
 * you have errors that require a STB reboot then you should return STB_LEVEL_ERROR
 * and ignore the error level you received.
 *
 ************************************************************************************/
void MwApp::handleStartupException(ErrorLevel errLevel)
{
    exit(errLevel);
}
