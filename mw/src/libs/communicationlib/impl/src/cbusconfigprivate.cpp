#include "cbusconfigprivate.h"

#include<QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

CBusConfigPrivate::CBusConfigPrivate() :
    nbrOfInWorkers(1), nbrOfOutWorkers(1), initialized(false),
    running(false)
{
}

bool CBusConfigPrivate::initialize(QString const &configFile)
{
    if(initialized)
    {        
        qDebug() << " communication library is already initialized ";
        return false;
    }

    if(running)
    {
        qDebug() << " communication library is already running ";
        return false;
    }

    //TODO read config file
    QFile file(configFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false ;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line[0] != '#') {
            // split in key/value pair
            QStringList l=line.split('=');
            if(l.size()==2) {
                if(l[0]=="module") {
                    QStringList s=l[1].split('|');
#ifdef DEBUG
                    qDebug() << " module" ;
#endif
                    if(s.size()==3){
                        module.x=s[0];
                        module.y=s[1];
                        module.z=s[2];
                    }
                    else if(s.size()==2){
                        module.x=s[0];
                        module.y=s[1];
                        module.z=QString();
                    }
                }
                else if(l[0]=="rmodule"){
#ifdef DEBUG
                    qDebug() << "remote module ";
#endif
                    QStringList s=l[1].split('|');
                    if(s.size()==3){
                        remoteModules.append(StringTriple(s[0],s[1],s[2]));
                    }
                    else if(s.size()==2){
                        remoteModules.append(StringTriple(s[0],s[1],QString()));
                    }
                }
                else if(l[0]=="nbrofinworkers") {
#ifdef DEBUG
                    qDebug() << "nbrOfInWorkers " << l[1];
#endif
                    nbrOfInWorkers = l[1].toShort();
                }
                else if(l[0]=="nbrofoutworkers") {
#ifdef DEBUG
                    qDebug() << "nbrOfOutWorkers " << l[1];
#endif
                    nbrOfOutWorkers = l[1].toShort();
                }
            }
        }
    }
    file.close();
    initialized=true;
    return true;
}

CBusConfigPrivate::StringTriple const & CBusConfigPrivate::getModule()
{
    return module;
}

QList<CBusConfigPrivate::StringTriple> CBusConfigPrivate::getRemoteModules()
{
    return remoteModules;
}

quint8 CBusConfigPrivate::getNbrOfInWorkers()
{
    return nbrOfInWorkers;
}

quint8 CBusConfigPrivate::getNbrOfOutWorkers()
{
    return nbrOfOutWorkers;
}

void CBusConfigPrivate::setRunning()
{
    running=true;
}
