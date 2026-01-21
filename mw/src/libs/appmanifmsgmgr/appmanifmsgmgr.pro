TEMPLATE = lib
VERSION = 0.0.1
CONFIG += debug
CONFIG += warn_on
CONFIG += $$(MW_BUILD)
CONFIG += $$(ARCH)

QT -= gui
TARGET = appmanmsglib
TEMPLATE = lib
DESTDIR = ../appmanifmsgmgr/interface/lib
OBJECTS_DIR = obj
MOC_DIR = moc
CONFIG += staticlib
CONFIG += plugin

appman_test {
DEFINES +=TEST_BUILD
TEST_BUILD=""
}

INCLUDEPATH += interface/include \
       ../appmanif/interface/include
SOURCES += impl/src/msgmanager.cpp
HEADERS += interface/include/msgmanager.h \
    impl/src/msgmanagerimpl.h

sh4 {
    
}
else {
    INCLUDEPATH += ../../../vendor/nsnhal/interface/include
}
