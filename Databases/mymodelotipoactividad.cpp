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

#include "mymodelotipoactividad.h"

MyModeloTipoActividad::MyModeloTipoActividad(QObject *parent, const QSqlDatabase db) :
    QAbstractTableModel(parent)
{
    if(!db.isOpen() || !db.isValid()) {}
    else {
        _tablaTipos=new QSqlTableModel(this,db);

        _tablaTipos->setTable(tr("tiposActividad"));
        _tablaTipos->select();
    }
}

int MyModeloTipoActividad::rowCount(const QModelIndex &parent) const
{
    if(_tablaTipos)
        return _tablaTipos->rowCount();
    else return 0;
}

int MyModeloTipoActividad::columnCount(const QModelIndex &parent) const
{
    if(_tablaTipos)
        return _tablaTipos->columnCount();
    else return 0;
}

QVariant MyModeloTipoActividad::data(const QModelIndex &index, int role) const
{
    if(_tablaTipos)
        return _tablaTipos->data(index,role);
    else return QVariant();
}

bool MyModeloTipoActividad::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(_tablaTipos) {
        _tablaTipos->setData(index,value,role);
        _tablaTipos->submitAll();
        emit dataChanged(index,index);
        emit editadoTipoActividad();
        return true;
    }
    else return false;
}

Qt::ItemFlags MyModeloTipoActividad::flags(const QModelIndex &index) const
{
    //Ajuste de version
#if QT_VERSION >=0x050000
    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
#else
    if(_tablaTipos)
        return _tablaTipos->flags(index);
    else
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
#endif

}

QVariant MyModeloTipoActividad::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(_tablaTipos)
        return _tablaTipos->headerData(section,orientation,role);
    else return QVariant();
}

void MyModeloTipoActividad::addTipoActividad(QString tipo)
{
    if(_tablaTipos) {
        int num=_tablaTipos->rowCount();
        beginInsertRows(QModelIndex(),num,num);
        QSqlRecord nRecord=_tablaTipos->record();
        nRecord.setValue(tr("tipo"),tipo);

        _tablaTipos->insertRecord(-1,nRecord);
        _tablaTipos->submitAll();
        _tablaTipos->select();
        emit nuevoTipoActividad();
        endInsertRows();
    }
}

void MyModeloTipoActividad::delTipoActividad(QString tipo)
{
    //En teoria, todas las actividades han sido ya modificadas desde MyActividadModels
    if(_tablaTipos) {
        bool found=false;
        int i;
        for(i=0;i<_tablaTipos->rowCount() && !found;i++) {
            if(_tablaTipos->index(i,0).data(Qt::EditRole).toString()==tipo)
                found=true;
        }

        if(!found)
            return;

        beginRemoveRows(QModelIndex(),i,i);

        QSqlQuery queryUpdate(_tablaTipos->database());

        _tablaTipos->database().transaction();

        queryUpdate.prepare("DELETE FROM tiposActividad WHERE tipo=:tipo");

        queryUpdate.bindValue(":tipo",tipo);

        if(!queryUpdate.exec()) {
            qDebug() << "ERROR en MyActividadModels->delTipoActividad: "<<queryUpdate.lastError().text();
            _tablaTipos->database().rollback();
            return;
        }

        _tablaTipos->database().commit();

        _tablaTipos->select();
        emit borradoTipoActividad();

        endRemoveRows();

        int filas=_tablaTipos->rowCount();
        int columnas=_tablaTipos->columnCount();

        emit dataChanged(this->index(0,0),this->index(filas,columnas));
    }
}
