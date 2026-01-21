TEMPLATE = app
VERSION = 0.0.1
#CONFIG += release

CONFIG += debug
CONFIG += warn_on
CONFIG -= rtti
QT -= gui
QT += testlib
TARGET = client
DESTDIR = .
OBJECTS_DIR = obj
INCLUDEPATH += ../../../interface/include \
    ../server
HEADERS += client.h
SOURCES += client.cpp \
    main.cpp
LIBS += -L../../../interface/lib \
    -lcommunicationlib
CONFIG += $$(ARCH)
sh4 { 
    
    LIBS +=  \
        -lzmq
}
else { 
    INCLUDEPATH += ../../../../../../vendor/0MQ/root/include
    LIBS += -L../../../../../../vendor/0MQ/root/lib \
        -lzmq
}
