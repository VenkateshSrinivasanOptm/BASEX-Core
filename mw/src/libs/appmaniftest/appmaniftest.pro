TEMPLATE = lib
VERSION = 0.0.1
CONFIG += debug
CONFIG += warn_on
CONFIG += plugin

QT -= gui
TARGET = appmaniflib
TEMPLATE = lib
DESTDIR = interface/lib
OBJECTS_DIR = obj
CONFIG += staticlib
INCLUDEPATH += interface/include \
        ../appmanifmsgmgr/interface/include
SOURCES += impl/src/appmanif.cpp
HEADERS += interface/include/appmanif.h \
          interface/include/appmanmsgif.h
