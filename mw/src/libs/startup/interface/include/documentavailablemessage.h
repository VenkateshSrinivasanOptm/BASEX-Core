#ifndef DOCUMENTAVAILABLEMESSAGE_H
#define DOCUMENTAVAILABLEMESSAGE_H

#include <comlibservices.h>
#include <message.h>

struct DocumentAvailable: public Message
{
    QByteArray document;
    DocumentAvailable(): Message(BLSTARTUP_DOCUMENT_AVAILABLE) {}
    DocumentAvailable(QByteArray const &_document):
            Message(BLSTARTUP_DOCUMENT_AVAILABLE), document(_document) {}
    virtual ~DocumentAvailable() {}
    virtual void serialize(QDataStream &stream) const
    {
        stream << document;
    }
    virtual void deserialize(QDataStream &stream)
    {
        stream >> document;
    }
};

#endif // DOCUMENTAVAILABLEMESSAGE_H
