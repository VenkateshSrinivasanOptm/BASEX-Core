#ifndef STARTUPSERVICE_H_
#define STARTUPSERVICE_H_

#include<QObject>
#include <brokerdocument.h>

class StartupServicePrivate;

class StartupService
{
    public:
      static StartupService & instance() {
            static StartupService instance;
            return instance;
      }

      /**
        * calling this method starts the BL process
        **/
    void init(QList<QByteArray> const &mwServiceList, QList<ServicesList> const &brokerDocumentList,
              quint16 minEPGhrs);

    public slots:


    private:

      StartupService();
      virtual ~StartupService();
      StartupServicePrivate *m_startupserviceP;


};

#endif /* STARTUPSERVICE_H_ */
