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

#include "breportmanager.h"

BReportManager::BReportManager(QObject *parent) :
    QObject(parent)
{
    dialogo=new ReportDialog(qobject_cast<QWidget*>(parent));
    connect(dialogo,SIGNAL(saveAsODT(QTextDocument*)),this,SLOT(toODT(QTextDocument*)));
    connect(dialogo,SIGNAL(saveAsPDF(QTextDocument*)),this,SLOT(toPDF(QTextDocument*)));
    connect(dialogo,SIGNAL(print(QTextDocument*)),this,SLOT(toPrint(QTextDocument*)));

    impresora=0;
}

void BReportManager::addFactory(BFactoryReport *factoria)
{
    QString nombre=factoria->metaObject()->className();

    listaFactorias.insert(nombre,factoria);
}

void BReportManager::removeFactory(BFactoryReport *factoria)
{
    QString nombre=factoria->metaObject()->className();

    listaFactorias.remove(nombre);
}

void BReportManager::removeFactory(QString factoria)
{
    listaFactorias.remove(factoria);
}


void BReportManager::basicReport(QString mensaje, QVariantMap datos, ReportOptions opcion)
{
    //qDebug() << "Dentro del manager...";
    //Primer paso, procesar el mensaje

    QStringList preProcesado=mensaje.split("::");

    //Buscar factoria

    //qDebug() << "Mensaje recibido: " << mensaje << " ...";

    BFactoryReport *factoria=listaFactorias.value(preProcesado.first(),0);

    if(!factoria) {
        emit error(tr("No se pudo recuperar la factoria de reportes especificada."));
        return;
    }

    //Segunda parte del mensaje

    BReport *reporte=factoria->getReport(preProcesado.at(1));

    if(!reporte) {
        emit error(tr("No existe el reporte indicado dentro de la factoria especificada."));
        return;
    }

    //qDebug() <<"REPORTE: "<< reporte->metaObject()->className();

    reporte->setData(datos);

    QTextDocument *docReport=reporte->generateReport();

    //Comprobar opciones

    switch(opcion) {
    case ShowOnDialog:
        dialogo->setReporte(docReport);
        break;
    case SaveAsPDF:
        toPDF(docReport);
        break;
    case SaveAsODT:
        toODT(docReport);
        break;
    case Print:
        toPrint(docReport);
        break;
    default:
        emit error(tr("Opcion de BReport no reconocida."));
        break;
    }
}

//TODO: Actualizar para usar correctamente QVariantMap en vez de QList<QVariantList>
void BReportManager::batchReport(QString mensaje, QList<QVariantMap> datos, ReportOptions opcion)
{
    //Obtener impresora
    if(opcion!=SaveAsODT) {
        if(impresora) {
            delete impresora;
            impresora=0;
        }
        impresora=new QPrinter();

        QPrintDialog *confDialog=new QPrintDialog(impresora,qobject_cast<QWidget*>(parent()));
        if(confDialog->exec()!=QDialog::Accepted) {
            delete impresora;
            impresora=0;
            emit error(tr("No se ha podido configurar la impresora para imprimir los reportes.\nPor favor, vuelva a intentarlo."));
            return;
        }
    }

    //Procesar mensaje
    QStringList preProcesado=mensaje.split("::");

    //Buscar factoria
    BFactoryReport *factoria=listaFactorias.value(preProcesado.first(),0);

    if(!factoria) {
        emit error(tr("No se pudo recuperar la factoria de reportes especificada."));
        return;
    }

    //Buscar reporte
    BReport *reporte=factoria->getReport(preProcesado.at(1));

    if(!reporte) {
        emit error(tr("No se encontro el reporte solicitado en la factoria especificada."));
        return;
    }

    QTextDocument *resultado;
    QProgressDialog pDialogo(tr("Preparando..."),tr("Cancelar salida"),0,datos.count(),qobject_cast<QWidget*>(this->parent()));
    pDialogo.setCancelButton(0);
    pDialogo.setMinimumDuration(0);
    pDialogo.setWindowModality(Qt::WindowModal);

    QList<QPair<QString,QTextDocument *> > reportesGenerados;

    pDialogo.setLabelText(tr("Generando reportes..."));
    int contador=0;
    foreach(QVariantMap lista,datos) {
        pDialogo.setValue(contador++);
        reporte->setData(lista);
        //Incluir el path de salida entre los datos del reporte es buena idea
        QString salida=reporte->getNombreReport();
        resultado=reporte->generateReport();
        reportesGenerados.append(QPair<QString,QTextDocument*>(salida,resultado));
    }

    for(int i=0;i<reportesGenerados.count();i++) {
        QPair<QString,QTextDocument*> pareja=reportesGenerados.value(i);
        QString texto=tr("Guardando reporte %1...");
        QString base=QFileInfo(pareja.first).baseName();
        QString textoLabel=texto.arg(base);
        pDialogo.setLabelText(textoLabel);
        pDialogo.setValue(i);
        QString salida=pareja.first;
        switch(opcion) {
        case SaveAsPDF: //Enviar a PDF
            salida.append(".pdf");
            saveToPDF(pareja.second,salida,impresora);
            break;
        case SaveAsODT:
            salida.append(".odt");
            saveToODT(pareja.second,salida);
            break;
        case Print:
            toPrint(pareja.second,impresora);
            break;
        default:
            break;
        }
    }

    pDialogo.setValue(datos.count());
}

void BReportManager::setImpresora(QPrinter *printer)
{
    if(impresora) {
        delete impresora;
        impresora=0;
    }

    impresora=printer;
}

void BReportManager::toODT(QTextDocument *doc)
{
    //Obtener nombre de salida del documento
    QString dirSalida=qApp->applicationDirPath();

    QString fileOutput=QFileDialog::getSaveFileName(qobject_cast<QWidget*>(parent()),
                                                    tr("Guardar reporte como ODT"),
                                                    dirSalida,
                                                    tr("Archivos ODT (*.odt)"));

    //Guardar archivo ODT

    QTextDocumentWriter *writer=new QTextDocumentWriter(fileOutput);
    writer->setFormat("ODF");
    writer->write(doc);

    QMessageBox::information(qobject_cast<QWidget*>(this->parent()),tr("BReport - Guardando archivo"),tr("Reporte guardado como ODT."));
}

void BReportManager::toPDF(QTextDocument *doc)
{
    //Obtener nombre de salida del documento
    QString dirSalida=qApp->applicationDirPath();

    QString fileOutput=QFileDialog::getSaveFileName(qobject_cast<QWidget*>(parent()),
                                                    tr("Guardar reporte como PDF"),
                                                    dirSalida,
                                                    tr("Archivos PDF (*.pdf)"));

    if(impresora) {
        delete impresora;
        impresora=0;
    }
    impresora=new QPrinter();

    QPrintDialog *confDialog=new QPrintDialog(impresora,qobject_cast<QWidget*>(parent()));
    if(confDialog->exec()!=QDialog::Accepted) {
        delete impresora;
        impresora=0;
        emit error(tr("No se ha podido configurar la impresora para imprimir los reportes.\nPor favor, vuelva a intentarlo."));
        return;
    }

    impresora->setOutputFormat(QPrinter::PdfFormat);
    impresora->setOutputFileName(fileOutput);
    doc->print(impresora);

    QMessageBox::information(qobject_cast<QWidget*>(this->parent()),tr("BReport - Guardando archivo"),tr("Reporte guardado como PDF."));
}

void BReportManager::toPrint(QTextDocument *doc)
{
    if(impresora) {
        delete impresora;
        impresora=0;
    }
        impresora=new QPrinter();

    QPrintDialog *confDialog=new QPrintDialog(impresora,qobject_cast<QWidget*>(parent()));

    if(confDialog->exec()!=QDialog::Accepted) {
        delete impresora;
        impresora=0;
        emit error(tr("No se ha podido configurar la impresora para imprimir los reportes.\nPor favor, vuelva a intentarlo."));
        return;
    }

    doc->print(impresora);
}

void BReportManager::toPrint(QTextDocument *doc, QPrinter *printer)
{
    doc->print(printer);
}

void BReportManager::saveToODT(QTextDocument *doc, QString path)
{
    //Guardar archivo ODT
    QTextDocumentWriter *writer=new QTextDocumentWriter(path);
    writer->setFormat("ODF");
    writer->write(doc);
}

void BReportManager::saveToPDF(QTextDocument *doc, QString path, QPrinter *printer)
{
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOutputFileName(path);
    doc->print(printer);
}

QString BReportManager::versionBReport()
{
    return QString("0.1.0");
}
