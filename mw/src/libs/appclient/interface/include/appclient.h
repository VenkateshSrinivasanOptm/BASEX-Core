#ifndef APPCLIENT_H
#define APPCLIENT_H

#include <QObject>
#include <appmanif.h>

class AppClient: public QObject
{
    Q_OBJECT
    public:

        AppClient(QString const & commCfg);

    private slots:

        void subscribe();
        void handlePowerRestartNotification(StateChange state);
        void handleStartupException(ErrorLevel errLevel);

    private:

        AppManIf &appManIf;
        void init(QString const &commCfg);
};


#endif // APPCLIENT_H
