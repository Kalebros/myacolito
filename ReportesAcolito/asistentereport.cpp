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

#include "asistentereport.h"

AsistenteReport::AsistenteReport(QObject *parent) :
    BReport(parent,"reporteAsistente")
{
    _fTexto=QFont("Verdana",12);
    _fCabecera=QFont("Verdana",13);
    _fCabecera.setBold(true);
}

QString AsistenteReport::getNombreReport() const
{
    return this->nombreCompleto();
}

QTextDocument *AsistenteReport::generateReport()
{
    initReport();

    QTextCharFormat formatoTexto,formatoCabecera;
    QTextBlockFormat formatoBloque;
    formatoBloque.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    formatoTexto.setFont(_fTexto);
    formatoCabecera.setFont(_fCabecera);

    crearTablaAsistente();

    //ID del asistente
    cursor->setBlockFormat(formatoBloque);
    QFont negrita=formatoCabecera.font();
    negrita.setPointSize(14);
    formatoCabecera.setFont(negrita);
    cursor->insertText(datos.value("id").toString(),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);

    //Nombre
    formatoBloque.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    formatoCabecera.setFont(_fCabecera);
    QFont fuenteNombre=formatoCabecera.font();
    fuenteNombre.setPointSize(14);
    formatoCabecera.setFont(fuenteNombre);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("nombre").toString(),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);

    //DNI
    formatoCabecera.setFont(_fCabecera);
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(tr("DNI"),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);
    formatoBloque.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("dni").toString(),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Categoria
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(tr("Categoria"),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);
    formatoBloque.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("categoria").toString(),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Pago
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText("Ha pagado:",formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);
    formatoBloque.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);
    if(datos.value("pago").toBool())
        cursor->insertText(tr("Si"),formatoTexto);
    else cursor->insertText(tr("No"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Correo y telefono
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("email").toString(),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("telefono").toString(),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    //Comentarios
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(tr("Comentarios"),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);
    formatoBloque.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(datos.value("comentarios").toString(),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);


    cursor->movePosition(QTextCursor::End);

    QList<QVariant> listaCompis=datos.value("grupo").toList();

    crearTablaHabitacion(listaCompis.count());

    //Codigo de habitacion
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(tr("Habitacion: %1").arg(datos.value("habitacion").toString()),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);

    //Compis de habitacion
    foreach(QVariant asis,listaCompis) {
        QVariantMap asistente=asis.toMap();
        cursor->insertText(asistente.value("nombre").toString(),formatoTexto);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->insertText(asistente.value("dni").toString(),formatoTexto);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->insertText(asistente.value("correo").toString(),formatoTexto);
        cursor->movePosition(QTextCursor::NextCell);
        cursor->insertText(asistente.value("categoria").toString(),formatoTexto);
        cursor->movePosition(QTextCursor::NextCell);
        if(asistente.value("pago").toBool())
            cursor->insertText(tr("Ha pagado"),formatoTexto);
        else cursor->insertText(tr("No ha pagado"),formatoTexto);
        cursor->movePosition(QTextCursor::NextCell);
    }

    cursor->movePosition(QTextCursor::End);

    return documento;
}

QTextTable *AsistenteReport::crearTablaAsistente()
{
    QTextTable *tabla=cursor->insertTable(6,7);
    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

    tabla->mergeCells(0,1,1,6);   //Nombre
    tabla->mergeCells(1,1,1,6);   //DNI
    tabla->mergeCells(2,1,1,4);   //Categoria
    tabla->mergeCells(3,4,1,3);   //Telefono
    tabla->mergeCells(3,0,1,4);   //Correo
    tabla->mergeCells(4,0,1,7);   //Cabecera de Comentarios
    tabla->mergeCells(5,0,1,7);   //Comentarios

    return tabla;
}

QTextTable *AsistenteReport::crearTablaHabitacion(int numCompis)
{
    QTextTable *tabla=cursor->insertTable(numCompis+1,5);
    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

    tabla->mergeCells(0,0,1,5); //Cabecera habitacion

    return tabla;
}
