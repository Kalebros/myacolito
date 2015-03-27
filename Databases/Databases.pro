#-------------------------------------------------
#
# Project created by QtCreator 2012-03-20T13:53:45
#
#-------------------------------------------------

QT       += core sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = Databases
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../lib

SOURCES += \
    acolitodb.cpp \
    datosconexion.cpp \
    myeventodb.cpp \
    sqlscript.cpp \
    myacolitodb.cpp \
    mymodeloeventos.cpp \
    mydatabase.cpp \
    myacolitomodels.cpp \
    myasistentemodels.cpp \
    acolitodatabase.cpp \
    mymodeloasistente.cpp \
    mymodelocategorias.cpp \
    mycategoriamodels.cpp \
    mymodelohabitaciones.cpp \
    myhabitacionmodels.cpp \
    mymodelotiposhab.cpp \
    mymodeloactividades.cpp \
    myactividadmodels.cpp \
    mymodelotipoactividad.cpp \
    mymodelolocalizaciones.cpp \
    mymensajesmodels.cpp

HEADERS += \
    acolitodb.h \
    datosconexion.h \
    myeventodb.h \
    sqlscript.h \
    myacolitodb.h \
    mymodeloeventos.h \
    mydatabase.h \
    myacolitomodels.h \
    myasistentemodels.h \
    acolitodatabase.h \
    mymodeloasistente.h \
    mymodelocategorias.h \
    mycategoriamodels.h \
    mymodelohabitaciones.h \
    myhabitacionmodels.h \
    mymodelotiposhab.h \
    mymodeloactividades.h \
    myactividadmodels.h \
    mymodelotipoactividad.h \
    mymodelolocalizaciones.h \
    mymensajesmodels.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    eventodb_script.sql \
    iconos/Green Ball.png \
    myacolito_script.sql \
    myacolito_procedures.sql \
    myacolito_triggers.sql \
    iconos/Red Ball.png

RESOURCES += \
    dbScripts.qrc

FORMS +=
