#-------------------------------------------------
#
# Project created by QtCreator 2012-04-13T19:12:13
#
#-------------------------------------------------

QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Componentes
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../lib

SOURCES += \
    textcombobox.cpp \
    intcheckbox.cpp \
    customlineedit.cpp \
    boolcheckbox.cpp \
    varianttoolbutton.cpp \
    variantpushbutton.cpp \
    binarytoolbutton.cpp \
    slidingstackedwidget.cpp

HEADERS += \
    textcombobox.h \
    intcheckbox.h \
    customlineedit.h \
    boolcheckbox.h \
    varianttoolbutton.h \
    variantpushbutton.h \
    binarytoolbutton.h \
    slidingstackedwidget.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
