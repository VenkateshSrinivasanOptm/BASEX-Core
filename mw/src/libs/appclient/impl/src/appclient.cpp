#include "appclient.h"

#include<cbusconfig.h>

AppClient::AppClient(QString const &commCfg):
    appManIf(*AppManIf::getInstance())
{
    init(commCfg);
}

void AppClient::init(QString const &commCfg)
{
    CBusConfig::initialize(commCfg);
    connect(&appManIf, SIGNAL(establishCommunication()), this, SLOT(subscribe()));
    connect(&appManIf, SIGNAL(notifyStateChange(StateChange)), this, SLOT(handlePowerRestartNotification(StateChange)));
    connect(&appManIf, SIGNAL(notifyException(ErrorLevel)), this, SLOT(handleStartupException(ErrorLevel)));

}

void AppClient::subscribe()
{
}

void AppClient::handlePowerRestartNotification(StateChange state)
{
}

void AppClient::handleStartupException(ErrorLevel errLevel)
{
}
