TEMPLATE = app
VERSION = 0.0.1
CONFIG += release

# CONFIG += debug
CONFIG += warn_on
CONFIG -= rtti
CONFIG += console
QT -= gui
QT += testlib
TARGET = test1
DESTDIR = .
OBJECTS_DIR = obj
INCLUDEPATH += ../../../interface/include \
               ../publisher/interface/include
HEADERS += impl/src/subscriber.h
SOURCES += impl/src/subscriber.cpp \
    impl/src/main.cpp
LIBS += -L../../../interface/lib -lcommunicationlib

CONFIG += $$(ARCH)
sh4 {

LIBS +=  -lzmq
}
else {
INCLUDEPATH += ../../../../../../vendor/0MQ/root/include
LIBS += -L../../../../../../vendor/0MQ/root/lib -lzmq
}
