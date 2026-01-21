QT += sql network serialbus serialport
TEMPLATE = lib
DESTDIR = ../uploadservice/interface/lib
TARGET = uploadservice
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
               ../canservice/interface/include \
               ../startup/interface/include \
               impl/src \
               ../canlib/impl/src \
                ../dbservice/interface/include

INCLUDEPATH += ../appmanif/interface/include
INCLUDEPATH += ../appmanifmsgmgr/interface/include

SOURCES += \
    impl/src/uploadcbuscom.cpp \
    impl/src/uploadservice.cpp \
    impl/src/tcpclient.cpp

HEADERS += \
    impl/src/uploadcbuscomm.h \
    impl/src/tcpclient.h \
    interface/include/uploadservice.h


LIBS += -L../communicationlib/interface/lib \
         -lcommunicationlib


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
