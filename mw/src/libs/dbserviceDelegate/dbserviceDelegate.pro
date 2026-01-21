QT -= gui
QT += serialbus serialport

TEMPLATE = lib

CONFIG += c++17
CONFIG += plugin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    impl/src/dbserviceDelegate.cpp \
    impl/src/dbserviceclient.cpp

HEADERS += \
    interface/include/dbserviceDelegate.h  \
    interface/include/dbserviceclient.h

LIBS += -L../../libs/communicationlib/interface/lib -l communicationlib

INCLUDEPATH += interface/include \
    impl/src \
    ../communicationlib/interface/include \
    ../dbservice/interface/include \
    ../uploadservice/impl/src \
    ../canlib/impl/src \
      ../canservice/interface/include

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
