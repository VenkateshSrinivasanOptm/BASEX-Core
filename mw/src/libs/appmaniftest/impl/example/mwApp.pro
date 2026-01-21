TEMPLATE = app
VERSION = 0.0.1
CONFIG += debug
CONFIG += warn_on
CONFIG += console
CONFIG -= app_bundle
CONFIG += $$(MW_BUILD)

QT -= gui
TARGET = mwapp
DESTDIR = .
OBJECTS_DIR = obj
MOC_DIR = moc
CONFIG += staticlib


#Option to include the test if lib or the real if lib. Later only required when testing the AM
appman_test {
    INCLUDEPATH +=  ../../../appmanifmsgmgr/interface/include
} else {
    INCLUDEPATH += ../../../appmaniftest/interface/include
}
INCLUDEPATH += ../../../appmanif/interface/include

SOURCES += impl/src/main.cpp \
    impl/src/mwapp.cpp
HEADERS += impl/src/mwapp.h

#Option to include the test if lib or the real if lib. Later only required when testing the AM
appman_test {
    LIBS += -L../../../appmanif/interface/lib/ -lappmaniflib \
          -L../../../appmanifmsgmgr/interface/lib/ -lappmanmsglib
} else {
    LIBS += -L../../../appmaniftest/interface/lib/ -lappmaniflib
}

sh4 {
    
}
else {
    INCLUDEPATH += ../../../../../vendor/nsnhal/interface/include
    LIBS += -L../../../../../vendor/nsnhal/interface/lib -lnsnhal
}
