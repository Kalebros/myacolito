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
#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include "BReport_global.h"

#include <QDialog>
#include <QTextDocument>


namespace Ui {
    class ReportDialog;
}

/*!
  Dialogo de presentacion de reportes
  @brief
  Esta clase es empleada por parte del BReportManager para mostrar un reporte al usuario
  y preguntarle por la acción que quiere realizar: guardarlo como PDF, guardarlo como ODT,
  o enviarlo a la impresora.
  */
class BREPORTSHARED_EXPORT ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(QWidget *parent = 0);
    ~ReportDialog();

    /*!
      Devuelve el reporte actual del dialogo
      @return QTextDocument*, puntero al reporte
      @note No modifica el objeto
      */
    QTextDocument *getReporteActual();

signals:

    /*!
      Guardar como ODT.
      @param report Documento del reporte
      */
    void saveAsODT(QTextDocument *report);

    /*!
      Guardar como PDF
      @param report Documento del reporte
      */
    void saveAsPDF(QTextDocument *report);

    /*!
      Imprimir
      @param report Documento del reporte
      */
    void print(QTextDocument *report);

public slots:

    /*!
      Establece el reporte actual del dialogo
      y lo muestra en pantalla
      @param reporte    Reporte actual
      @note Modifica el objeto
      */
    void setReporte(QTextDocument *reporte);

private:
    Ui::ReportDialog *ui;
    QTextDocument *report;  ///< Reporte del dialogo a mostrar

    /*!
      Prepara el dialogo
      */
    void setupDialog();

private slots:

    /*!
      Envia el documento para guardarlo como PDF
      */
    void sendToPDF();

    /*!
      Envia el documento para guardarlo como ODT
      */
    void sendToODT();

    /*!
      Envia el documento para imprimirlo
      */
    void sendToPrint();
};

#endif // REPORTDIALOG_H
