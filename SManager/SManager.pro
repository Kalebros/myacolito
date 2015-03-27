#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T17:25:30
#
#-------------------------------------------------

QT += sql gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SManager
TEMPLATE = lib

DEFINES += SMANAGER_LIBRARY

DESTDIR =../lib

SOURCES += \
    smstorage.cpp \
    smstolite.cpp \
    smscriptlite.cpp \
    smscriptform.cpp \
    smscript.cpp \
    smcontainerwidget.cpp \
    smanager.cpp

HEADERS +=\
        SManager_global.h \
    smstorage.h \
    smstolite.h \
    smscriptlite.h \
    smscriptform.h \
    smscript.h \
    smcontainerwidget.h \
    smanager.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    smscriptform.ui \
    smcontainerwidget.ui

RESOURCES += \
    recursos.qrc

OTHER_FILES += \
    iconos/system-run.png \
    iconos/list-remove.png \
    iconos/add.png
