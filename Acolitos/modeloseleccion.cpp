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

#include "modeloseleccion.h"

ModeloSeleccion::ModeloSeleccion(QObject *parent) :
    QSortFilterProxyModel(parent),
    _columnaSeleccion(-1),
    _columnaIdentificador(-1),
    _listaSeleccion(),
    _seleccionExclusiva(false)
{
}

void ModeloSeleccion::setColumnaIdentificador(int columna)
{
    _listaSeleccion.clear();
    _columnaIdentificador=columna;
}

int ModeloSeleccion::getColumnaIdentificador() const
{
    return _columnaIdentificador;
}

void ModeloSeleccion::setColumnaSeleccionable(int columna)
{
    _listaSeleccion.clear();
    _columnaSeleccion=columna;
}

int ModeloSeleccion::getColumnaSeleccionable() const
{
    return _columnaSeleccion;
}

QVariantList ModeloSeleccion::elementosMarcados() const
{
    return _listaSeleccion;
}

Qt::ItemFlags ModeloSeleccion::flags(const QModelIndex &index) const
{
    Qt::ItemFlags res=Qt::NoItemFlags;
    if(_columnaIdentificador==-1 && _columnaSeleccion==-1)
        res=QSortFilterProxyModel::flags(index);
    else {
        Qt::ItemFlags flags=QSortFilterProxyModel::flags(index);
        if(index.column()==_columnaSeleccion)
            res=(flags | Qt::ItemIsUserCheckable);
        else res=flags;
    }

    return res;
}

void ModeloSeleccion::setSeleccionExclusiva(bool modo)
{
    _seleccionExclusiva=modo;
    _listaSeleccion.clear();
    for(int i=0;i<sourceModel()->rowCount();i++)
        emit dataChanged(this->index(i,_columnaSeleccion),this->index(i,_columnaSeleccion));
}

QVariant ModeloSeleccion::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if(index.column()!=_columnaSeleccion)
        res=QSortFilterProxyModel::data(index,role);
    else if(index.column()==_columnaSeleccion && role==Qt::CheckStateRole) {
        QVariant dataSelect=QSortFilterProxyModel::data(this->index(index.row(),_columnaIdentificador),Qt::EditRole);
        if(_listaSeleccion.contains(dataSelect))
            res=Qt::Checked;
        else res=Qt::Unchecked;
    }
    else res=QSortFilterProxyModel::data(index,role);

    return res;
}

bool ModeloSeleccion::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.column()!=_columnaSeleccion)
        return QSortFilterProxyModel::setData(index,value,role);
    else if(index.column()==_columnaSeleccion && role==Qt::CheckStateRole) {
        if(value.toInt()==0) //Qt::Unchecked
            _listaSeleccion.removeAll(QSortFilterProxyModel::data(this->index(index.row(),_columnaIdentificador),Qt::EditRole));
        else {
            if(_seleccionExclusiva) {//Seleccion exclusiva, limpia la seleccion
                _listaSeleccion.clear();
                for(int i=0;i<sourceModel()->rowCount();i++)
                    emit dataChanged(this->index(i,_columnaSeleccion),this->index(i,_columnaSeleccion));
            }
            _listaSeleccion.append(QSortFilterProxyModel::data(this->index(index.row(),_columnaIdentificador),Qt::EditRole));
        }
        emit dataChanged(index,index);
    }
    return true;
}

void ModeloSeleccion::marcar(int fila, bool estado)
{
    QVariant dataValue=QSortFilterProxyModel::data(index(fila,_columnaIdentificador),Qt::EditRole);

    if(estado==Qt::Checked) {
        if(!_listaSeleccion.contains(dataValue)) {
            if(_seleccionExclusiva) {//Seleccion exclusiva, limpia la seleccion
                _listaSeleccion.clear();
                for(int i=0;i<sourceModel()->rowCount();i++)
                    emit dataChanged(this->index(i,_columnaSeleccion),this->index(i,_columnaSeleccion));
            }
            _listaSeleccion.append(dataValue);
        }
    }
    else {
        _listaSeleccion.removeAll(dataValue);
    }
    emit dataChanged(index(fila,_columnaSeleccion),index(fila,_columnaSeleccion));
}
