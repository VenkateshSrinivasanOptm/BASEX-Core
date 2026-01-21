QT += core
VERSION = 0.0.1
TEMPLATE = lib
DESTDIR = ../rfidservice/interface/lib
TARGET = rfidservice
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
               ../../modules/hmiservice \
               ../startup/interface/include

INCLUDEPATH += ../appmanif/interface/include
INCLUDEPATH += ../appmanifmsgmgr/interface/include
INCLUDEPATH += /usr/include/PCSC

INCLUDEPATH += /opt/fsl-imx-wayland/5.15-kirkstone/sysroots/armv8a-poky-linux/usr/include/PCSC

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -lpcsclite


SOURCES += \
    impl/src/rfidcbuscomm.cpp \
    impl/src/rfidservice.cpp \
     impl/src/rfidreader.cpp

HEADERS += \
    impl/src/rfidcbuscomm.h \
    interface/include/rfidservice.h \
    interface/include/rfidservicemessages.h \
    interface/include/rfidreader.h

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -lpcsclite


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
