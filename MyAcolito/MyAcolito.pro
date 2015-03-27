#-------------------------------------------------
#
# Project created by QtCreator 2012-03-20T13:47:36
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = MyAcolito
TEMPLATE = app

INCLUDEPATH += ..

CONFIG(debug,debug|release) {
    LIBS += -L../lib -lBReport -lReportesAcolito -lComponentes -lDatabases -lWizardEvento -lAcolitos -lSManager -lAcolitoJSON
}
else {
    LIBS += -L../lib -lBReport -lReportesAcolito -lComponentes -lDatabases -lWizardEvento -lAcolitos -lSManager -lAcolitoJSON
}

SOURCES += main.cpp\
        mainwindow.cpp \
    logdialog.cpp \
    preferenciasdialog.cpp \
    conexionessql.cpp \
    conexiondialog.cpp \
    delegadoevento.cpp \
    eventodialog.cpp \
    gestorpaginas.cpp \
    procesadoractividades.cpp \
    gestorinformes.cpp \
    mensajesdialog.cpp

HEADERS  += mainwindow.h \
    logdialog.h \
    preferenciasdialog.h \
    conexionessql.h \
    conexiondialog.h \
    delegadoevento.h \
    eventodialog.h \
    gestorpaginas.h \
    procesadoractividades.h \
    gestorinformes.h \
    mensajesdialog.h

FORMS    += mainwindow.ui \
    logdialog.ui \
    preferenciasdialog.ui \
    conexiondialog.ui \
    eventodialog.ui \
    grupoasistentesdialog.ui \
    procesadoractividades.ui \
    mensajesdialog.ui

OTHER_FILES += \
    iconos/view-media-artist.png \
    iconos/server-database.png \
    iconos/network-disconnect.png \
    iconos/network-connect.png \
    iconos/add.png \
    iconos/dadosERA.png \
    iconos/application-exit.png \
    iconos/window-close.png \
    iconos/userLog.png \
    iconos/remove.png \
    iconos/template_source.png \
    iconos/stock_script.png

RESOURCES += \
    myResources.qrc
