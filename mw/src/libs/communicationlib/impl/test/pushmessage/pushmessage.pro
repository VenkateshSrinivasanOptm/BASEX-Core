QT += testlib
QT -= gui
TARGET = subscriberinfo
CONFIG += console
CONFIG -= app_bundle
CONFIG += debug
CONFIG += warn_on
CONFIG -= rtti
HEADERS += pushmessagetest.h
SOURCES += pushmessagetest.cpp
INCLUDEPATH += ../../../interface/include
LIBS += -L../../../../../../vendor/0MQ/root/lib \
    -L../../../interface/lib \
    -lzmq \
    -lcommunicationlib
