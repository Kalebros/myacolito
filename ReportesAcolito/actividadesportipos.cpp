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

#include "actividadesportipos.h"

ActividadesPorTipos::ActividadesPorTipos(QObject *parent) :
    BReport(parent,"reporteTipos")
{
    _fTexto=QFont("Verdana",12);
    _fCabecera=QFont("Verdana",14);
}

QString ActividadesPorTipos::getNombreReport() const
{
    return this->nombreCompleto();
}


QTextDocument *ActividadesPorTipos::generateReport()
{
    initReport();

    QTextCharFormat formatoTexto;
    formatoTexto.setFont(_fTexto);
    cursor->setCharFormat(formatoTexto);

    QList<QVariant> listaTipos=datos.value("listado").toList();
    foreach(QVariant aux,listaTipos) {
        QVariantMap datosTipos=aux.toMap();

        formatoTexto.setFont(_fTexto);
        cursor->setCharFormat(formatoTexto);
        BReport::addTitulo(documento,cursor,datosTipos.value("categoria").toString(),14);

        BReport::addLineaResaltada(documento,cursor,tr("Numero de actividades"),Negrita | Subrayado,
                                   datosTipos.value("numActividades").toString(),Italica);

        cursor->insertBlock();

        QTextTable *tablaActividad=crearTablaActividades(datosTipos.value("numActividades").toInt());

        QList<QVariant> listaActividades=datosTipos.value("actividades").toList();

        formatoTexto.setFont(_fTexto);
        foreach(QVariant aux,listaActividades) {
            QVariantMap actividad=aux.toMap();
            QString dia=actividad.value("fecha").toDate().toString("ddd dd");
            dia.replace(0,1,dia.at(0).toUpper());
            cursor->insertText(dia,formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(actividad.value("inicio").toTime().toString("hh:mm"),
                               formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(BReport::elideText(actividad.value("nombre").toString(),
                                         100),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(actividad.value("lugar").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
        }

        cursor->movePosition(QTextCursor::End);
        cursor->insertBlock();
        cursor->insertBlock();
        cursor->insertBlock();
    }

    return documento;
}

QTextTable *ActividadesPorTipos::crearTablaActividades(int numActividades)
{
    QTextTable *tabla=cursor->insertTable(numActividades+1,4);

    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

    //Para cada reporte de ActividadesPor... cambiar las cabeceras
    QTextCharFormat formatoTexto;
    formatoTexto.setFont(_fCabecera);
    cursor->insertText(tr("Dia"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->insertText(tr("Inicio"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->insertText(tr("Actividad"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->insertText(tr("Lugar"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    return tabla;
}
