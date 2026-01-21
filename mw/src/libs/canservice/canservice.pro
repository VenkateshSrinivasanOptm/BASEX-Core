QT += sql serialbus
VERSION = 0.0.1
TEMPLATE = lib
DESTDIR = ../canservice/interface/lib
CONFIG -= rtti
TARGET = canservice
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
               ../canlib/interface/include \
               ../canlib/impl/src \
               ../uploadservice/impl/src \
               ../dbservice/interface/include \
               ../dbservice/impl/src \
               ../uploadservice/interface/include \
               ../uploadservice/impl/src \
               ../startup/interface/include \
               impl/src
               
INCLUDEPATH += ../appmanif/interface/include
INCLUDEPATH += ../appmanifmsgmgr/interface/include
INCLUDEPATH += ../btservice/interface/include
INCLUDEPATH += ../fotaservice/interface/include

HEADERS += \
    interface/include/canservice.h


SOURCES += \
    impl/src/canservice.cpp

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -L../../libs/canlib/interface/lib -lcanlib
LIBS += -L../../libs/btservice/interface/lib -l btservice
LIBS += -L../../libs/fotaservice/interface/lib -lfotaservice

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
