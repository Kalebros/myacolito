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

#include "mymodelohabitaciones.h"

MyModeloHabitaciones::MyModeloHabitaciones(QObject *parent, const QSqlDatabase db) :
    QAbstractTableModel(parent),
    _modeloHabitaciones(0),
    _modeloAlojados(0)
{
    if(!db.isValid() || !db.isOpen()) {
        _modeloHabitaciones=0;
        _modeloAlojados=0;
    }
    else
    {
        _modeloHabitaciones=new QSqlTableModel(this,db);
        _modeloHabitaciones->setTable(tr("Habitacion"));
        _modeloHabitaciones->setFilter(tr("codHab!='Sin asignar'"));

        _modeloHabitaciones->select();

        _modeloAlojados=new QSqlTableModel(this,db);
        _modeloAlojados->setTable(tr("alojado_en"));

        _modeloAlojados->select();
    }

    //Inicializar iconos
    iconoOk=QIcon(":/iconos/iconoModelo");
    iconoError=QIcon(":/iconos/iconoError");
}


int MyModeloHabitaciones::rowCount(const QModelIndex &parent) const
{
    if(_modeloHabitaciones)
        return _modeloHabitaciones->rowCount();

    else return 0;
}

int MyModeloHabitaciones::columnCount(const QModelIndex &parent) const
{
    //Una columna adicional: si la habitacion esta libre o no
    if(_modeloHabitaciones)
        return _modeloHabitaciones->columnCount()+1;
    else return 0;
}

QVariant MyModeloHabitaciones::data(const QModelIndex &index, int role) const
{
    if(!_modeloHabitaciones || !index.isValid())
        return QVariant();

    if((index.column()==4 || index.column()==3) && role==Qt::DecorationRole) {
        int valor=_modeloHabitaciones->index(index.row(),index.column()).data(Qt::EditRole).toInt();
        if(valor)
            return QVariant(iconoOk);
        else return QVariant(iconoError);
    }

    if(index.column() < _modeloHabitaciones->columnCount())
        return _modeloHabitaciones->data(index,role);


    //Devuelve un 0 si esta ocupada o un 1 si esta libre
    if(index.column()==6)
    {
        QVariant res=QVariant(1);

        QString codHab=_modeloHabitaciones->index(index.row(),0).data(Qt::EditRole).toString();

        bool found=false;
        for(int i=0;i<_modeloAlojados->rowCount() && !found;i++) {
            if(_modeloAlojados->index(i,1).data(Qt::EditRole).toString()==codHab) {
                res=QVariant(0);
                found=true;
            }
        }

        if(role==Qt::EditRole || role==Qt::DisplayRole)
            return res;
        if(role==Qt::DecorationRole) {
            if(res.toBool())
                return QVariant(iconoOk);
            else return QVariant(iconoError);
        }
    }

    return QVariant();
}

QVariant MyModeloHabitaciones::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole) {
        switch(section) {
        case 0:
            res=QString(tr("Habitacion"));
            break;
        case 1:
            res=QString(tr("Tipo"));
            break;
        case 2:
            res=QString(tr("Comentario"));
            break;
        case 3:
            res=QString(tr("Abierta"));
            break;
        case 4:
            res=QString(tr("Reservada"));
            break;
        case 5:
            res=QString(tr("ConceptoReserva"));
            break;
        case 6:
            res=QString(tr("Libre"));
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

Qt::ItemFlags MyModeloHabitaciones::flags(const QModelIndex &index) const
{
    if(index.column()<6)
        return (Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    if(index.column()==6)
        return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

bool MyModeloHabitaciones::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool res=_modeloHabitaciones->setData(index,value,role);
    _modeloHabitaciones->submitAll();
    _modeloHabitaciones->select();

    emit dataChanged(index,index);

    if(index.row()==0)  // Se ha cambiado el codigo de una habitacion
        emit cambiosCodigoHabitaciones();
    return res;
}

void MyModeloHabitaciones::addNuevaHabitacion(Habitacion *habitacion)
{
    if(habitacion->codigo.isEmpty() || habitacion->codigo.isNull())
        return;
    if(habitacion->tipo.isEmpty() || habitacion->tipo.isNull())
        return;

    beginInsertRows(QModelIndex(),_modeloHabitaciones->rowCount(),_modeloHabitaciones->rowCount());

    QSqlRecord nHabitacion=_modeloHabitaciones->record();

    nHabitacion.setValue(tr("codHab"),habitacion->codigo);
    nHabitacion.setValue(tr("tipoHab"),habitacion->tipo);
    nHabitacion.setValue(tr("comentario"),habitacion->comentario);
    nHabitacion.setValue(tr("llaveEntregada"),habitacion->llaveEntregada);
    nHabitacion.setValue(tr("estaReservada"),habitacion->estaReservada);
    nHabitacion.setValue(tr("conceptoReserva"),habitacion->conceptoReserva);

    if(!_modeloHabitaciones->insertRecord(-1,nHabitacion)) {
        qDebug() << "ERROR en tabla Habitaciones: "+_modeloHabitaciones->lastError().text();
        endInsertRows();
        return;
    }

    _modeloHabitaciones->submitAll();
    _modeloAlojados->select();

    emit cambiosCodigoHabitaciones();
    endInsertRows();
}

void MyModeloHabitaciones::desalojarHabitacion(QString codHab)
{
    if(_modeloHabitaciones && _modeloAlojados) {
        int i;
        int numHabitaciones=_modeloHabitaciones->rowCount();
        bool found=false;
        for(i=0;i<numHabitaciones && !found;i++) {
            if(_modeloAlojados->record(i).value("codHab").toString()==codHab) {
                _modeloAlojados->setData(_modeloAlojados->index(i,1),tr("Sin asignar"),Qt::EditRole);
                found=true;
            }
        }

        _modeloAlojados->submitAll();
        _modeloAlojados->select();

        emit cambiosCodigoHabitaciones();
    }
}

void MyModeloHabitaciones::deleteHabitacion(Habitacion *habitacion)
{
    deleteHabitacion(habitacion->codigo);
}

void MyModeloHabitaciones::deleteHabitacion(QString codHab)
{
    QModelIndex indexHab=getModelIndexOf(codHab);
    beginRemoveRows(QModelIndex(),indexHab.row(),indexHab.row());

    _modeloHabitaciones->removeRow(indexHab.row());
    _modeloHabitaciones->submitAll();
    _modeloHabitaciones->select();
    _modeloAlojados->select();

    emit cambiosCodigoHabitaciones();
    endRemoveRows();
}

QModelIndex MyModeloHabitaciones::getModelIndexOf(QString codHab)
{
    QModelIndex res;
    bool found=false;
    int numFilas=_modeloHabitaciones->rowCount();
    for(int i=0;i<numFilas && !found;i++) {
        if(_modeloHabitaciones->index(i,0).data().toString()==codHab) {
            res=_modeloHabitaciones->index(i,0);
            found=true;
        }
    }
    return res;
}

bool MyModeloHabitaciones::estaAbierta(QString codHab)
{
    QModelIndex res;
    res=getModelIndexOf(codHab);

    return _modeloHabitaciones->index(res.row(),3).data(Qt::EditRole).toBool();
}

void MyModeloHabitaciones::setLlaveHabitacion(QString codHab, bool estado)
{
    QModelIndex index=getModelIndexOf(codHab);
    QModelIndex idx=_modeloHabitaciones->index(index.row(),3);

    _modeloHabitaciones->setData(idx,estado,Qt::EditRole);
    emit dataChanged(this->index(idx.row(),idx.column()),
                     this->index(idx.row(),idx.column()));
}

void MyModeloHabitaciones::updateTiposHabitacion()
{
    if(_modeloHabitaciones) {
        _modeloHabitaciones->select();

        for(int i=0;i<_modeloHabitaciones->rowCount();i++)
            emit dataChanged(this->index(i,1),this->index(i,1));
    }
}

void MyModeloHabitaciones::updateHabitacionesLibres()
{
    if(_modeloHabitaciones && _modeloAlojados) {
        _modeloAlojados->select();

        int numHabitaciones=_modeloHabitaciones->rowCount();

        for(int i=0;i<numHabitaciones;i++)
            emit dataChanged(index(i,6),index(i,6));
    }
}
