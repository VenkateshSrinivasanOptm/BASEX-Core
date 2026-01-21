QT += sql serialbus serialport
VERSION = 0.0.1
TEMPLATE = lib
DESTDIR = ../fotaservice/interface/lib
TARGET = fotaservice
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
                 ../startup/interface/include

INCLUDEPATH += ../appmanif/interface/include
INCLUDEPATH += ../appmanifmsgmgr/interface/include

SOURCES += \
    impl/src/fotacbuscomm.cpp \
    impl/src/fotaservice.cpp

HEADERS += \
    impl/src/fotacbuscomm.h \
    interface/include/fotaservice.h

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

