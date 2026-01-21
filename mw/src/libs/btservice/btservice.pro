QT += bluetooth serialport serialbus
TEMPLATE = lib
DESTDIR = ../btservice/interface/lib
TARGET = btservice
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
               impl/src \
               ../startup/interface/include \
               ../canservice/interface/include \
                   ../startup/interface/include \
                 ../canlib/impl/src

INCLUDEPATH += ../appmanif/interface/include
INCLUDEPATH += ../appmanifmsgmgr/interface/include

SOURCES += \
    bluetoothmanager.cpp \
    impl/src/btcbuscomm.cpp \
    impl/src/btservice.cpp

HEADERS += \
    bluetoothmanager.h \
    impl/src/btcbuscomm.h \
    interface/include/btservice.h

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
