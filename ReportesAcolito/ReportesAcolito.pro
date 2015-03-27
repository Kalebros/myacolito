#-------------------------------------------------
#
# Project created by QtCreator 2012-12-29T15:59:13
#
#-------------------------------------------------

QT       += core sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReportesAcolito
TEMPLATE = lib

DEFINES += REPORTESACOLITO_LIBRARY

INCLUDEPATH +=..

DESTDIR =../lib

LIBS += -L../lib -lBReport

SOURCES += \
    factoriaactividades.cpp \
    actividadreport.cpp \
    actividadespordias.cpp \
    actividadesportipos.cpp \
    actividadesporlocalizacion.cpp \
    factoriaasistentes.cpp \
    asistentereport.cpp \
    asistentesporcategorias.cpp \
    factoriahabitaciones.cpp \
    habitacionreport.cpp

HEADERS += \
    ReportesAcolito_global.h \
    factoriaactividades.h \
    actividadreport.h \
    actividadespordias.h \
    actividadesportipos.h \
    actividadesporlocalizacion.h \
    factoriaasistentes.h \
    asistentereport.h \
    asistentesporcategorias.h \
    factoriahabitaciones.h \
    habitacionreport.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS +=
