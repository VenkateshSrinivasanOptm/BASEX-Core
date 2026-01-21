QT = core network serialbus serialport
CONFIG += c++17 cmdline
TEMPLATE = app
CONFIG += no_lflags_merge
QT -= gui
CONFIG -= rtti
TARGET = downloadservice
MOC_DIR = moc
DESTDIR = ../interface/bin
OBJECTS_DIR = obj
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../libs/downloadservice/interface/include \
../../libs/communicationlib/interface/include \
 ../../libs/startup/interface/include \
 ../../libs/uploadservice/impl/src \
 ../hmiservice \
 ../../libs/canservice/interface/include \
 ../../libs/canlib/impl/src \
 ../../libs/downloadservice \
 ../../libs/dbservice/interface/include

SOURCES += \
        impl/src/main.cpp

LIBS += -L../../libs/downloadservice/interface/lib -ldownloadservice
LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -L../../libs/appmanif/interface/lib -lappmaniflib
LIBS += -L../../libs/appmanifmsgmgr/interface/lib -lappmanmsglib


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    downloadservice.qrc
