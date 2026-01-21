#ifndef FOTASERVICE_H
#define FOTASERVICE_H

#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include <initializationmessage.h>

#define TESTING
#define MWNAMESTR "fotaservice"
#define MSVERSTR "1.0"

class FotaCBusComm;

class FotaService : public QObject
{
    Q_OBJECT
public:
    static FotaService& getInstance();

    /* initialisation method for startup sequence */
    void initialise(void);

public slots:
    void processStart(QSharedPointer<InitializationMessage const>);

private:
    FotaService();
    virtual ~FotaService();
    Q_DISABLE_COPY(FotaService);
    QSharedPointer<FotaCBusComm> m_CBusComm;
};

#endif // FOTASERVICE_H
