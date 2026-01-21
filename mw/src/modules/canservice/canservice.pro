QT = core

CONFIG += c++17 cmdline
CONFIG += no_lflags_merge
CONFIG += plugin

TEMPLATE = app
TARGET = canservice

QT -= gui
QT += sql serialbus


MOC_DIR = moc
DESTDIR = ../interface/bin
OBJECTS_DIR = obj

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../libs/canservice/interface/include \
../../libs/communicationlib/interface/include \
../../libs/canlib/interface/include \
../../libs/canlib/impl/src \
../../libs/uploadservice/impl/src \
../../libs/appmanif/interface/include \
../../libs/startup/interface/include \
../../libs/dbservice/interface/include \
../../libs/dbservice/impl/src \
../../libs/canservice/interface/include \
../hmiservice

INCLUDEPATH += ../../libs/btservice/interface/include
INCLUDEPATH += ../../libs/fotaservice/interface/include

SOURCES += \
        main.cpp

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -L../../libs/canlib/interface/lib -lcanlib
LIBS += -L../../libs/canservice/interface/lib -lcanservice
LIBS += -L../../libs/appmanif/interface/lib -lappmaniflib
LIBS += -L../../libs/appmanifmsgmgr/interface/lib -lappmanmsglib
LIBS += -L../../libs/dbservice/interface/lib -ldbservice
LIBS += -L../../libs/btservice/interface/lib -l btservice
LIBS += -L../../libs/fotaservice/interface/lib -lfotaservice

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    canservice.qrc
