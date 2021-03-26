QT += core
QT -= gui

CONFIG += c++11

TARGET = DllLoadBase
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include($$PWD/../common/common.pri)

SOURCES += main.cpp \
    dllmanager.cpp

HEADERS += \
    dllmanager.h
