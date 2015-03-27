#-------------------------------------------------
#
# Project created by QtCreator 2012-12-29T15:22:51
#
#-------------------------------------------------

QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = BReport
TEMPLATE = lib

DEFINES += BREPORT_LIBRARY

DESTDIR = ../lib

SOURCES += \
    reportdialog.cpp \
    breportmanager.cpp \
    breport.cpp \
    bfactoryreport.cpp

HEADERS += \
    reportdialog.h \
    libBReport.h \
    breportmanager.h \
    BReport_global.h \
    breport.h \
    bfactoryreport.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    reportdialog.ui

RESOURCES += \
    BReportResources.qrc \
    BReportResources.qrc

OTHER_FILES += \
    iconos/printer.png \
    iconos/breportIcon.png \
    iconos/application-vnd.oasis.opendocument.text.png \
    iconos/application-pdf.png
