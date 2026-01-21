#ifndef MWAPP_H
#define MWAPP_H

#include <QObject>
#include "appmanif.h"

class MwApp : public QObject
{
    Q_OBJECT

public:
    MwApp();
    ~MwApp();
    void initialise(void);
private slots:
    void subscribe(void);
    void handlePowerRestartNotification(StateChange state);
    void handleStartupException(ErrorLevel errLevel);
    void heartbeat();
private:
    AppManIf *startup;
};

#endif // MWAPP_H
