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

#include "mymodelotiposhab.h"

MyModeloTiposHab::MyModeloTiposHab(QObject *parent,const QSqlDatabase db) :
    QAbstractTableModel(parent),
    modeloTipos(0)
{
    if(!db.isOpen() || !db.isValid()) {}
    else {
        modeloTipos=new QSqlTableModel(this,db);

        modeloTipos->setTable(tr("tipoHab"));
        modeloTipos->setFilter(tr("nombreTipo!='SIN_TIPO'"));

        modeloTipos->select();
    }
}

int MyModeloTiposHab::rowCount(const QModelIndex &parent) const
{
    if(modeloTipos)
        return modeloTipos->rowCount();
    else return 0;
}

int MyModeloTiposHab::columnCount(const QModelIndex &parent) const
{
    if(modeloTipos)
        return modeloTipos->columnCount();
    else return 0;
}

QVariant MyModeloTiposHab::data(const QModelIndex &index, int role) const
{
    if(modeloTipos)
        return modeloTipos->data(index,role);
    else return QVariant();
}

bool MyModeloTiposHab::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(modeloTipos) {
        modeloTipos->setData(index,value,role);
        modeloTipos->submitAll();
        emit dataChanged(index,index);
        emit cambiosTipos();
        return true;
    }
    else return false;
}

Qt::ItemFlags MyModeloTiposHab::flags(const QModelIndex &index) const
{
    //Ajuste de version
#if QT_VERSION >=0x050000
    return (Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
#else
    if(modeloTipos)
        return modeloTipos->flags(index);
    else return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
#endif
}

QVariant MyModeloTiposHab::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(modeloTipos)
        return modeloTipos->headerData(section,orientation,role);
    else return QVariant();
}

void MyModeloTiposHab::nuevoTipoHabitacion(QVariantMap datos)
{
    if(modeloTipos) {
        beginInsertRows(QModelIndex(),modeloTipos->rowCount(),modeloTipos->rowCount());
        QSqlRecord record=modeloTipos->record();

        record.setValue(tr("nombreTipo"),datos.value("nombreTipo"));
        record.setValue(tr("plazas"),datos.value("plazas"));
        record.setValue(tr("categoriaPermitida"),datos.value("categoriaPermitida"));

        modeloTipos->insertRecord(-1,record);
        modeloTipos->submitAll();
        modeloTipos->select();
        emit nuevaCategoria();
        endInsertRows();
    }
}

void MyModeloTiposHab::borraTipoHabitacion(QString tipo,QString sustituto)
{
    //TODO: implementar y revisar

    if(modeloTipos) {
        bool found=false;
        int i;
        for(i=0;i<modeloTipos->rowCount() && !found;i++) {
            if(modeloTipos->index(i,0).data(Qt::EditRole).toString()==tipo)
                found=true;
        }

        if(!found)
            return;

        beginRemoveRows(QModelIndex(),i,i);

        QSqlQuery queryUpdate(modeloTipos->database());

        modeloTipos->database().transaction();
        queryUpdate.prepare("UPDATE habitacion SET tipoHab=:sustituto WHERE tipoHab=:tipo");

        queryUpdate.bindValue(":sustituto",sustituto);
        queryUpdate.bindValue(":tipo",tipo);

        if(!queryUpdate.exec()) {
            qDebug() << "ERROR en MyHabitacionModels->borraTipoHabitacion: "<<queryUpdate.lastError().text();
            modeloTipos->database().rollback();
            return;
        }

        queryUpdate.prepare("DELETE FROM tipoHab WHERE nombreTipo=:tipo");

        queryUpdate.bindValue(":tipo",tipo);

        if(!queryUpdate.exec()) {
            qDebug() << "ERROR en MyHabitacionModels->borraTipoHabitacion: "<<queryUpdate.lastError().text();
            modeloTipos->database().rollback();
            return;
        }

        modeloTipos->database().commit();

        modeloTipos->select();
        emit cambiosTipos();
        endRemoveRows();

        int filas=modeloTipos->rowCount();
        int columnas=modeloTipos->columnCount();

        emit dataChanged(this->index(0,0),this->index(filas,columnas));
    }
}
