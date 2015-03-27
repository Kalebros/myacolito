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

#include "breport.h"

BReport::BReport(QObject *parent, QString nombreBase) :
    QObject(parent),
    nombreInforme(nombreBase)
{
    documento=0;
    cursor=0;
}

QVariantMap BReport::getData() const
{
    return datos;
}

void BReport::setData(QVariantMap datosVariant)
{
    datos.clear();
    datos=datosVariant;
}

void BReport::initReport()
{
    documento=new QTextDocument(0); ///Si eliminamos el reporte, el documento permanece
    if(cursor)
        delete cursor;

    cursor=new QTextCursor(documento);
}

QTextDocument *BReport::lastReport()
{
    return documento;
}

QString BReport::nombreCompleto() const
{
    if(!datos.contains("nombreInforme"))
        return nombreInforme;

    QString path=datos.value("nombreInforme").toString();

    //Comprobar el contenido del path
    if(!QFile::exists(path))
        QDir().mkpath(path);

    QString res=path.append("/").append(nombreInforme);
    return res;
}

QTextTable *BReport::addTabla(QAbstractTableModel *dataModel, QTextDocument *doc, QTextCursor *cursor,QTextCharFormat formatoCab)
{
    QTextTable *tabla=cursor->insertTable(dataModel->rowCount()+1,dataModel->columnCount());

    //Escribir cabecera de informacion
    for(int i=0;i<dataModel->columnCount();i++) {
        QTextTableCell celda=tabla->cellAt(0,i);
        QTextCursor cursorCelda=celda.firstCursorPosition();

        QString nombreColumna=dataModel->headerData(i,Qt::Horizontal).toString();
        cursorCelda.insertText(nombreColumna,formatoCab);
    }

    //Escribir informacion de la tabla en cada celda correspondiente

    for(int i=0;i<dataModel->rowCount();i++) {
        for(int j=0;j<dataModel->columnCount();j++) {
            QTextTableCell celda=tabla->cellAt(i+1,j);
            QTextCursor cursorCelda=celda.firstCursorPosition();

            QString data=dataModel->data(dataModel->index(i,j)).toString();
            cursorCelda.insertText(data);
        }
    }

    cursor->movePosition(QTextCursor::End);

    return tabla;
}

QTextTable *BReport::addTabla(QStandardItemModel *modelo, QTextDocument *doc, QTextCursor *cursor,QTextCharFormat formatoCab)
{
    QTextTable *tabla=cursor->insertTable(modelo->rowCount()+1,modelo->columnCount());

    //Escribir cabecera de informacion
    for(int i=0;i<modelo->columnCount();i++) {
        QTextTableCell celda=tabla->cellAt(0,i);
        QTextCursor cursorCelda=celda.firstCursorPosition();

        QString nombreColumna=modelo->headerData(i,Qt::Horizontal).toString();
        cursorCelda.insertText(nombreColumna,formatoCab);
    }

    //Escribir informacion de la tabla en cada celda correspondiente

    for(int i=0;i<modelo->rowCount();i++) {
        for(int j=0;j<modelo->columnCount();j++) {
            QTextTableCell celda=tabla->cellAt(i+1,j);
            QTextCursor cursorCelda=celda.firstCursorPosition();

            QString data=modelo->data(modelo->index(i,j)).toString();
            cursorCelda.insertText(data);
        }
    }

    cursor->movePosition(QTextCursor::End);

    return tabla;
}

void BReport::addLineaResaltada(QTextDocument *doc,QTextCursor *cursor,
                                QString titulo, OpcionesTexto opTitulo,
                                QString texto, OpcionesTexto opTexto)
{
    QTextCharFormat formatoBase=BReport::cambiarTipoChar(cursor, opTitulo);
    cursor->insertText(titulo);
    cursor->setCharFormat(formatoBase);
    cursor->insertText(tr(":\t"));
    formatoBase=BReport::cambiarTipoChar(cursor,opTexto);
    cursor->insertText(texto);
    cursor->setCharFormat(formatoBase);
    cursor->insertBlock();
}

void BReport::addTitulo(QTextDocument *doc, QTextCursor *cursor, QString titulo, int fontSize)
{
    QTextCharFormat formatoBase=cursor->charFormat();
    QTextCharFormat formato=formatoBase;
    QFont fuente=formatoBase.font();
    fuente.setBold(true);
    fuente.setPointSize(fontSize);
    formato.setFont(fuente);

    cursor->setCharFormat(formato);
    cursor->insertText(titulo);
    cursor->setCharFormat(formatoBase);
    cursor->insertBlock();
}

void BReport::addLinea(QTextDocument *doc, QTextCursor *cursor, QString linea, OpcionesTexto opciones)
{
    cursor->insertBlock();
    QTextCharFormat formatoBase=BReport::cambiarTipoBloque(cursor,opciones);
    cursor->insertText(linea);
    cursor->setCharFormat(formatoBase);
}

void BReport::addLinea(QTextDocument *doc, QTextCursor *cursor, QString linea)
{
    cursor->insertBlock();
    cursor->insertText(linea);
}

QTextCharFormat BReport::cambiarTipoChar(QTextCursor *cursor, OpcionesTexto op)
{
    QTextCharFormat base=cursor->charFormat();
    QTextCharFormat nFormato=base;
    QFont fuente=nFormato.font();
    if((op & Negrita)==Negrita) {
        fuente.setBold(true);
    }
    if((op & Italica)==Italica) {
        fuente.setItalic(true);
    }
    if((op & Subrayado)==Subrayado) {
        fuente.setUnderline(true);
    }
    nFormato.setFont(fuente);
    cursor->setCharFormat(nFormato);
    return base;
}

QTextCharFormat BReport::cambiarTipoBloque(QTextCursor *cursor, OpcionesTexto op)
{
    QTextCharFormat base=cursor->blockCharFormat();
    QTextCharFormat nFormato=base;
    QFont fuente=nFormato.font();
    if((op & Negrita)==Negrita) {
        fuente.setBold(true);
    }
    if((op & Italica)==Italica) {
        fuente.setItalic(true);
    }
    if((op & Subrayado)==Subrayado) {
        fuente.setUnderline(true);
    }
    nFormato.setFont(fuente);
    cursor->setBlockCharFormat(nFormato);
    return base;
}

QString BReport::elideText(QString texto, int numCaracteres)
{
    QString res=texto;
    if(res.length()>numCaracteres) {
        res.resize(numCaracteres);
        res.append("...");
    }

    return res;
}
