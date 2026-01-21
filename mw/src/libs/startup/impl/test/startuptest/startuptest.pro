QT += testlib
QT -= gui
TARGET = startup
CONFIG += console
CONFIG -= app_bundle
CONFIG += debug
CONFIG += warn_on
CONFIG -= rtti
QT += sql
TEMPLATE = app
HEADERS += request.h \
    programinfoquerier.h \
    startuptest.h \
    request2.h \
    settingsquerier.h
SOURCES += request.cpp \
    programinfoquerier.cpp \
    startuptest.cpp \
    request2.cpp \
    settingsquerier.cpp
SOURCES += ../../src/startupserviceprivate.cpp
HEADERS += ../../src/startupserviceprivate.h
INCLUDEPATH += ../../src \
    ../../../../communicationlib/interface/include \
    ../../../interface/include \
    /home/project/danos/p4/nmc/veyron/dev/mw/src/libs/ociservice/interface/include \
    ../../../../mmddf_writer/interface/include \
    ../../../../oci_writer/interface/include \
    ../../../../pacman/interface/include \
    ../../../../dbcommon/interface/include
CONFIG += no_lflags_merge
LIBS += -L../../../../communicationlib/interface/lib \
    -lcommunicationlib
LIBS += -L../../../../../../vendor/0MQ/root/lib \
    -lzmq
