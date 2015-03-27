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

#include "asistentesporcategorias.h"

AsistentesPorCategorias::AsistentesPorCategorias(QObject *parent) :
    BReport(parent,"asistentesPorCategorias")
{
    _fTexto=QFont("Verdana",12);
    _fCabecera=QFont("Verdana",14);
    _fCabecera.setBold(true);
}

QTextDocument *AsistentesPorCategorias::generateReport()
{
    initReport();

    QTextCharFormat formatoCabecera,formatoTexto;
    formatoCabecera.setFont(_fCabecera);
    formatoTexto.setFont(_fTexto);
    QTextBlockFormat formatoBloque;

    QList<QVariant> categoriasAsistentes=datos.value("listado").toList();

    cursor->insertBlock();
    cursor->insertBlock();
    foreach(QVariant aux,categoriasAsistentes) {
        QVariantMap categoria=aux.toMap();
        QList<QVariant> listadoAsistentes=categoria.value("asistentes").toList();
        QString cat=categoria.value("categoria").toString();
        cat=cat.append(": %1 asistentes");
        cursor->insertText(cat.arg(listadoAsistentes.count()),formatoCabecera);
        cursor->insertBlock();
        crearTablaCategoria(listadoAsistentes.count());
        foreach(QVariant aux2,listadoAsistentes) {
            QVariantMap asistente=aux2.toMap();
            cursor->insertText(asistente.value("nombre").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(asistente.value("DNI").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(asistente.value("email").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            formatoBloque.setAlignment(Qt::AlignCenter);
            cursor->setBlockFormat(formatoBloque);
            if(asistente.value("habitacion").isNull())
                cursor->insertText(tr("Sin asignar"),formatoTexto);
            else cursor->insertText(asistente.value("habitacion").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            if(asistente.value("pago").toBool()==true)
                cursor->insertText("Ha pagado",formatoTexto);
            else cursor->insertText("No ha pagado",formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
        }
        cursor->movePosition(QTextCursor::End);
        cursor->insertBlock();
        cursor->insertBlock();
    }

    cursor->movePosition(QTextCursor::Start);
    cursor->insertText(tr("Listado de categorias:"),formatoCabecera);
    cursor->insertBlock();

    QTextList *lista=cursor->insertList(QTextListFormat::ListDisc);
    foreach(QVariant aux,categoriasAsistentes) {
        QVariantMap categoria=aux.toMap();
        QList<QVariant> listaAsistentes=categoria.value("asistentes").toList();
        QString cat=categoria.value("categoria").toString();
        cat=cat.append(": %1 asistentes.");
        cursor->insertText(cat.arg(listaAsistentes.count()),formatoCabecera);
        cursor->insertBlock();
    }

    lista->removeItem(lista->count()-1);

    return documento;
}

QString AsistentesPorCategorias::getNombreReport() const
{
    return nombreCompleto();
}

QTextTable *AsistentesPorCategorias::crearTablaCategoria(int numAsistentes)
{
    QTextCharFormat formatoCabecera;
    formatoCabecera.setFont(_fCabecera);
    QTextBlockFormat formatoBloque;
    formatoBloque.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(formatoBloque);
    QTextTable *tabla=cursor->insertTable(numAsistentes+1,5);
    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

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
    cursor->insertText(tr("Habitacion"),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->setBlockFormat(formatoBloque);
    cursor->insertText(tr("Pago"),formatoCabecera);
    cursor->movePosition(QTextCursor::NextCell);

    return tabla;
}
