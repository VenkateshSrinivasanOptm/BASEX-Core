#ifndef CANLIB_H
#define CANLIB_H

#include <QObject>

#include "canparser.h"

class Canlib : public QObject
{
    Q_OBJECT
public:
    static Canlib& getInstance()
    {
        static Canlib instance;
        return instance;
    }

    const CanParser &getCanParser() const;
signals:
    int batteryStatus(int bateryId);
    /* emit this signal to write can data in DB and send to server*/
    void CANDataToWriteInDB(const QString& canData);

private:

    // methods
    Canlib();
    Q_DISABLE_COPY(Canlib);
    virtual ~Canlib();

    // members

    CanParser m_canParser;
    void initialise();
};

#endif // CANLIB_H
