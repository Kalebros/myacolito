/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ram�rez Marti
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

#include "actividadreport.h"

ActividadReport::ActividadReport(QObject *parent) :
    BReport(parent,"actividadReport")
{
    _limiteCarDescripcion=900;
    _limiteCarRequisitos=800;
    _limiteCarTitulo=100;
    _limiteCarCategoria=13;

    _fTexto=QFont("Verdana",10);
    _fCabecera=QFont("Verdana",14);
    _fId=QFont("Verdana",20);

}

QString ActividadReport::getNombreReport() const
{
    return datos.value("nombre").toString();
}

QTextDocument *ActividadReport::generateReport()
{
    this->initReport();

    QTextTable *tablaActividad=crearTablaBase();


    QTextCharFormat formatoTexto,formatoID;
    QTextBlockFormat formatoBloque;

    //Formato para las cabeceras
    formatoTexto.setFont(_fCabecera);

    //Formato para el ID de la actividad
    formatoID.setFont(_fId);
    formatoID.setBackground(QBrush(Qt::darkGray));
    formatoID.setForeground(QBrush(Qt::white));

    //Insertamos el nombre de la categoria
    formatoBloque.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(BReport::elideText(datos.value("categoria","NOT FOUND").toString(),_limiteCarCategoria)
                       ,formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Nombre de la actividad
    formatoBloque.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    cursor->setBlockFormat(formatoBloque);
    QString nActividad=BReport::elideText(datos.value("nombre",tr("NOT FOUND")).toString(),_limiteCarTitulo);
    cursor->insertText(nActividad,formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //ID de la actividad
    formatoBloque.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("id",0).toString(),formatoID);
    cursor->movePosition(QTextCursor::NextCell);

    //Responsable de la actividad
    formatoBloque.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QFont fuente(_fTexto);
    fuente.setItalic(true);
    formatoTexto.setFont(fuente);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("responsable",tr("NOT FOUND")).toString(),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Dia de la actividad
    formatoBloque.setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    QDate fecha=datos.value("fecha").toDate();
    QString dia=fecha.toString("ddd dd");
    dia.replace(0,1,dia.at(0).toUpper());
    cursor->setBlockFormat(formatoBloque);
    formatoTexto.setFont(_fCabecera);
    cursor->insertText(dia,formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Descripcion de la actividad
    formatoBloque.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QString descripcion=BReport::elideText(datos.value("descripcion",tr("Sin descripcion")).toString(),_limiteCarDescripcion);
    formatoTexto.setFont(_fTexto);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(descripcion,formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Horario de la actividad
    formatoBloque.setAlignment(Qt::AlignCenter);
    QTime hInicio,hFin;
    hInicio=datos.value("inicio").toTime();
    hFin=datos.value("fin").toTime();
    formatoTexto.setFont(_fCabecera);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(hInicio.toString("HH:mm"),formatoTexto);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(QString("\n"),formatoTexto);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(hFin.toString("HH:mm"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Numero de participantes
    formatoTexto.setFont(_fCabecera);
    formatoBloque.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);

    int maximo=datos.value("maxParticipantes",10).toInt();
    int minimo=datos.value("minParticipantes",1).toInt();

    QString numParticipantes("De %1 a %2\nparticipantes");

    cursor->insertText(numParticipantes.arg(minimo).arg(maximo),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Cabecera de requisitos
    formatoBloque.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(tr("Requisitos:"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Requisitos
    formatoTexto.setFont(_fTexto);
    formatoBloque.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    cursor->setBlockFormat(formatoBloque);
    QString requisitos=datos.value("requisitos",tr("Sin requisitos")).toString();
    cursor->insertText(BReport::elideText(requisitos,_limiteCarRequisitos),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Lugar
    formatoBloque.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    cursor->setBlockFormat(formatoBloque);
    formatoTexto.setFont(_fCabecera);
    cursor->insertText(tr("Donde:"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->insertText(datos.value("lugar",tr(" ")).toString(),formatoTexto);

    cursor->movePosition(QTextCursor::End);

    cursor->insertBlock();
    cursor->insertBlock();

    crearTablaParticipantes();

    return documento;
}

QTextTable *ActividadReport::crearTablaBase()
{
    QTextTable *tabla=cursor->insertTable(8,7);

    tabla->mergeCells(0,1,1,6); //Nombre de actividad
    tabla->mergeCells(1,1,1,6); //Responsable de la actividad
    tabla->mergeCells(2,1,2,6); //Descripcion de actividad
    tabla->mergeCells(4,1,1,6);
    tabla->mergeCells(5,1,2,6);
    tabla->mergeCells(4,0,3,1);
    tabla->mergeCells(7,1,1,6);
    //tabla->mergeCells(5,1,2,6);

    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

    QTextCharFormat formatoID;
    formatoID=tabla->cellAt(1,0).format();
    formatoID.setBackground(Qt::darkGray);
    formatoID.setForeground(Qt::white);
    tabla->cellAt(1,0).setFormat(formatoID);

    return tabla;
}

QTextTable *ActividadReport::crearTablaParticipantes()
{
    QTextTable *tabla=cursor->insertTable(11,4);
    tabla->mergeCells(0,0,1,4);

    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

    QTextBlockFormat formatoBloque;
    formatoBloque.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QTextCharFormat formatoTexto;
    formatoTexto.setFont(QFont("Verdana",18));
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(tr("Participantes"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    formatoTexto.setFont(QFont("Verdana",14));
    formatoBloque.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);

    QTextCursor cursorAux;
    int contador=1;
    for(int j=0;j<4;j++) {
        for(int i=1;i<11;i++) {
            cursorAux=tabla->cellAt(i,j).firstCursorPosition();
            cursorAux.insertText(QString::number(contador++),formatoTexto);
        }
    }

    return tabla;
}
