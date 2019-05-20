QT += core testlib
QT -= gui

CONFIG += c++11

TARGET = IniReader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


include($$PWD/readini/readini.pri)

INCLUDEPATH += $$PWD/readini

SOURCES += main.cpp

HEADERS += \
    readini.h
