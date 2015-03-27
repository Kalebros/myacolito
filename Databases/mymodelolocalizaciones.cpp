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

#include "mymodelolocalizaciones.h"

MyModeloLocalizaciones::MyModeloLocalizaciones(QObject *parent, const QSqlDatabase db) :
    QAbstractTableModel(parent)
{
    if(!db.isOpen() || !db.isValid()) {}
    else {
        _tablaLugar=new QSqlTableModel(this,db);

        _tablaLugar->setTable(tr("localizaciones"));
        _tablaLugar->select();
    }
}

int MyModeloLocalizaciones::rowCount(const QModelIndex &parent) const
{
    if(_tablaLugar)
        return _tablaLugar->rowCount();
    else return 0;
}

int MyModeloLocalizaciones::columnCount(const QModelIndex &parent) const
{
    if(_tablaLugar)
        return _tablaLugar->columnCount();
    else return 0;
}

QVariant MyModeloLocalizaciones::data(const QModelIndex &index, int role) const
{
    if(_tablaLugar)
        return _tablaLugar->data(index,role);
    else return QVariant();
}

bool MyModeloLocalizaciones::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(_tablaLugar) {
        _tablaLugar->setData(index,value,role);
        _tablaLugar->submitAll();
        emit dataChanged(index,index);
        emit editadaLocalizacion();
        return true;
    }
    else return false;
}

Qt::ItemFlags MyModeloLocalizaciones::flags(const QModelIndex &index) const
{
    //Ajuste de version
#if QT_VERSION >=0x050000
    return (Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
#else
    if(_tablaLugar)
        return _tablaLugar->flags(index);
    else return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
#endif
}

QVariant MyModeloLocalizaciones::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(_tablaLugar)
        return _tablaLugar->headerData(section,orientation,role);
    else return QVariant();
}

void MyModeloLocalizaciones::addLocalizacion(QString lugar)
{
    if(_tablaLugar) {
        int num=_tablaLugar->rowCount();
        beginInsertRows(QModelIndex(),num,num);
        QSqlRecord nRecord=_tablaLugar->record();
        nRecord.setValue(tr("lugar"),lugar);

        _tablaLugar->insertRecord(-1,nRecord);
        _tablaLugar->submitAll();
        _tablaLugar->select();
        emit nuevaLocalizacion();
        endInsertRows();
    }
}

void MyModeloLocalizaciones::delLocalizacion(QString lugar)
{
    //En teoria, todas las actividades han sido ya modificadas desde MyActividadModels
    if(_tablaLugar) {
        bool found=false;
        int i;
        for(i=0;i<_tablaLugar->rowCount() && !found;i++) {
            if(_tablaLugar->index(i,0).data(Qt::EditRole).toString()==lugar)
                found=true;
        }

        if(!found)
            return;

        beginRemoveRows(QModelIndex(),i,i);

        QSqlQuery queryUpdate(_tablaLugar->database());

        _tablaLugar->database().transaction();

        queryUpdate.prepare("DELETE FROM localizaciones WHERE lugar=:lugar");

        queryUpdate.bindValue(":lugar",lugar);

        if(!queryUpdate.exec()) {
            qDebug() << "ERROR en MyActividadModels->delLocalizacion: "<<queryUpdate.lastError().text();
            _tablaLugar->database().rollback();
            return;
        }

        _tablaLugar->database().commit();

        _tablaLugar->select();
        emit borradaLocalizacion();

        endRemoveRows();

        int filas=_tablaLugar->rowCount();
        int columnas=_tablaLugar->columnCount();

        emit dataChanged(this->index(0,0),this->index(filas,columnas));
    }
}
