#ifndef GSMSERVICEMESSAGES_H
#define GSMSERVICEMESSAGES_H

#include <comlibservices.h>
#include <message.h>
#include <QVariant>


/* update connection manager about gsm signal strength */
struct GsmSignalStrengthUpdate : public Message {
  int data;
    GsmSignalStrengthUpdate() : Message(GSM_SIGNAL_UPDATE){}
  GsmSignalStrengthUpdate(const int& Data)
      : Message(GSM_SIGNAL_UPDATE) {
    data = Data;
  }
  virtual ~GsmSignalStrengthUpdate() {}

  virtual void serialize(QDataStream &stream) const { stream << data;

  }

  virtual void deserialize(QDataStream &stream) { stream >> data;
  }

};

#endif // GSMSERVICEMESSAGES_H
