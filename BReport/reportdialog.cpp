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
#include "reportdialog.h"
#include "ui_reportdialog.h"

ReportDialog::ReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
    setupDialog();
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::setupDialog()
{
    connect(ui->odtButton,SIGNAL(clicked()),this,SLOT(sendToODT()));
    connect(ui->pdfButton,SIGNAL(clicked()),this,SLOT(sendToPDF()));
    connect(ui->printButton,SIGNAL(clicked()),this,SLOT(sendToPrint()));
}

void ReportDialog::setReporte(QTextDocument *reporte)
{
    report=reporte;
    ui->visualizador->setDocument(report);
    show();
}

void ReportDialog::sendToODT()
{
    emit saveAsODT(report);
}

void ReportDialog::sendToPDF()
{
    emit saveAsPDF(report);
}

void ReportDialog::sendToPrint()
{
    emit print(report);
}
