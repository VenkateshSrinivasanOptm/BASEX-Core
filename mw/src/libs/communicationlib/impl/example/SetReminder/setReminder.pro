TEMPLATE = app

VERSION = 0.0.1

CONFIG += release
#CONFIG += debug
CONFIG += warn_on
CONFIG -= rtti
CONFIG += console

QT -= gui

TARGET = test1

DESTDIR = .

OBJECTS_DIR = obj

INCLUDEPATH += ../../../interface/include \
               ../ReminderResponse/interface/include \
               interface/include

HEADERS += \ 
     ../ReminderResponse/interface/include/remindermessage.h \
     impl/src/dtv.h

SOURCES += \ 
    impl/src/dtv.cpp \
    impl/src/main.cpp

LIBS += -L../../../interface/lib -lcommunicationlib

CONFIG += $$(ARCH)
sh4 {

LIBS +=  -lzmq
}
else {
INCLUDEPATH += ../../../vendor/0MQ/root/include
LIBS += -L../../../vendor/0MQ/root/lib -lzmq
}
