TEMPLATE = app
VERSION = 0.0.1
TARGET = appman
CONFIG += console
CONFIG -= app_bundle
CONFIG += $$(MW_BUILD)
CONFIG += no_lflags_merge
CONFIG += plugin

QT -= gui
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = ../interface/bin
INCLUDEPATH += impl/src \
    ../../libs/appmanif/interface/include \
    ../../libs/appman/interface/include \
    ../../libs/appmanifmsgmgr/interface/include \
    ../../libs/communicationlib/interface/include \
    ../../libs/startup/interface/include \
    ../../libs/mmddfservice/interface/include \
    ../hmiservice

appman_test {
TEST_BUILD=""
SOURCES += impl/test/AppManTest/stubs.cpp
HEADERS += impl/test/AppManTest/stubs.h
}
else {
LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
}

SOURCES += impl/src/main.cpp

LIBS += -L../../libs/appman/interface/lib -lappmanlib

CONFIG += $$(ARCH)
sh4 {
##QMAKE_LIBS  = -lgcc
##INCLUDEPATH += ../../libs/logging/interface/include
LIBS += -L../../libs/startup/interface/lib -lstartup

LIBS +=  -lconfig++
LIBS +=  -lconfig

LIBS +=  -lpcrecpp

CONFIG += link_pkgconfig
PKGCONFIG += libhal
LIBS +=  -lzmq
}
else {
INCLUDEPATH += ../../../vendor/nsnhal/interface/include
SOURCES +=
LIBS += -L../../libs/appman/interface/lib -lappmanlib
LIBS += -L../../libs/startup/interface/lib -lstartup

!appman_test {
LIBS += -L../../../vendor/0MQ/root/lib -lzmq
}
}
LIBS += -L../../libs/appmanif/interface/lib -lappmaniflib
LIBS += -L../../libs/appmanifmsgmgr/interface/lib -lappmanmsglib

RESOURCES += \
    appman.qrc
