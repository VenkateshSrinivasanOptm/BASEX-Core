TEMPLATE = lib
VERSION = 0.0.1
#CONFIG += release

#DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_DEBUG

# uncomment this for performance test
#DEFINE  += PERFORMANCE

CONFIG += warn_on
CONFIG -= rtti
QT -= gui
TARGET = communicationlib
DESTDIR = ../communicationlib/interface/lib
OBJECTS_DIR = obj
INCLUDEPATH += interface/include
INCLUDEPATH += ../performance/interface/include
CONFIG += plugin

HEADERS += interface/include/message.h \
    interface/include/messagehandler.h \
    interface/include/deserializer.h \
    interface/include/cbusinterface.h \
    impl/src/callee.h \
    impl/src/cbus.h \
    interface/include/callback.h \
    interface/include/callbackgen.h \
    interface/include/stringMessage.h \
    interface/include/listMessage.h \
    interface/include/mapMessage.h \
    interface/include/shortMessage.h \
    interface/include/stringListMessage.h \
    impl/src/zeromqmultipleserver.h \
    impl/src/zeromqworker.h \
    impl/src/zeromqsingleserver.h \
    impl/src/zeromqcommunication.h \
    impl/src/zeromqclient.h \
    impl/src/zeromqqbytearray.h \
    impl/src/zeromqincommingmessage.h \
    interface/include/cbusconfig.h \
    impl/src/cbusconfigprivate.h \
    impl/src/triple.h \
    interface/include/comlibservices.h \
    impl/src/cbuscallbackintf.h \
    interface/include/subscriberIntf.h \
    impl/src/zeromqpublishserver.h \
    impl/src/definitions.h \
    impl/src/sendobject.h \
    impl/src/zeromqsenderthread.h \
    impl/src/zeromqbasicclient.h \
    impl/src/zeromqsubscriberserver.h \
    impl/src/queue.h \
    impl/src/outgoingworker.h \
    impl/src/cbusforwardintf.h

SOURCES += impl/src/cbusinterface.cpp \
    impl/src/cbus.cpp \
    impl/src/zeromqmultipleserver.cpp \
    impl/src/zeromqworker.cpp \
    impl/src/zeromqsingleserver.cpp \
    impl/src/zeromqcommunication.cpp \
    impl/src/zeromqclient.cpp \
    impl/src/cbusconfig.cpp \
    impl/src/cbusconfigprivate.cpp \
    impl/src/zeromqincommingmessage.cpp \
    impl/src/zeromqpublishserver.cpp \
    impl/src/zeromqsenderthread.cpp \
    impl/src/zeromqbasicclient.cpp \
    impl/src/zeromqsubscriberserver.cpp \
    impl/src/outgoingworker.cpp

CONFIG += $$(ARCH)
sh4 {

LIBS +=  -lzmq
}
else {
INCLUDEPATH += ../../../vendor/0MQ/root/include
LIBS += -L../../../vendor/0MQ/root/lib -lzmq
}
