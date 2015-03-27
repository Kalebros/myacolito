/***********************************************************************
** SManager, copyright @2013,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of SManager.
**
** SManager is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SManager is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with SManager.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "smscriptlite.h"

#include "smstolite.h"

SMScriptLite::SMScriptLite(QObject *parent, int idInterno, SMStoLite *db, QSqlTableModel *modelo) :
    SMScript(parent),
    _identificador(idInterno),
    _almacen(db),
    _modeloDatos(modelo),
    pNombre(-1),
    pPath(-1),
    pArgumentos(-1)
{
    getFieldsIndex();
}

void SMScriptLite::getFieldsIndex()
{
    if(_modeloDatos && _almacen->isOpen()) {
        QSqlRecord record=_modeloDatos->record();

        pNombre=record.indexOf("nombre");
        pPath=record.indexOf("path");
        pArgumentos=record.indexOf("argumentos");
    }
}

int SMScriptLite::getID() const
{
    return _identificador;
}

int SMScriptLite::searchPosition() const
{
    int res=-1;
    if(_almacen->isOpen() && _modeloDatos) {
        int nScripts=_modeloDatos->rowCount();
        bool found=false;
        int value=-1;
        for(int i=0;i<nScripts && !found;i++) {
            value=_modeloDatos->record(i).field("idScript").value().toInt();
            if(value==_identificador) {
                found=true;
                res=i;
            }
        }

        if(!found) res=-1;
    }
    return res;
}

QString SMScriptLite::getNombre() const
{
    int pos=searchPosition();

    QString res;
    if(pos!=-1)
        res=_modeloDatos->record(pos).field("nombre").value().toString();

    return res;
}

QString SMScriptLite::getPath() const
{
    int pos=searchPosition();

    QString res;
    if(pos!=-1)
        res=_modeloDatos->record(pos).field("path").value().toString();

    return res;
}
QString SMScriptLite::getArgumentos() const
{
    int pos=searchPosition();

    QString res;
    if(pos!=-1)
        res=_modeloDatos->record(pos).field("argumentos").value().toString();

    return res;
}

void SMScriptLite::setNombre(QString nombre)
{
    int pos=searchPosition();

    if(pNombre==-1)
        getFieldsIndex();

    if(pos!=-1) {
        _modeloDatos->setData(_modeloDatos->index(pos,pNombre),nombre,Qt::EditRole);
        _modeloDatos->submit();
    }
}

void SMScriptLite::setPath(QString path)
{
    int pos=searchPosition();

    if(pPath==-1)
        getFieldsIndex();

    if(pos!=-1) {
        _modeloDatos->setData(_modeloDatos->index(pos,pPath),path,Qt::EditRole);
        _modeloDatos->submit();
    }

}

void SMScriptLite::setArgumentos(QString argumentos)
{
    int pos=searchPosition();

    if(pArgumentos==-1)
        getFieldsIndex();

    if(pos!=-1) {
        _modeloDatos->setData(_modeloDatos->index(pos,pArgumentos),argumentos,Qt::EditRole);
        _modeloDatos->submit();
    }
}
