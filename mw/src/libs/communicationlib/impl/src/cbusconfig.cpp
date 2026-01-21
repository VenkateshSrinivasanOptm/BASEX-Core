#include"cbusconfig.h"
#include"cbusinterface.h"

#include"cbusconfigprivate.h"

bool CBusConfig::initialize(QString const &configFile)
{
    bool value=CBusConfigPrivate::instance().initialize(configFile);
    if(value) {
        // start CBUS
        CBusInterface::instance();
    }
    else {
        //TODO error logging
    }
    return value;
}
