TEMPLATE = lib
CONFIG += debug
CONFIG += warn_on
CONFIG += $$(MW_BUILD)
CONFIG += $$(ARCH)
QT -= gui
OBJECTS_DIR = obj
MOC_DIR = moc
TARGET = appmanlib
TEMPLATE = lib
DESTDIR = ../appman/interface/lib
CONFIG += staticlib
CONFIG += plugin

appman_test {
DEFINES +=TEST_BUILD
TEST_BUILD=""
INCLUDEPATH += impl/test/ \
     ../../modules/appman/impl/test/AppManTest/
SOURCES += ../../../vendor/nsnhal/impl/src/powersaving.cpp
}

sh4 {
    
}
else {
    INCLUDEPATH += ../../../vendor/nsnhal/interface/include
}
INCLUDEPATH += ../../libs/startup/interface/include \
    ../../libs/communicationlib/interface/include

INCLUDEPATH += interface/include \
    impl/src \
    ../appmanif/interface/include \
    ../appmanifmsgmgr/interface/include \
    ../../modules/appman/impl/test

# ../startup/interface/include
SOURCES += impl/src/main.cpp \
    impl/src/package.cpp \
    impl/src/mwconfig.cpp \
    impl/src/commandmonitor.cpp \
    impl/src/mwstartup.cpp
#   impl/src/semwatcher.cpp

HEADERS += impl/src/package.h \
    impl/src/mwconfig.h \
    interface/include/mwserviceinfo.h \
    impl/src/commandmonitor.h \
    interface/include/appmanmessages.h \
    interface/include/mwstartup.h
#   impl/src/semwatcher.h \

RESOURCES += \
    appmanlib.qrc
