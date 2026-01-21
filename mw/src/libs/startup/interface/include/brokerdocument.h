#ifndef BROKERDUMENT_H
#define BROKERDUMENT_H
#include <QByteArray>
#include <QList>

struct BrokerDocument
{
    QByteArray docName;
    quint8 docPriority;
};

struct ServicesList
{
    QByteArray serviceName;
    QList<BrokerDocument> documents;
};

#endif // BROKERDUMENT_H
