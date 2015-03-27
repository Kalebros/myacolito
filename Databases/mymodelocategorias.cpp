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

#include "mymodelocategorias.h"

MyModeloCategorias::MyModeloCategorias(QObject *parent, const QSqlDatabase db) :
    QAbstractTableModel(parent)
{
    if(!db.isValid() || !db.isOpen()) {
        _tablaCategorias=0;
    }
    else {
        _tablaCategorias=new QSqlTableModel(this,db);
        _tablaCategorias->setTable(tr("Categoria"));
        _tablaCategorias->setFilter(tr("nombre!='SIN_CATEGORIA'"));
        _tablaCategorias->select();
        _tablaCategorias->sort(0,Qt::AscendingOrder);

        updateNumPlazas();
    }
}

void MyModeloCategorias::updateNumPlazas()
{
    {
        QSqlQuery query(_tablaCategorias->database());
        query.exec("SELECT nombre,numeroPlazas(nombre) FROM Categoria WHERE nombre!='SIN_CATEGORIA'");

        _mapaPlazas.clear();

        while(query.next())
            _mapaPlazas.insert(query.value(0).toString(),query.value(1).toInt());
    }
}

int MyModeloCategorias::rowCount(const QModelIndex &parent) const
{
    return _tablaCategorias->rowCount(parent);
}

int MyModeloCategorias::columnCount(const QModelIndex &parent) const
{
    return _tablaCategorias->columnCount(parent)+1;
}

Qt::ItemFlags MyModeloCategorias::flags(const QModelIndex &index) const
{
    Qt::ItemFlags res=QAbstractTableModel::flags(index);

    //Ajuste de version
#if QT_VERSION >=0x050000
    return (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
#else
    if(index.column()<_tablaCategorias->columnCount())
        res=_tablaCategorias->flags(index);

    return res;
#endif
}

QVariant MyModeloCategorias::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QVariant res;

    if(index.column()<_tablaCategorias->columnCount())
        res=_tablaCategorias->data(index,role);
    else if(index.column()==5){
        if(role==Qt::DisplayRole) {
            QString categoria=_tablaCategorias->data(_tablaCategorias->index(index.row(),0),Qt::DisplayRole).toString();
            res=QVariant(_mapaPlazas.value(categoria));
        }
    }

    return res;
}

bool MyModeloCategorias::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    bool res=_tablaCategorias->setData(index,value,role);
    _tablaCategorias->submit();
    emit dataChanged(index,index);

    if(index.column()==0) //Actualizar nombre de categoria, cambiar los demas
    {
        actualizaNumeroCategoria(index.data().toString());
    }
    emit cambiosCategoria();
    return res;
}

QVariant MyModeloCategorias::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole) {
        switch(section) {
        case 0:
            res=QString(tr("Nombre"));
            break;
        case 1:
            res=QString(tr("Precio"));
            break;
        case 2:
            res=QString(tr("Maximo"));
            break;
        case 3:
            res=QString(tr("Entrada"));
            break;
        case 4:
            res=QString(tr("Salida"));
            break;
        case 5:
            res=QString(tr("Actuales"));
            break;
        default:
            break;
        }
    }
    else if(orientation==Qt::Vertical && role==Qt::DisplayRole) {
        res=QAbstractTableModel::headerData(section,orientation,role);
    }

    return res;
}

void MyModeloCategorias::actualizaNumeroCategoria(const QString categoria)
{
    //@todo REVISAR
    {
        updateNumPlazas();
        for(int i=0;i<_tablaCategorias->rowCount();i++)
            emit dataChanged(index(i,_tablaCategorias->columnCount()),
                            index(i,_tablaCategorias->rowCount()));
    }
}

int MyModeloCategorias::categoriaRow(QString nombre)
{
    bool found=false;
    int res=-1;
    for(int i=0;i<_tablaCategorias->rowCount() && !found;i++) {
        if(_tablaCategorias->data(_tablaCategorias->index(i,0)).toString()==nombre) {
            res=i;
            found=true;
        }
    }

    return res;
}

void MyModeloCategorias::creaNuevaCategoria(QVariantMap nuevaCategoria)
{
    //TODO: implementar
    if(_tablaCategorias) {
        beginInsertRows(QModelIndex(),_tablaCategorias->rowCount(),_tablaCategorias->rowCount());
        QSqlRecord record=_tablaCategorias->record();

        record.setValue(tr("nombre"),nuevaCategoria.value("nombre").toString());
        record.setValue(tr("precio"),nuevaCategoria.value("precio").toFloat());
        record.setValue(tr("maximo"),nuevaCategoria.value("maximo").toInt());
        record.setValue(tr("fechaInicio"),nuevaCategoria.value("fechaInicio").toDate());
        record.setValue(tr("fechaLimite"),nuevaCategoria.value("fechaLimite").toDate());

        _tablaCategorias->insertRecord(-1,record);

        _tablaCategorias->submitAll();

        updateNumPlazas();
        endInsertRows();
    }
}

void MyModeloCategorias::borraCategoria(QString categoria, QString catSubstituto)
{
    if(_tablaCategorias) {
        //Vamos a eliminar una categoria y vamos a actualizar los datos de todas las demas

        _tablaCategorias->database().transaction();

        QSqlQuery queryUpdate(_tablaCategorias->database());

        queryUpdate.prepare("UPDATE plaza SET tipo=:nuevaCat WHERE tipo=:oldCat");
        queryUpdate.bindValue(":nuevaCat",catSubstituto);
        queryUpdate.bindValue(":oldCat",categoria);

        if(!queryUpdate.exec()) {
            qDebug() << "ERROR en MyModeloCategorias->borraCategoria: " << queryUpdate.lastError().text();
            _tablaCategorias->database().rollback();
            return;
        }

        queryUpdate.prepare("DELETE FROM categoria WHERE nombre=:categoria");
        queryUpdate.bindValue(":categoria",categoria);

        if(!queryUpdate.exec()) {
            qDebug() << "ERROR en MyModeloCategorias->borraCategoria: " << queryUpdate.lastError().text();
            _tablaCategorias->database().rollback();
            return;
        }

        _tablaCategorias->database().commit();

        beginRemoveRows(QModelIndex(),_tablaCategorias->rowCount(),_tablaCategorias->rowCount());
        _tablaCategorias->select();
        updateNumPlazas();
        endRemoveRows();
    }
}
