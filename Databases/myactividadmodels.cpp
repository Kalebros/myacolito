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

#include "myactividadmodels.h"

MyActividadModels::MyActividadModels(QObject *parent, MyAcolitoDB *db) :
    MyAcolitoModels(parent,db),
    _modeloActividades(0),
    _modeloTipos(0),
    _modeloLugares(0)
{
}

bool MyActividadModels::isActive() const
{
    if(_modeloActividades && _modeloTipos && _modeloLugares)
        return true;
    else return false;
}

void MyActividadModels::deleteModelos()
{
    if(_modeloActividades) {
        delete _modeloActividades;
        _modeloActividades=0;
    }

    if(_modeloTipos) {
        delete _modeloTipos;
        _modeloTipos=0;
    }

    if(_modeloLugares) {
        delete _modeloLugares;
        _modeloLugares=0;
    }
}

void MyActividadModels::loadModelos()
{
    if(_db && !_modeloActividades) {
        _modeloActividades=new MyModeloActividades(this,_db->database());
    }

    if(_db && !_modeloTipos) {
        _modeloTipos=new MyModeloTipoActividad(this,_db->database());

        connect(_modeloTipos,SIGNAL(editadoTipoActividad()),_modeloActividades,SLOT(reloadTipos()));
    }

    if(_db && !_modeloLugares) {
        _modeloLugares=new MyModeloLocalizaciones(this,_db->database());

        connect(_modeloLugares,SIGNAL(editadaLocalizacion()),_modeloActividades,SLOT(reloadLugares()));
    }
}

QSortFilterProxyModel *MyActividadModels::getModeloActividades()
{
    QSortFilterProxyModel *res=0;

    if(_db) {
        if(!_modeloActividades) {
            _modeloActividades=new MyModeloActividades(this,_db->database());
        }
        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloActividades);
    }

    return res;
}

QSortFilterProxyModel *MyActividadModels::getModeloLocalizaciones()
{
    QSortFilterProxyModel *res=0;
    if(_db) {
        if(!_modeloLugares) {
            _modeloLugares=new MyModeloLocalizaciones(this,_db->database());

            connect(_modeloLugares,SIGNAL(editadaLocalizacion()),_modeloActividades,SLOT(reloadLugares()));
        }
        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloLugares);
    }

    return res;
}

QSortFilterProxyModel *MyActividadModels::getModeloTiposActividad()
{
    QSortFilterProxyModel *res=0;
    if(_db) {
        if(!_modeloTipos) {
            _modeloTipos=new MyModeloTipoActividad(this,_db->database());

            connect(_modeloTipos,SIGNAL(editadoTipoActividad()),_modeloActividades,SLOT(reloadTipos()));
        }
        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloTipos);
    }

    return res;
}

void MyActividadModels::activarTodasLasJornadas(int idActividad, bool modo)
{
    if(_modeloActividades)
        _modeloActividades->activarTodasLasJornadas(idActividad,modo);
}

bool MyActividadModels::getModoTodasLasJornadas(int idActividad)
{
    if(_modeloActividades)
        return _modeloActividades->getTodasLasJornadas(idActividad);
    else return false;
}

void MyActividadModels::addTipoActividad(QString tipo)
{
    if(_modeloTipos) {
        _modeloTipos->addTipoActividad(tipo);
    }
}

void MyActividadModels::delTipoActividad(QString tipo, QString sustituto)
{
    if(_modeloTipos && _modeloActividades) {
        _modeloActividades->cambiarTipoActividades(tipo,sustituto);
        _modeloTipos->delTipoActividad(tipo);
    }
}

void MyActividadModels::addLocalizacion(QString lugar)
{
    if(_modeloLugares)
        _modeloLugares->addLocalizacion(lugar);
}

void MyActividadModels::delLocalizacion(QString lugar, QString sustituto)
{
    if(_modeloLugares && _modeloActividades) {
        _modeloActividades->cambiarLocalizaciones(lugar,sustituto);
        _modeloLugares->delLocalizacion(lugar);
    }
}

void MyActividadModels::addActividad(QVariantMap actividad)
{
    if(_modeloActividades)
        _modeloActividades->addActividad(actividad);
}

void MyActividadModels::addListaActividades(QList<QVariantMap> listaActividades)
{
    if(_modeloActividades)
        _modeloActividades->addListaActividad(listaActividades);
}

void MyActividadModels::delActividad(int id)
{
    if(_modeloActividades)
        _modeloActividades->delActividad(id);
}

void MyActividadModels::delActividad(QVariant id)
{
    if(_modeloActividades)
        _modeloActividades->delActividad(id);
}

QString MyActividadModels::getOrganizador(int idActividad)
{
    if(_modeloActividades)
        return _modeloActividades->getOrganizador(idActividad);
    return QString();
}
