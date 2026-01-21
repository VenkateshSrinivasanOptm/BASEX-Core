QT += sql serialbus serialport
VERSION = 0.0.1
TEMPLATE = lib
DESTDIR = ../dbservice/interface/lib
TARGET = dbservice
CONFIG += c++17
MOC_DIR = moc
OBJECTS_DIR = obj
CONFIG += plugin
CONFIG += no_lflags_merge

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += interface/include \
               ../communicationlib/interface/include \
               ../uploadservice/impl/src \
               ../canservice/interface/include \
               impl/src \
                ../canlib/impl/src \
               ../../modules/hmiservice

INCLUDEPATH += ../appmanif/interface/include
INCLUDEPATH += ../appmanifmsgmgr/interface/include

SOURCES += \
    impl/src/dbcbuscomm.cpp \
    impl/src/dbservice.cpp

HEADERS += \
    impl/src/dbcbuscomm.h \
    interface/include/dbservice.h

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

