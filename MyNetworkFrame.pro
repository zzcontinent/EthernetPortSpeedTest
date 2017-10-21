TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cgetpacket.cpp \
    MBuffer.cpp \
    ProtoType.cpp \
    Sampling.cpp \
    TCP_UDP.cpp

HEADERS += \
    cgetpacket.h \
    MBuffer.h \
    MyHeader.h \
    Sampling.h \
    ProtoType.h \
    TCP_UDP.h

LIBS +=-lpthread

