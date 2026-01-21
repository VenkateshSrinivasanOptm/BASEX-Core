#ifndef CBUSCONFIG_H
#define CBUSCONFIG_H

#include<QString>

class CBusConfig
{
public:

    /**
      * this method reads the config file for the communication library. Before using the communication
      * library this method must be called first. Each process has a unique config file.
      * the method returns false if the config file is not found or the cbus is already initialized
      **/

    static bool initialize(QString const &configfile);

private:
    CBusConfig(){}
    virtual ~CBusConfig(){}
    Q_DISABLE_COPY(CBusConfig);
};

#endif // CBUSCONFIG_H
