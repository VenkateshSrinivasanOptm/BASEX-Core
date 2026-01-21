#include <QDebug>
#include <QFile>
#include "mwconfig.h"
#include "appmanmsgif.h"
#include <QCoreApplication>
#include <QDir>

MwConfig::MwConfig()
{
    packageCount=0;
    heartbeatPeriod=30000; //Default heartbeat period is 30 secs
    stabilityTimeout=600000; // Default 10 mins of no error after any reboot.
    amStartupTimeout=60000; // Default timeout to achieve MW Startup
    packageTerminateTimeout=30000; // Default timeout for a child to die before killing it
    uiRestartRequestTimeout=1800000; //30 mins
    minEPGhrs=12;
    gdbDumpInProgress.release();
}
QString MwConfig::createConfigFileFromResource(const QString &resourcePath, const QString &fileName) {
    // Get the executable directory
    QString exeDirPath = QCoreApplication::applicationDirPath();
    QString filePath = QDir(exeDirPath).filePath(fileName); // Construct the full path

    QFile resourceFile(resourcePath);
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open resource file:" << resourcePath;
        return QString();
    }

    // Create an actual file in the executable directory with the specified name
    QFile actualFile(filePath);
    if (actualFile.open(QIODevice::WriteOnly)) {
        actualFile.write(resourceFile.readAll()); // Copy contents from the resource file
        actualFile.close();
        return actualFile.fileName(); // Return the full path of the created file
    } else {
        qWarning() << "Could not create file at:" << filePath;
        return QString();
    }
}


void MwConfig::acquire()
{

    QFile file(getenv("APPMAN_CFG_FILE"));
    QByteArray line;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString configFilePath = createConfigFileFromResource(":/appman", "appman");
        file.setFileName(":/appman");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           qDebug() << "Unable to connect to config file. Try setting env var APPMAN_CFG_FILE" ;
           qDebug("Unable to connect to config file. Try setting env var APPMAN_CFG_FILE\n");
           exit(STB_LEVEL_ERR);
        }
    }

    line=file.readLine();
    line.remove(0,line.indexOf(':')+1);
    qDebug() << line ;
    packageCount=atoi(line);
    qDebug() <<  packageCount ;
    while (!file.atEnd())
    {
        line=file.readLine().trimmed();
        if (line.startsWith("appName"))
        {
            line.remove(0,line.indexOf(':')+1);
            packageId.append(line);
        } else if (line.startsWith("pathName"))
        {
            line.remove(0,line.indexOf(':')+1);
            if (line[0]=='$')
            {
                int i;
                QByteArray envVar;
                i=2;  //skip '$' and assume a name of at least one character
                while (i<line.count() && line[i]!='\\' && line[i]!='/')
                    i++;
                envVar=getenv(line.mid(1,i-1));
                envVar.append(line.right(line.count() - i));
                line=envVar;
            }
            packagePath.append(line);
        } else if (line.startsWith("serviceName"))
        {
            ServicesList service;
            line.remove(0,line.indexOf(':')+1);
            service.serviceName=line;
            mwServiceList.append(service);
            mwServNameList.append(line);
        } else if (line.startsWith("docName"))
        {
            BrokerDocument doc;
            line.remove(0,line.indexOf(':')+1);
            doc.docName=line;
            // Ignore any documents that are not associated to a service.
            if (mwServiceList.count()>0)
            {
                line=file.readLine().trimmed();
                line.remove(0,line.indexOf(':')+1);
                doc.docPriority=atoi(line);
                mwServiceList.last().documents.append(doc);
            }

        } else if (line.startsWith("gdbScriptFile"))
        {
            line.remove(0,line.indexOf(':')+1);
            if (line[0]=='$')
            {
                int i;
                QByteArray envVar;
                i=2;  //skip '$' and assume a name of at least one character
                while (i<line.count() && line[i]!='\\' && line[i]!='/')
                    i++;
                envVar=getenv(line.mid(1,i-1));
                envVar.append(line.right(line.count() - i));
                line=envVar;
            }
            gdbScriptFile.append(line);
        } else if (line.startsWith("heartbeatMS"))
        {
            line.remove(0,line.indexOf(':')+1);
            heartbeatPeriod=atoi(line);
        } else if (line.startsWith("stabilityTimeout"))
        {
            line.remove(0,line.indexOf(':')+1);
            stabilityTimeout=atoi(line);
        } else if (line.startsWith("startupTimeout"))
        {
            line.remove(0,line.indexOf(':')+1);
            amStartupTimeout=atoi(line);
        } else if (line.startsWith("shutdownTimeout"))
        {
            line.remove(0,line.indexOf(':')+1);
            packageTerminateTimeout=atoi(line);
        }else if (line.startsWith("minEPGhrs"))
        {
            line.remove(0,line.indexOf(':')+1);
            minEPGhrs=atoi(line);
        } else if (line.startsWith("UIrestartRequestTimeout"))
        {
            line.remove(0,line.indexOf(':')+1);
            uiRestartRequestTimeout=atoi(line);
        }

    }

    // Check we have one packageId per packagePath
    if(packageId.count()!=packagePath.count())
    {
        qDebug() << "ERROR: Invalid AppMan config file.Counted " << packageId.count() << " packages and " << packagePath.count() << " package paths" ;
        exit(STB_LEVEL_ERR);
    }
}

MwConfig* MwConfig::pInstance = NULL;

void MwConfig::instantiate() {
    pInstance = new MwConfig();
}

int MwConfig::getHeartbeatPeriod()
{
    return heartbeatPeriod;
}

QByteArray MwConfig::getGdbScriptFile()
{
    return gdbScriptFile;
}

MwConfig *MwConfig::getInstance() {
    return pInstance;
}

int MwConfig::getPackageCount()
{
    return packageCount;
}
int MwConfig::getAMStartTimeout()
{
    return amStartupTimeout;
}
int MwConfig::getPackageTerminateTimeout(void)
{
    return packageTerminateTimeout;
}
int MwConfig::getStabilityTimeout()
{
    return stabilityTimeout;
}
int MwConfig::getUIrestartRequestTimeout()
{
    return uiRestartRequestTimeout;
}
QByteArray MwConfig::getPackagePath(int idx)
{
    return packagePath[idx];
}

QByteArray MwConfig::getPackageId(int idx)
{
    return packageId[idx];
}
