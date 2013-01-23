#-------------------------------------------------
#
# Project created by QtCreator 2013-01-19T14:13:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientTCP
TEMPLATE = app


SOURCES += main.cpp\
        clienttcpwindow.cpp \
    gui/rcvprogressview.cpp \
    gui/rcvprogressscene.cpp \
    datahandler.cpp \
    connection.cpp \
    protocol/packet.cpp \
    dhtcp/dhtcp.cpp \
    connectionthread.cpp \
    dhtcp/dhtcpdecoder.cpp \
    protocol/rawblock.cpp

HEADERS  += clienttcpwindow.h \
    gui/rcvprogressview.h \
    gui/rcvprogressscene.h \
    datahandler.h \
    connection.h \
    protocol/protocoltypes.h \
    protocol/ports_define.h \
    protocol/packet.h \
    protocol/cmd_define.h \
    dhtcp/dhtcp.h \
    dhtcp/dhtcpprotocol.h \
    connectionthread.h \
    dhtcp/dhtcpdecoder.h \
    protocol/rawblock.h

FORMS    += clienttcpwindow.ui
