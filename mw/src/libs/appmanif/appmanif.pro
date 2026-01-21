TEMPLATE = lib
VERSION = 0.0.1
CONFIG += debug
CONFIG += warn_on
QT -= gui
TARGET = appmaniflib
TEMPLATE = lib
DESTDIR = ../appmanif/interface/lib
OBJECTS_DIR = obj
MOC_DIR = moc
CONFIG += staticlib
CONFIG += $$(MW_BUILD)
CONFIG += $$(ARCH)
CONFIG += plugin

appman_test {
DEFINES +=TEST_BUILD
TEST_BUILD=""
INCLUDEPATH += ../../modules/appman/impl/test/AppManTest/
}

sh4 {
    
}
else {
    INCLUDEPATH += ../../../vendor/nsnhal/interface/include
}
INCLUDEPATH += interface/include \
    impl/src \
    ../../libs/logging/interface/include \
    ../appmanifmsgmgr/interface/include
SOURCES += impl/src/appmanif.cpp \
    impl/src/eventwatcher.cpp
HEADERS += interface/include/appmanif.h \
    impl/src/eventwatcher.h \
    interface/include/appmanmsgif.h
#LIBS += -Linterface/lib/     -lappmanmsglib
