/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of MyAcolito.
**
** MyAcolito is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** MyAcolito is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with MyAcolito.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "procesadoractividades.h"
#include "ui_procesadoractividades.h"

ProcesadorActividades::ProcesadorActividades(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::ProcesadorActividades),
    database(db)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->labelMensaje->setVisible(false);
    connect(ui->dirButton,SIGNAL(clicked()),this,SLOT(seleccionarDirectorio()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(empezarProcesado()));
}

ProcesadorActividades::~ProcesadorActividades()
{
    delete ui;
}

void ProcesadorActividades::seleccionarDirectorio()
{
    QString directorio=QFileDialog::getExistingDirectory(this,tr("Seleccionar carpeta de actividades"),QString());

    ui->dirLine->setText(directorio);
}

void ProcesadorActividades::empezarProcesado()
{
    //Desactivar botones
    if(ui->dirLine->text().isEmpty() || ui->dirLine->text().isNull()) {
        QMessageBox::warning(this,tr("MyAcolito - Procesar actividades"),tr("No se puede iniciar el procesado porque el directorio no es valido"));
        return;
    }

    ui->groupBox->setEnabled(false);
    ui->startButton->setEnabled(false);

    ui->progressBar->setRange(0,0);
    ui->labelMensaje->setText(tr("Procesando carpeta..."));

    ui->progressBar->setVisible(true);
    ui->labelMensaje->setVisible(true);
    QDir directorio(ui->dirLine->text());

    QStringList filtroArchivos;
    filtroArchivos << "*.eml";
    QFileInfoList listaArchivos=directorio.entryInfoList(filtroArchivos);

    QString contenidoArchivo;
    QString mensajeA=tr("Procesando %1");
    ui->progressBar->setRange(0,listaArchivos.count());

    int i=0;
    QList<QVariantMap> listadoActividades;


    foreach(QFileInfo entrada,listaArchivos) {
        QVariantMap actividad;
        ui->labelMensaje->setText(mensajeA.arg(entrada.baseName()));
        ui->progressBar->setValue(++i);

        QFile archivo(entrada.absoluteFilePath());

        archivo.open(QFile::Text | QFile::ReadOnly);

        QTextStream stream(&archivo);

        stream.setCodec("UTF-8");

        contenidoArchivo=stream.readAll();

        //Empezamos a hacer split
        QStringList mensajeRaw=contenidoArchivo.split(tr("---"),QString::SkipEmptyParts);

        mensajeRaw.removeFirst();
        //Esto me lo divide en varios grupos
        mensajeRaw=mensajeRaw.at(0).split(tr("--"),QString::SkipEmptyParts,Qt::CaseSensitive);


        //Grupo 0: actividad,asociacion,responsable,correo,coorganizadores

        QString grupo=mensajeRaw.at(0);
        grupo=grupo.trimmed();
        QStringList aux=grupo.split("ACTIVIDAD:",QString::SkipEmptyParts);

        aux=aux.at(0).split("ASOCIACION:");
        actividad["nombre"]=aux.at(0).trimmed();

        aux=aux.at(1).split("RESPONSABLE:");
        actividad["asociacion"]=aux.at(0).trimmed();

        aux=aux.at(1).split("CORREO:");
        actividad["responsable"]=aux.at(0).trimmed();

        aux=aux.at(1).split("COORGANIZADORES:");
        actividad["correo"]=aux.at(0).trimmed();

        actividad["coorganizadores"]=aux.at(1).trimmed();

        //Siguiente bloque: material propio
        grupo=mensajeRaw.at(1);

        grupo=grupo.remove(tr("MATERIAL PROPIO:")).trimmed();
        actividad["mPropio"]=grupo;

        //Siguiente bloque, material organizacion
        grupo=mensajeRaw.at(2);
        grupo=grupo.remove(tr("MATERIAL ORGANIZACION:")).trimmed();
        actividad["mOrganizacion"]=grupo;

        //Siguiente bloque, necesidades espacio
        grupo=mensajeRaw.at(3);
        grupo=grupo.remove(tr("NECESIDADES ESPACIO:")).trimmed();
        actividad["espacio"]=grupo;

        //Siguiente bloque, horario
        grupo=mensajeRaw.at(4);
        aux=grupo.split("INICIO:",QString::SkipEmptyParts);

        QString dia=aux.first().remove("DIA:").trimmed();

        if(dia.contains(tr("Jueves"))) {
            actividad["dia"]=QVariant(QDate(2012,8,2));
        }
        else if(dia.contains("Viernes")) {
            actividad["dia"]=QVariant(QDate(2012,8,3));
        }
        else if(dia.contains("4")) {
            actividad["dia"]=QVariant(QDate(2012,8,4));
        }
        else actividad["dia"]=QVariant(QDate(2012,8,5));

        aux=aux.at(1).trimmed().split("FIN:",QString::SkipEmptyParts);
        QTime hInicio=QTime::fromString(aux.at(0).trimmed(),tr("HH:mm"));

        if(hInicio.isNull() || !hInicio.isValid()) {
            bool ok=false;

            QString nuevoInicio;
            do {
                nuevoInicio=QInputDialog::getText(this,tr("Correccion de actividad"),tr("No se reconoce la siguiente hora de inicio en la actividad '%1', por favor corrigela:\n").arg(actividad["nombre"].toString())+aux.at(0).trimmed(),QLineEdit::Normal,aux.at(0).trimmed(),&ok);
            } while(!ok);

            hInicio=QTime::fromString(nuevoInicio.trimmed(),tr("HH:mm"));
        }

        actividad["horaInicio"]=hInicio;

        QTime hFin=QTime::fromString(aux.at(1).trimmed(),tr("HH:mm"));

        if(hFin.isNull() || !hFin.isValid()) {
            bool ok=false;

            QString nuevoFin;
            do {
                nuevoFin=QInputDialog::getText(this,tr("Correccion de actividad"),tr("No se reconoce la siguiente hora de fin en la actividad '%1', por favor corrigela:\n").arg(actividad["nombre"].toString())+aux.at(1).trimmed(),QLineEdit::Normal,aux.at(1).trimmed(),&ok);
            } while(!ok);

            hFin=QTime::fromString(nuevoFin.trimmed(),tr("HH:mm"));
        }

        actividad["horaFin"]=hFin;

        //Siguiente bloque, participantes y tipo
        grupo=mensajeRaw.at(5);

        aux=grupo.split("MAXIMO PARTICIPANTES:",QString::SkipEmptyParts);
        QString minPart=aux.at(0);
        minPart=minPart.remove("MINIMO PARTICIPANTES:").trimmed();

        bool correcto;
        int mPart=minPart.toInt(&correcto);

        if(!correcto)
        {
            bool ok=false;
            QString nuevoMinimo;
            do {
                nuevoMinimo=QInputDialog::getText(this,tr("Correccion de actividad"),tr("No se reconoce el minimo en la actividad '%1', por favor corrígela:\n").arg(actividad["nombre"].toString())+minPart,QLineEdit::Normal,minPart,&ok);
            } while(!ok);

            mPart=nuevoMinimo.toInt();
        }

        actividad["minimoPart"]=mPart;

        aux=aux.at(1).split("TIPO ACTIVIDAD:",QString::SkipEmptyParts);
        QString maxPart=aux.at(0).trimmed();

        int mxPart=maxPart.toInt(&correcto);

        if(!correcto) {
            bool ok=false;
            QString nuevoMaximo;
            do {
                nuevoMaximo=QInputDialog::getText(this,tr("Correccion de actividad"),tr("No se reconoce el maximo en la actividad '%1', por favor corrígela:\n").arg(actividad["nombre"].toString())+maxPart,QLineEdit::Normal,maxPart,&ok);
            } while(!ok);

            mxPart=nuevoMaximo.toInt();
        }

        actividad["maximoPart"]=mxPart;

        actividad["tipo"]=aux.at(1).trimmed();

        //Siguiente grupo: desc
        grupo=mensajeRaw.at(6);

        grupo=grupo.remove("DESCRIPCION:").trimmed();

        actividad["descripcion"]=grupo;

        //siguiente grupo: requisitos
        grupo=mensajeRaw.at(7);
        grupo=grupo.remove("REQUISITOS:").trimmed();
        actividad["requisitos"]=grupo;

        //Siguiente grupo:
        grupo=mensajeRaw.at(8);
        grupo=grupo.remove("OBSERVACIONES:").trimmed();
        actividad["observaciones"]=grupo;

        actividad["todaJornada"]=QVariant(false);
        actividad["lugar"]=tr("Sin asignar");

        listadoActividades << actividad;
    }

    i=0;
    ui->progressBar->setRange(0,listadoActividades.count());

    foreach(QVariantMap nuevaActividad,listadoActividades) {
        ui->labelMensaje->setText(tr("Guardando %1 en base dedatos...").arg(nuevaActividad["nombre"].toString()));
        ui->progressBar->setValue(++i);

        database->addActividad(nuevaActividad);
    }

    ui->progressBar->setValue(listadoActividades.count());
    ui->groupBox->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->labelMensaje->setVisible(false);
    ui->progressBar->setVisible(false);
}
