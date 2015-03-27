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

#include "habitacionreport.h"

HabitacionReport::HabitacionReport(QObject *parent) :
    BReport(parent, tr("reporteHabitacion"))
{
    _fCabecera=QFont("Verdana",14);
    _fTexto=QFont("Verdana",12);
}

QString HabitacionReport::getNombreReport() const
{
    return this->nombreCompleto();
}

QTextDocument *HabitacionReport::generateReport()
{
    initReport();

    QTextCharFormat formatoCabecera,formatoTexto;
    formatoCabecera.setFont(_fCabecera);
    formatoTexto.setFont(_fTexto);
    QTextBlockFormat formatoBloque;

    crearTablaHabitacion();

    formatoCabecera.setForeground(Qt::white);
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("codigo").toString(),formatoCabecera);
    formatoCabecera.setForeground(Qt::black);
    cursor->movePosition(QTextCursor::NextCell);

    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("tipo").toString(),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);

    cursor->setBlockFormat(formatoBloque);
    if(datos.value("libre").toBool())
        cursor->insertText("Habitacion Libre",formatoCabecera);
    else cursor->insertText("Habitacion Ocupada",formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);


    if(datos.value("abierta").toBool())
        cursor->insertText("Abierta",formatoCabecera);
    else cursor->insertText("Cerrada",formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);

    cursor->insertText(datos.value("comentarios").toString(),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    if(datos.value("reservada").toBool()) {
        cursor->setBlockFormat(formatoBloque);
        formatoCabecera.setForeground(Qt::white);
        cursor->insertText(tr("Habitacion RESERVADA"),formatoCabecera);
        formatoCabecera.setForeground(Qt::black);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->insertText(datos.value("cReserva").toString(),formatoTexto);
        cursor->movePosition(QTextCursor::NextCell);
    }
    cursor->movePosition(QTextCursor::End);
    cursor->insertBlock();

    //Tabla de asistentes
    if(!datos.contains("alojados")) {
        crearTablaAlojados(0);
    }
    else {
        QList<QVariant> alojados=datos.value("alojados").toList();
        crearTablaAlojados(alojados.count()+1);

        foreach(QVariant aux,alojados) {
            QVariantMap alojado=aux.toMap();
            cursor->insertText(alojado.value("nombre").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(alojado.value("DNI").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(alojado.value("correo").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(alojado.value("tipo").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            if(alojado.value("pago").toBool())
                cursor->insertText(tr("Ha pagado"),formatoTexto);
            else cursor->insertText(tr("No ha pagado"),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
        }
    }

    cursor->movePosition(QTextCursor::End);

    return documento;

}

QTextTable *HabitacionReport::crearTablaAlojados(int numAlojados)
{
    QTextTable *tabla;
    QTextCharFormat formatoTexto,formatoCabecera;
    QTextBlockFormat formatoBloque;
    formatoTexto.setFont(_fTexto);
    formatoCabecera.setFont(_fCabecera);

    if(numAlojados==0) {
        tabla=cursor->insertTable(2,1);

        QTextCharFormat formatoID;
        formatoID=tabla->cellAt(1,0).format();
        formatoID.setBackground(Qt::darkGray);
        formatoID.setForeground(Qt::white);
        tabla->cellAt(0,0).setFormat(formatoID);

        formatoCabecera.setForeground(Qt::white);
        formatoBloque.setAlignment(Qt::AlignCenter);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("Asistentes alojados"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);
        formatoCabecera.setForeground(Qt::black);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("Habitacion VACIA"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);

    }
    else {
        tabla=cursor->insertTable(numAlojados+1,5);
        tabla->mergeCells(0,0,1,5);

        QTextCharFormat formatoID;
        formatoID=tabla->cellAt(0,0).format();
        formatoID.setBackground(Qt::darkGray);
        formatoID.setForeground(Qt::white);
        tabla->cellAt(0,0).setFormat(formatoID);

        formatoCabecera.setForeground(Qt::white);
        formatoBloque.setAlignment(Qt::AlignCenter);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("Asistentes alojados"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);
        formatoCabecera.setForeground(Qt::black);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("Nombre"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("DNI"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("Correo"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("Categoria"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->setBlockFormat(formatoBloque);
        cursor->insertText(tr("Pago"),formatoCabecera);
        cursor->movePosition(QTextCursor::NextCell);
    }

    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

    return tabla;
}

QTextTable *HabitacionReport::crearTablaHabitacion()
{

    QTextTable *tabla;

    if(!datos.value("reservada").toBool()) {
        tabla=cursor->insertTable(3,3);
        tabla->mergeCells(0,1,1,2);
        tabla->mergeCells(1,0,1,2);
        tabla->mergeCells(2,0,1,3);
    }
    else {
        tabla=cursor->insertTable(5,3);
        tabla->mergeCells(0,1,1,2);
        tabla->mergeCells(1,0,1,2);
        tabla->mergeCells(2,0,1,3);
        tabla->mergeCells(3,0,1,3);
        tabla->mergeCells(4,0,1,3);
    }


    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);


    QTextCharFormat formatoID;
    formatoID=tabla->cellAt(1,0).format();
    formatoID.setBackground(Qt::darkGray);
    formatoID.setForeground(Qt::white);
    tabla->cellAt(0,0).setFormat(formatoID);
    if(datos.value("reservada").toBool())
        tabla->cellAt(3,0).setFormat(formatoID);

    return tabla;
}
