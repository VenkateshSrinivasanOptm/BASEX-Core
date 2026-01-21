QT += sql serialbus serialport
VERSION = 0.0.1
TEMPLATE = lib
DESTDIR = ../connectionmanagerservice/interface/lib
TARGET = connectionmanagerservice
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
               ../../modules/hmiservice \
               ../startup/interface/include \
               ../wifiservice/interface/include \
               ../gsmservice/interface/include

INCLUDEPATH += ../appmanif/interface/include
INCLUDEPATH += ../appmanifmsgmgr/interface/include

SOURCES += \
    impl/src/connectionmanagercbuscomm.cpp \
    impl/src/connectionmanagerservice.cpp

HEADERS += \
    impl/src/connectionmanagercbuscomm.h \
    interface/include/connectionmanagerservice.h \
    interface/include/connectionmanagerservicemessages.h

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -L../../libs/wifiservice/interface/lib -lwifiservice
LIBS += -L../../libs/gsmservice/interface/lib -lgsmservice


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

