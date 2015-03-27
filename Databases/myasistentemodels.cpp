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

#include "myasistentemodels.h"

MyAsistenteModels::MyAsistenteModels(QObject *parent, MyAcolitoDB *db) :
    MyAcolitoModels(parent,db),
    _modeloAsistentes(0)
{
}

bool MyAsistenteModels::isActive() const
{
    if(!_modeloAsistentes)
        return false;
    return true;
}

QSortFilterProxyModel *MyAsistenteModels::getModeloAsistentes()
{
    QSortFilterProxyModel *res=0;

    if(_db) {
        if(!_modeloAsistentes) {
            _modeloAsistentes=new MyModeloAsistente(this,_db->database());
        }

        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloAsistentes);
    }

    return res;
}

void MyAsistenteModels::deleteModelos()
{
    if(_modeloAsistentes) {
        delete _modeloAsistentes;
        _modeloAsistentes=0;
    }
}

void MyAsistenteModels::loadModelos()
{
    if(_db && !_modeloAsistentes) {
        _modeloAsistentes=new MyModeloAsistente(this,_db->database());
        connect(_modeloAsistentes,SIGNAL(borradaPlaza(QString)),this,SIGNAL(borradaPlaza(QString)));
        connect(_modeloAsistentes,SIGNAL(nuevaPlaza(QString)),this,SIGNAL(nuevaPlaza(QString)));
        connect(_modeloAsistentes,SIGNAL(cambiosGrupo()),this,SIGNAL(cambiosGrupo()));

        //Depurar
        //connect(_modeloAsistentes,SIGNAL(borradaPlaza(QString)),this,SLOT(depurar(QString)));
        //connect(_modeloAsistentes,SIGNAL(nuevaPlaza(QString)),this,SLOT(depurar(QString)));

        emit changeModelos();
    }
}

void MyAsistenteModels::addAsistente(Asistente *asis)
{
    if(_db && _modeloAsistentes)
        _modeloAsistentes->addAsistente(asis);
}

void MyAsistenteModels::eliminarAsistente(Asistente *asis)
{
    //TODO: TESTEAR
    if(_db && _modeloAsistentes)
        _modeloAsistentes->deleteAsistente(asis);
}

void MyAsistenteModels::agruparAsistentes(QList<Asistente *> lista)
{
    if(_db && _modeloAsistentes)
        _modeloAsistentes->agruparAsistentes(lista);
}

void MyAsistenteModels::separarAsistente(Asistente *asis)
{
    if(_db &&_modeloAsistentes)
        _modeloAsistentes->separarAsistente(asis->idAsis);
}

void MyAsistenteModels::nuevoGrupoAsistentes(QList<Asistente*> lista)
{
    if(_db && _modeloAsistentes)
        _modeloAsistentes->nuevoGrupoAsistentes(lista);
}

void MyAsistenteModels::cambiarAsistenteDeGrupo(Asistente *asis, int idGrupo)
{
    if(_db && _modeloAsistentes)
        _modeloAsistentes->cambiarAsistenteDeGrupo(asis,idGrupo);
}

QIcon MyAsistenteModels::getIconoOk() const
{
    QIcon icono;
    if(_modeloAsistentes)
        icono=_modeloAsistentes->getIconoOk();
    return icono;
}

QIcon MyAsistenteModels::getIconoFalse() const
{
    QIcon icono;
    if(_modeloAsistentes)
        icono=_modeloAsistentes->getIconoFalse();
    return icono;
}

void MyAsistenteModels::depurar(QString mensaje)
{
    qDebug() << mensaje;
}

void MyAsistenteModels::recargarDatosCategorias()
{
    if(_modeloAsistentes)
        _modeloAsistentes->updateCategorias();
}

bool MyAsistenteModels::getPagoAsistente(QString dni) const
{
    if(_modeloAsistentes)
        return _modeloAsistentes->pagoAsistente(dni);

    else return 0;
}
