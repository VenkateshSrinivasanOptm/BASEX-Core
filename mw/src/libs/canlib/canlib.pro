QT -= gui serialbus serialport
QT += serialbus widgets
CONFIG += c++17
TEMPLATE = lib
VERSION = 0.0.1
CONFIG += warn_on
CONFIG -= rtti
QT -= gui
TARGET = canlib
DESTDIR = ../canlib/interface/lib
OBJECTS_DIR = obj
CONFIG += plugin



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    impl/src/canlib.cpp \
    impl/src/canparser.cpp

HEADERS += \
    impl/src/canparser.h \
    interface/include/canlib.h

INCLUDEPATH += interface/include \
INCLUDEPATH += impl/src \

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
