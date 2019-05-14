#-------------------------------------------------
#
# Project created by QtCreator 2019-05-12T09:16:07
#
#-------------------------------------------------

QT       += network serialport

QT       -= gui

TARGET = DllBase
TEMPLATE = lib

DEFINES += DLLBASE_LIBRARY

CONFIG += skip_target_version_ext
VERSION = 1.0.0.0

windows{
CONFIG(debug, debug|release){
    DESTDIR = ./output_Debug
}
CONFIG(release, debug|release){
    DESTDIR = ./output
}
}
unix{
    DESTDIR = ./output
}


SOURCES += dllbase.cpp \
    main.cpp

HEADERS +=\
        dllbase_global.h \
    qteventandthread.h \
    dllbase.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
