#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T19:48:13
#
#-------------------------------------------------

QT += sql

TARGET = AcolitoJSON
TEMPLATE = lib

DEFINES += ACOLITOJSON_LIBRARY

DESTDIR = ../lib

INCLUDEPATH += ..

SOURCES += \
    acolitojsonparser.cpp

LIBS += -L../lib -lDatabases

HEADERS +=\
        AcolitoJSON_global.h \
    acolitojsonparser.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
