#-------------------------------------------------
#
# Project created by QtCreator 2012-03-29T13:24:20
#
#-------------------------------------------------

TARGET = WizardEvento
TEMPLATE = lib
CONFIG += staticlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


QT += sql

INCLUDEPATH += ..

DESTDIR = ../lib

SOURCES += \
    wizardevento.cpp \
    wevplazacombobox.cpp \
    weventopaginaplazas.cpp \
    weventopaginainicial.cpp \
    weventopaginafinal.cpp \
    calendarioseleccion.cpp

HEADERS += \
    wizardevento.h \
    wevplazacombobox.h \
    weventopaginaplazas.h \
    weventopaginainicial.h \
    weventopaginafinal.h \
    calendarioseleccion.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    weventopaginaplazas.ui \
    weventopaginainicial.ui \
    weventopaginafinal.ui
