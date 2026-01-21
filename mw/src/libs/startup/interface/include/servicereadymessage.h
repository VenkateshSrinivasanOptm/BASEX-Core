/*
 * servicereadymessage.h
 *
 *
 *  Created on: 15-sep-2010
 *      Author: sdelaet
 *
 *
 * Message to be send from seperate services to STARTUP_SERV when the service has finished its initialization.
 * The STARTUP_SERV will then notify the UI that this service is ready.
 */

#ifndef SERVICEREADYMESSAGE_H_
#define SERVICEREADYMESSAGE_H_
#include <message.h>
#include <comlibservices.h>


struct ServiceReadyMessage : public Message
{
        // the service Names are defined in the commlibserv.h file
        QByteArray serviceName;
        ServiceReadyMessage(QByteArray _serviceName):Message(BLSTARTUP_SERVICEREADY), serviceName(_serviceName){}
	ServiceReadyMessage():Message(BLSTARTUP_SERVICEREADY){}
	virtual ~ServiceReadyMessage(){}

	virtual void serialize(QDataStream &stream) const
	{
                stream <<  serviceName;
	}

	virtual void deserialize(QDataStream &stream)
	{
                stream >> serviceName;
	}

};

#endif /* SERVICEREADYMESSAGE_H_ */
