QT -= gui

QT += serialbus widgets

TEMPLATE = lib

CONFIG += c++17
CONFIG += plugin

DESTDIR = ../canserviceDelegate/interface/lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    impl/src/CanServiceClient.cpp \
    impl/src/CanserviceDelegate.cpp

HEADERS += \
    interface/include/CanServiceClient.h \
    interface/include/CanserviceDelegate.h

LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -L../../libs/canlib/interface/lib -lcanlib
LIBS += -L../../libs/canservice/interface/lib -l canservice

INCLUDEPATH += ../../libs/canservice/interface/include \
../../libs/communicationlib/interface/include \
../../libs/canserviceDelegate/interface/include \
../../libs/canserviceDelegate/impl/src \
../../libs/canlib/interface/include \
../../libs/canlib/impl/src \
../../libs/canservice/interface/include \
../../libs/wifiservice/interface/include \
../../libs/gsmservice/interface/include \
../../libs/connectionmanagerservice/interface/include \
../../libs/btservice/interface/include


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
