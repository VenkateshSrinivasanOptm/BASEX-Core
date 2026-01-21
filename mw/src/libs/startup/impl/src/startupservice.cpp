#include "startupservice.h"
#include "startupserviceprivate.h"
#include <QList>


StartupService::StartupService(){
	m_startupserviceP = new StartupServicePrivate();
}

StartupService::~StartupService() {
	delete m_startupserviceP;
}



void StartupService::init(QList<QByteArray> const &mwServiceList, QList<ServicesList> const &brokerDocumentList,
                          quint16 minEPGhrs)
{
       m_startupserviceP->init(mwServiceList, brokerDocumentList, minEPGhrs);
}

