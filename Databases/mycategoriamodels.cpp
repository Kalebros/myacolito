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

#include "mycategoriamodels.h"

MyCategoriaModels::MyCategoriaModels(QObject *parent,MyAcolitoDB *db) :
    MyAcolitoModels(parent,db),
    _modeloBase(0)
{
}

bool MyCategoriaModels::isActive() const
{
    if(!_modeloBase)
        return false;
    return true;
}

void MyCategoriaModels::deleteModelos()
{
    if(_modeloBase) {
        delete _modeloBase;
        _modeloBase=0;
    }
}

void MyCategoriaModels::loadModelos()
{
    if(_db && !_modeloBase) {
        _modeloBase=new MyModeloCategorias(this,_db->database());
        connect(_modeloBase,SIGNAL(cambiosCategoria()),this,SIGNAL(cambiosCategoria()));

        emit changeModelos();
    }
}

QSortFilterProxyModel *MyCategoriaModels::getModeloCategorias()
{
    QSortFilterProxyModel *res=0;

    if(_db) {
        if(!_modeloBase) {
            _modeloBase=new MyModeloCategorias(this,_db->database());
        }

        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloBase);
    }

    return res;
}

void MyCategoriaModels::crearNuevaCategoria(QVariantMap datosCategoria)
{
    if(_db) {
        if(!_modeloBase)
            _modeloBase=new MyModeloCategorias(this,_db->database());

        _modeloBase->creaNuevaCategoria(datosCategoria);
    }
}

void MyCategoriaModels::borrarCategoria(QString cat,QString sustituta)
{
    if(_db) {
        if(!_modeloBase)
            _modeloBase=new MyModeloCategorias(this,_db->database());

        _modeloBase->borraCategoria(cat,sustituta);
    }
}

void MyCategoriaModels::updateCategoria(const QString cat)
{
    if(_modeloBase)
        _modeloBase->actualizaNumeroCategoria(cat);
}
