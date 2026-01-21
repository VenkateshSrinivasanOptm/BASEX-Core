#include <QFileInfo>
#include "package.h"
#include "appmanif.h"
#include "msgmanager.h"
#include "mwconfig.h"

Package::Package()
{  
    mwConfig = MwConfig::getInstance();

    process = new QProcess();
    processId=0;
    processState = PACKAGE_DEAD;
    outstandingHeartbeatResponse=false;
    qDebug() << "PACKAGE_CREATING";

    if(!connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(handleMessageNotification())))
    {
        qDebug() << "Error readyReadStandardOutput signal";
    }
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(stderrNotification()));
    connect(process, SIGNAL(started()), this, SLOT(started()));
   // connect(process, SIGNAL(error( QProcess::ProcessError)), this, SLOT(error( QProcess::ProcessError)));
    connect(process, SIGNAL(finished (int , QProcess::ExitStatus )), this, SLOT(finished ( int, QProcess::ExitStatus )));

}
Package::~Package()
{
    delete process;
}

void Package::finished ( int exitCode, QProcess::ExitStatus exitStatus )
{
    if (exitStatus==0)
    {
        qDebug()  << "child " << packageId << " finished. ExitCode=" << exitCode << ", exitStatus=" << exitStatus ;
        childDied((ErrorLevel)exitCode);
    }
    else
    {
        qDebug() << "child " << packageId << " terminated by OS. ExitCode=" << exitCode << ", exitStatus=" << exitStatus ;


        childDied(STB_LEVEL_ERR);
    }
}

void Package::postMortemNotification(void)
{
    postMortemDebug+=postMotemProcess->readAllStandardOutput();
}

void Package::notifyStateChange(StateChange state)
{
    MsgManager *msg;

    if (state==POWER_OFF || state==POWER_DEEP_SLEEP  or state==RESTART)
    {
        processState=PACKAGE_DYING;
        outstandingHeartbeatResponse=false;
        msg = new MsgManager(MESSAGE_TERMINATE,state);
    }
    else
        msg = new MsgManager(MESSAGE_NOTIFY,state);

    qDebug() << "NotifyStateChange " << state  ;
    msg->sendMessage(process);
    delete msg;
}
void Package::childDied(ErrorLevel errLevel)
{
    processState=PACKAGE_DEAD;
    outstandingHeartbeatResponse=false;
    qDebug() << "child died,restart it. " << packageId;
    restart();
    emit shutdown(errLevel, packageId);
}

void Package::error(QProcess::ProcessError err)
{
    qDebug() << "package error " << err  ;
}

void Package::start(QByteArray processId, QString processPath)
{
    packageId=processId;
    qDebug() << "processId " << processId << " processPath " << processPath;
    // Extract any argument list
    arguments=processPath.split(' ', Qt::SkipEmptyParts);

    packagePath=arguments[0];
    qDebug() << packagePath;

    arguments.removeFirst();

    restart();
}

void Package::restart()
{
    QFileInfo info1(packagePath);
    if (!info1.isExecutable())
    {
        qDebug() << "Error: No such executable file" << packagePath ;
        exit(1);
    }

    if (processState!=PACKAGE_DEAD)
    {
        qDebug() << "ERROR appman:package::restart Request to restart " << packageId
                  << " when child not dead yet. Child processState = " << processState ;
    }
    else
    {
        if (arguments.count()>0)
            process->start(packagePath,arguments);
        else
            process->start(packagePath);

        qDebug() << "package start" << packageId << ":" << packagePath ;
        processState=PACKAGE_CREATED;
        outstandingHeartbeatResponse=false;
    }

}

void Package::started()
{
    // The started notification can lag the published notification. The started notification is internal to
    // QT and they are at liberty to delay it I guess.
    if (processState==PACKAGE_CREATED)
        processState=PACKAGE_STARTED;
    processId=process->processId();
    qDebug() << "PACKAGE_STARTED - " << packageId << ", pid = " << processId;

}

void Package::sendSubscribeMessage(char powerState)
{
    MsgManager::sendEstablishCommunication(process, powerState);
    qDebug() << "send subscribe" ;
}

void Package::startupTimedout()
{
    qDebug() << "Package::startupTimedout() " << processState;
    switch (processState)
    {
        case PACKAGE_CREATED:
            qDebug() << "ERROR starting child process " << packageId ;
            break;
        case PACKAGE_STARTED:
            qDebug() << "ERROR child process " << packageId << " timed out whilst publishing its interface ";
            break;
        case PACKAGE_PUBLISHED:
            qDebug() << "ERROR child process " << packageId << " timed out whilst waiting for subscribed notification " ;
            break;
        case PACKAGE_SUBSCRIBED:
            return;
        default:
            // Death hit us whilst we are still being born. Time to restart the STB
            childDied(STB_LEVEL_ERR);
            return;
    }
    childDied(MW_UI_LEVEL_ERR);
}

void Package::handleMessageNotification()
{
    switch (processState)
    {
        case PACKAGE_CREATED:
        case PACKAGE_STARTED:

            processState=PACKAGE_PUBLISHED;

            // Collect the package Name and version string
            if (MsgManager::getPublished(process, packageNameVer)!=true)
            {
                qDebug() << "ERROR " << packageId << ". Published notification not received. Child state = " << processState ;
            }

            qDebug() << "PACKAGE_PUBLISHED " << packageId << " = " << packageNameVer ;
            // Notify that the package has completed publication
            emit published();

            break;
        case PACKAGE_PUBLISHED:
            processState=PACKAGE_SUBSCRIBED;

            if (MsgManager::getCommunicationEstablished(process)!=true)
            {
                qDebug() << "ERROR " << packageId << ". Subscribed notification not received. Child state = " << processState;
            }
            // Notify that the package has completed subscribing
            qDebug() << "PACKAGE_SUBSCRIBED" << packageId ;
            emit subscribed();
            return;
        case PACKAGE_SUBSCRIBED:
            {
               MsgManager rcvMsg;
               rcvMsg.receiveMessage(process);
               switch (rcvMsg.msgId())
               {
               case MESSAGE_ASK_RESTART_RESPONSE:
                   qDebug() << "received MESSAGE_ASK_RESTART_RESPONSE. Sending request to UI";
                   emit swRestartResponse(rcvMsg.getUiRestartResponse());
                   break;
               case MESSAGE_UI_CHANGE_OF_POWER:
                   qDebug() << "MESSAGE_UI_CHANGE_OF_POWER to " << rcvMsg.getUiPowerNotificationState();
                   emit uiPowerChangeNotification(rcvMsg.getUiPowerNotificationState());
                   break;
               case MESSAGE_HEARTBEAT:
                   qDebug() << "MESSAGE_HEARTBEAT from " << packageId;
                   outstandingHeartbeatResponse=false;
                   break;
               default:
                   qDebug() << "ERROR module " << packageId << " sent unrecognised msg " <<  (rcvMsg.serialize()).msg;
                   break;
               }
            }
            break;
        default:
            MsgManager rcvMsg;
            rcvMsg.receiveMessage(process);
            qDebug() << "ERROR appman::package() invalid message for state. " << packageId << " curr state = " << processState << " msgId received =" << rcvMsg.msgId();
            break;
    }
}
void Package::stderrNotification(void)
{
    qDebug() << packageId << " : " << process->readAllStandardError();
    fflush(stderr);
}

QByteArray Package::getPackageId()
{
    return packageId;
}

QString Package::getPackagePath()
{
    return packagePath;
}

PACKAGE_STATE Package::getProcessState()
{
    return processState;
}
bool Package::sendHeartbeat()
{
    if (processState==PACKAGE_SUBSCRIBED)
    {
        if (outstandingHeartbeatResponse)
        {
            qDebug() << "kill process " << process->processId();
            process->kill();  //Terminate the process.
            processState=PACKAGE_DEAD;
            restart();
            return false;
        }
        qDebug() << "send hearbeat to " << process->processId();
        outstandingHeartbeatResponse=true;
        MsgManager::sendHeartbeat(process);
        return true;
    }
    else
        outstandingHeartbeatResponse=false;
    return true;
}

#ifdef TEST_BUILD
void Package::testNotifyInstructionMsgReceived(QByteArray msg)
{
    process->write("`");  //Send the guard
    process->write(msg);
    process->write("\n");
}
#endif


