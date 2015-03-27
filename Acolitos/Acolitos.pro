#-------------------------------------------------
#
# Project created by QtCreator 2012-04-06T18:58:24
#
#-------------------------------------------------

QT       += sql svg core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Acolitos
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += .. .

DESTDIR = ../lib

SOURCES += \
    myacolitoasistentes.cpp \
    myacolito.cpp \
    addpersonasdialog.cpp \
    booleanicondelegate.cpp \
    acolitotextcombobox.cpp \
    acolitoboolcheckbox.cpp \
    categoriasdelegate.cpp \
    addgrupodialog.cpp \
    modeloseleccion.cpp \
    acolitobinarytoolbutton.cpp \
    acolitovariantbutton.cpp \
    myacolitohabitaciones.cpp \
    acolitocustomlineedit.cpp \
    addhabitaciondialog.cpp \
    addlistahabitacionesdialog.cpp \
    tiposhabdelegate.cpp \
    asistentecategoriasdialog.cpp \
    textcombobox.cpp \
    habitaciontiposdialog.cpp \
    nuevacategoriadialog.cpp \
    nuevotipohabdialog.cpp \
    alojargrupodialog.cpp \
    alojarasistentedialog.cpp \
    myacolitoactividades.cpp \
    tipoactividaddialog.cpp \
    localizacionesdialog.cpp \
    addactividaddialog.cpp \
    actcompletadialog.cpp \
    imprimiractividaddialog.cpp \
    fechasdelegate.cpp \
    tipodelegate.cpp

HEADERS += \
    myacolitoasistentes.h \
    myacolito.h \
    addpersonasdialog.h \
    booleanicondelegate.h \
    acolitotextcombobox.h \
    acolitoboolcheckbox.h \
    categoriasdelegate.h \
    addgrupodialog.h \
    modeloseleccion.h \
    acolitobinarytoolbutton.h \
    acolitovariantbutton.h \
    myacolitohabitaciones.h \
    acolitocustomlineedit.h \
    addhabitaciondialog.h \
    addlistahabitacionesdialog.h \
    tiposhabdelegate.h \
    asistentecategoriasdialog.h \
    textcombobox.h \
    habitaciontiposdialog.h \
    nuevacategoriadialog.h \
    nuevotipohabdialog.h \
    alojargrupodialog.h \
    alojarasistentedialog.h \
    myacolitoactividades.h \
    tipoactividaddialog.h \
    localizacionesdialog.h \
    addactividaddialog.h \
    actcompletadialog.h \
    imprimiractividaddialog.h \
    fechasdelegate.h \
    tipodelegate.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    myacolitoasistentes.ui \
    addpersonasdialog.ui \
    addgrupodialog.ui \
    myacolitohabitaciones.ui \
    addhabitaciondialog.ui \
    addlistahabitacionesdialog.ui \
    asistentecategoriasdialog.ui \
    habitaciontiposdialog.ui \
    nuevacategoriadialog.ui \
    nuevotipohabdialog.ui \
    alojargrupodialog.ui \
    alojarasistentedialog.ui \
    myacolitoactividades.ui \
    tipoactividaddialog.ui \
    localizacionesdialog.ui \
    addactividaddialog.ui \
    actcompletadialog.ui \
    imprimiractividaddialog.ui

RESOURCES += \
    acolitoRC.qrc \
    ../MyAcolito/myResources.qrc

OTHER_FILES += \
    iconos/Orange Ball.png \
    iconos/Green Ball.png \
    iconos/edit-find.png \
    iconos/user-group-new.png \
    iconos/gtk-edit.png \
    iconos/emblem-people.png \
    iconos/config-users.png \
    iconos/document-print.png \
    iconos/_0027_Key.png \
    iconos/_0015_Padlock.png \
    iconos/gtk-yes.png \
    iconos/gtk-no.png \
    iconos/Home.png \
    iconos/adept_notifier_warning.png \
    iconos/adept_commit.png \
    iconos/addHome.png \
    iconos/addListaHab.png \
    iconos/view-media-artist.png \
    iconos/revert.png \
    iconos/restart.png \
    iconos/view-list-compact-symbolic.png \
    iconos/android_128x128-32.png \
    iconos/alarm_128x128-32.png \
    iconos/document-export.png
