TEMPLATE = lib
VERSION = 0.0.1

# CONFIG += plugin
CONFIG += debug
CONFIG -= rtti
CONFIG += plugin
CONFIG += no_lflags_merge

QT -= gui
TARGET = startup
DESTDIR = ../startup/interface/lib

OBJECTS_DIR = obj
MOC_DIR = moc

INCLUDEPATH += interface/include
INCLUDEPATH += ../communicationlib/interface/include
INCLUDEPATH += ../oci_writer/interface/include
INCLUDEPATH += ../mmddf_writer/interface/include
INCLUDEPATH += ../programinfoquerier/interface/include
INCLUDEPATH += ../dbcommon/interface/include
INCLUDEPATH += ../dalcommon/interface/include
INCLUDEPATH += ../pacman/interface/include
INCLUDEPATH += ../settingsquerier/interface/include

CONFIG += $$(HOSTNAME)
danos:INCLUDEPATH += /home/project/danos/p4/nmc/veyron/dev/mw/src/libs/ociservice/interface/include
else:INCLUDEPATH += ../ociservice/interface/include

HEADERS += impl/src/startupserviceprivate.h \
    interface/include/startupservice.h \
    interface/include/synchronizeddatamessage.h \
    interface/include/initializationmessage.h \
    interface/include/documentavailablemessage.h \
    interface/include/servicereadymessage.h \
    interface/include/unassignedmessage.h \
    interface/include/brokerdocument.h
SOURCES += impl/src/startupservice.cpp \
    impl/src/startupserviceprivate.cpp


LIBS += -L../../libs/communicationlib/interface/lib -l communicationlib

CONFIG += $$(ARCH)

sh4 { 
    LIBS +=  \
        -l \
        hal-sh4
    LIBS +=  \
        -l \
        zmq
}
else { 
    LIBS += -L../../../vendor/0MQ/root/lib \
        -l \
        zmq
    INCLUDEPATH += ../../../vendor/nsnhal/interface/include
}

HEADERS += interface/include/autoconfigmessage.h
