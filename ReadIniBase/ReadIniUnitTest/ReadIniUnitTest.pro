#-------------------------------------------------
#
# Project created by QtCreator 2019-05-20T23:11:40
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_readiniunittesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


include($$PWD/../readini/readini.pri)

INCLUDEPATH += $$PWD/../readini

SOURCES += tst_readiniunittesttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
