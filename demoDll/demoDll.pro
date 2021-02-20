QT -= gui
QT += network serialport sql

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = demoDll
TEMPLATE = lib

DEFINES += DLLBASE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

include($$PWD/common/common.pri)
include($$PWD/readini/readini.pri)
include($$PWD/databaseManager/databaseManager.pri)
include($$PWD/networkManager/networkManager.pri)

SOURCES += \
        SerailManager.cpp \
        dllExport.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    SerailManager.h \
    dllExport.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

