/***********************************************************************
** BReport, copyright @2012,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of BReport.
**
** BReport is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** BReport is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with BReport.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#ifndef BREPORTMANAGER_H
#define BREPORTMANAGER_H


#include "bfactoryreport.h"
#include "reportdialog.h"

#include <QObject>
#include <QWidget>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QVariantList>
#include <QVariantMap>
#include <QPair>
#include <QFileInfo>
#include <QString>
#include <QMap>
#include <QMessageBox>
#include <QApplication>

#include <QPrintDialog>
#include <QPrinter>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QProgressDialog>

#include <QDebug>

/*!
  Opciones del manager
  - ShowOnDialog: Muestra el reporte en un dialogo
  con diferentes opciones. No se emplea a la hora
  de imprimir un batch de reportes
  - SaveAsPDF: Guarda el reporte como un PDF. Lanza un
  dialogo para preguntar el directorio donde guardarlo.
  Se puede emplear en un batch de reportes.
  - SaveAsODT: Guarda el reporte como un ODT. Lanza un
  dialogo para preguntar el directorio donde guardarlo.
  Se puede emplear en un batch de reportes.
  - Print: Imprime directamente el informe. Lanza un dialogo
  para seleccionar impresora. Se puede emplear en un batch
  de reportes.

  @note Es imprescindible, antes de poder emplear las
  opciones del enum en el sistema de signals y slots,
  registrar dicho tipo mediante la plantilla @i qRegisterMetaType,
  de la siguiente forma:
  @i qRegisterMetaType<ReportOptions>("ReportOptions");
  Registrarlo dentro del constructor de la clase donde se vaya a emplear
  (p.e, en el constructor del MainWindow)
  */

enum ReportOptions
{ ShowOnDialog,SaveAsPDF,SaveAsODT,Print};

Q_DECLARE_METATYPE(ReportOptions)

/*!
  Gestor de reportes

  @brief
  El gestor de reportes es el encargado de recibir y pre-procesar
  los mensajes para seleccionar la factoria correcta. Tambien es el
  encargado de generar los dialogos de visualizacion, imprimir
  los reportes en tandas o de uno en uno, guardarlos como archivos pdf
  o bien en formato ODT.

  @note Es necesario inicializar el archivo de recursos de BReport
  incluido dentro de la libreria estatica para todo aquel programa
  que haga uso de la libreria. Esto se puede lograr invocando a la macro
  Q_INIT_RESOURCE para el archivo BReportResources, como se puede ver en el ejemplo
  de Qt del sistema de recursos.
  */

class BREPORTSHARED_EXPORT BReportManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(ReportOptions)
public:
    explicit BReportManager(QObject *parent = 0);

    /*!
      Elimina una factoria del reportManager
      @param factoria   Puntero a la factoria que queremos eliminar
      @note Modifica el objeto
      */

    void removeFactory(BFactoryReport *factoria);

    /*!
      @overload
      */
    void removeFactory(QString factoria);

    /*!
      Devuelve la version actual de la libreria
      @note MODIFICAR CADA INCREMENTO DE VERSION
      */

    static QString versionBReport();

signals:

    /*!
      Envia un mensaje indicando que se ha producido un error de algun tipo.
      @param mensaje    Mensaje de error
      */

    void error(QString mensaje);

public slots:

    /*!
      Añade una nueva factoria
      @param factoria   Puntero a la factoria a añadir
      @note Modifica el objeto
      */

    void addFactory(BFactoryReport *factoria);

    /*!
      Genera un reporte basico
      @param mensaje    Codigo de la factoria y del reporte
      @param datos  Datos necesarios para generar el reporte
      @param opcion Opcion del reporte (consultar @ref ReportOptions )
      @note No modifica el objeto
      */
    void basicReport(QString mensaje,QVariantMap datos,ReportOptions opcion);

    /*!
      Genera un reporte de tipo batch
      @param mensaje    Codigo de la factoria y del reporte que usaremos como plantilla para el batch
      @param datos  Datos necesarios para generar el batch de reportes
      @param opcion Opcion del reporte (consultar @ref ReportOptions )
      @note No modifica el objeto
      */
    void batchReport(QString mensaje,QList<QVariantMap> datos,ReportOptions opcion);

    /*!
      Establece una impresora como predeterminada para el gestor
      @param printer    Impresora a utilizar
      @note Modifica el objeto
      @note Al imprimir documentos en modo batch, se vuelve a solicitar la impresora
      @pre La impresora debe de haber sido configurada con antelacion
      */

    void setImpresora(QPrinter *printer);

private:

    QMap<QString,BFactoryReport *> listaFactorias;  ///< Factorias conocidas por el reportManager
    ReportDialog *dialogo;      ///< Dialogo de presentacion de reportes, para el basicReport
    QPrinter *impresora;        ///< Impresora a utilizar para generar los reportes

private slots:

    /*!
      Guarda un reporte como archivo PDF
      @param doc Documento del reporte
      @note No modifica el objeto
      */
    void toPDF(QTextDocument *doc);

    /*!
      Guarda un reporte como archivo ODT
      @param doc    Nombre del reporte
      @note No modifica el objeto
      */
    void toODT(QTextDocument *doc);

    /*!
      Imprime un reporte
      @param doc    Documento del reporte
      @note No modifica el objeto
      */
    void toPrint(QTextDocument *doc);

    /*!
      @overload
      */

    void toPrint(QTextDocument *doc,QPrinter *printer);

    /*!
      Guarda un reporte como archivo PDF, con un nombre concreto
      @param doc    Documento del reporte
      @param path   Nombre completo del archivo
      @param printer  Impresora a utilizar
      @note No modifica el objeto
      */

    void saveToPDF(QTextDocument *doc,QString path,QPrinter *printer);

    /*!
      Guarda un reporte como archivo ODT, con un nombre concreto
      @param doc    Documento del reporte
      @param path   Nombre completo del archivo
      @note No modifica el objeto
      */

    void saveToODT(QTextDocument *doc,QString path);

};

#endif // BREPORTMANAGER_H
