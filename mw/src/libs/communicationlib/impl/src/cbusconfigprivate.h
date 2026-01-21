#ifndef CBUSCONFIGPRIVATE_H
#define CBUSCONFIGPRIVATE_H

#include<QString>
#include<QList>
#include"triple.h"

class CBusConfigPrivate
{
public:

    // first element is the module name, second the call address, third the publish address
    typedef Triple<QString,QString,QString> StringTriple;
    static CBusConfigPrivate & instance() {
            static CBusConfigPrivate instance;
            return instance;
    }

    /**
      * this method reads the config file for the communication library. Before using the communication
      * library this method must be called first. Each process has a unique config file.
      * the method returns false if the config file is not found or Cbus is already initialized or running
      **/

    bool initialize(QString const &configFile);

    /**
      * this method returns the process name and the communication address
      **/
    StringTriple const & getModule();

    /**
      * this method gives a list of modules that has to communicate with the running process
      **/
    QList<StringTriple> getRemoteModules();

    /**
      * this method defines the number of threads for incoming requests
      **/
    quint8 getNbrOfInWorkers();

    /**
      * this method defines the number of threads for outgoing requests
      **/
    quint8 getNbrOfOutWorkers();

    void setRunning();


private:
    CBusConfigPrivate();
    virtual ~CBusConfigPrivate(){}
    Q_DISABLE_COPY(CBusConfigPrivate);
    QList<StringTriple> remoteModules;
    StringTriple module;
    quint8 nbrOfInWorkers;
    quint8 nbrOfOutWorkers;
    bool initialized;
    bool running;

};

#endif // CBUSCONFIGPRIVATE_H
