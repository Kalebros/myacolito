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

#include "myhabitacionmodels.h"

MyHabitacionModels::MyHabitacionModels(QObject *parent, MyAcolitoDB *db) :
    MyAcolitoModels(parent,db),
    _modeloHabitacion(0),
    _modeloAlojamientos(0),
    _modeloTipoHab(0)
{
}

bool MyHabitacionModels::isActive() const
{
    if(_modeloHabitacion && _modeloAlojamientos && _modeloTipoHab)
        return true;
    else return false;
}

void MyHabitacionModels::deleteModelos()
{
    if(_modeloHabitacion) {
        delete _modeloHabitacion;
        _modeloHabitacion=0;
    }
    if(_modeloAlojamientos) {
        delete _modeloAlojamientos;
        _modeloAlojamientos=0;
    }
    if(_modeloTipoHab) {
        delete _modeloTipoHab;
        _modeloTipoHab=0;
    }
}

void MyHabitacionModels::loadModelos()
{
    if(_db && !_modeloHabitacion) {
        _modeloHabitacion=new MyModeloHabitaciones(this,_db->database());

        connect(_modeloHabitacion,SIGNAL(cambiosCodigoHabitaciones()),this,SLOT(reloadModeloAlojamientos()));
        connect(this,SIGNAL(reloadAlojamientos()),_modeloHabitacion,SLOT(updateHabitacionesLibres()));
    }
    if(_db && !_modeloAlojamientos) {
        _modeloAlojamientos=new QSqlTableModel(this,_db->database());
        _modeloAlojamientos->setTable(tr("alojado_en"));
        _modeloAlojamientos->select();
    }
    if(_db && !_modeloTipoHab) {
        _modeloTipoHab=new MyModeloTiposHab(this,_db->database());

        connect(_modeloTipoHab,SIGNAL(cambiosTipos()),_modeloHabitacion,SLOT(updateTiposHabitacion()));
    }
}

QSortFilterProxyModel *MyHabitacionModels::getModeloHabitaciones()
{
    QSortFilterProxyModel *res=0;

    if(_db) {
        if(!_modeloHabitacion) {
            _modeloHabitacion=new MyModeloHabitaciones(this,_db->database());
        }
        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloHabitacion);
    }

    return res;
}

QSortFilterProxyModel *MyHabitacionModels::getModeloAlojamientos()
{
    QSortFilterProxyModel *res=0;

    if(_db) {
        if(!_modeloAlojamientos) {
            _modeloAlojamientos=new QSqlTableModel(this,_db->database());
            _modeloAlojamientos->setTable(tr("alojado_en"));
            _modeloAlojamientos->select();
        }
        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloAlojamientos);
    }

    return res;
}

QSortFilterProxyModel *MyHabitacionModels::getModeloTipoHabitaciones()
{
    QSortFilterProxyModel *res=0;

    if(_db) {
        if(!_modeloTipoHab)
            _modeloTipoHab=new MyModeloTiposHab(this,_db->database());

        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloTipoHab);
    }
    return res;
}

void MyHabitacionModels::reloadModeloAlojamientos()
{
    if(_db && _modeloAlojamientos) {
        _modeloAlojamientos->select();
        emit reloadAlojamientos();
    }
}

void MyHabitacionModels::nuevaHabitacion(Habitacion *habitacion)
{
    if(_db && _modeloHabitacion) {
        _modeloHabitacion->addNuevaHabitacion(habitacion);
    }
}

void MyHabitacionModels::nuevoTipoHabitacion(QVariantMap datos)
{
    if(_db && _modeloTipoHab)
        _modeloTipoHab->nuevoTipoHabitacion(datos);
}

void MyHabitacionModels::borraTipoHabitacion(QString tipo, QString sustituto)
{
    if(_db && _modeloTipoHab)
        _modeloTipoHab->borraTipoHabitacion(tipo,sustituto);
}

void MyHabitacionModels::nuevaListaHabitaciones(QList<Habitacion *> listadoHabitaciones)
{
    if(_db && _modeloHabitacion) {
        foreach(Habitacion *hab,listadoHabitaciones) {
            _modeloHabitacion->addNuevaHabitacion(hab);
        }
    }
}

void MyHabitacionModels::borraHabitacion(Habitacion *habitacion)
{
    if(_db && _modeloHabitacion) {
        _modeloHabitacion->deleteHabitacion(habitacion->codigo);
    }
}

void MyHabitacionModels::borraListaHabitaciones(QList<Habitacion *> listadoHabitaciones)
{
    if(_db && _modeloHabitacion) {
        foreach(Habitacion *hab,listadoHabitaciones) {
            _modeloHabitacion->deleteHabitacion(hab->codigo);
        }
    }
}

void MyHabitacionModels::alojarGrupoHabitacion(int IDgrupo, QString codHab)
{
    if(_db && _modeloAlojamientos) {
        QSqlQuery query(_db->database());

        _db->database().transaction();
        query.prepare("SELECT count(codHab) FROM alojado_en WHERE codHab=:codigo");

        query.bindValue(":codigo",codHab);
        if(!query.exec()) {
            qDebug() << "ERROR en MyHabitacionModels->alojarGrupoHabitacion: " << query.lastError().text();
            _db->database().rollback();
            return;
        }

        query.first();

        int presente=query.value(0).toInt();

        switch(presente) {
        case 0:
            //Ningun grupo esta alojado
            query.prepare("UPDATE alojado_en SET codHab=:codigo WHERE IDgrupo=:grupo");
            query.bindValue(":grupo",IDgrupo);
            query.bindValue(":codigo",codHab);

            if(!query.exec()) {
                qDebug() << "ERROR en MyHabitacionModels->alojarGrupoHabitacion: " << query.lastError().text();
                _db->database().rollback();
                return;
            }
            break;
        default:
            //Tenemos que dejar al otro grupo sin alojamiento antes de asignar la habitacion
            query.prepare("UPDATE alojado_en SET codHab='Sin asignar' WHERE codHab=:codigo");
            query.bindValue(":codigo",codHab);

            if(!query.exec()) {
                qDebug() << "ERROR en MyHabitacionModels->alojarGrupoHabitacion: " << query.lastError().text();
                _db->database().rollback();
                return;
            }

            query.prepare("UPDATE alojado_en SET codHab=:codigo WHERE IDgrupo=:grupo");
            query.bindValue(":codigo",codHab);
            query.bindValue(":grupo",IDgrupo);

            if(!query.exec()) {
                qDebug() << "ERROR en MyHabitacionModels->alojarGrupoHabitacion: " << query.lastError().text();
                _db->database().rollback();
                return;
            }
            break;
        }

        _db->database().commit();

        _modeloAlojamientos->select();
        emit reloadAlojamientos();
    }
}

void MyHabitacionModels::desalojarHabitacion(QString codHabitacion)
{
    if(_db && _modeloHabitacion && _modeloAlojamientos) {
        _modeloHabitacion->desalojarHabitacion(codHabitacion);
        emit reloadAlojamientos();
    }
}

void MyHabitacionModels::intercambiarHabitacionesGrupos(int IDgrupoA, int IDgrupoB)
{
    if(_db && _modeloAlojamientos) {
        QSqlQuery query(_db->database());

        _db->database().transaction();

        //Recuperar codigos de habitaciones de los dos grupos
        QString codigoA,codigoB;

        query.prepare("SELECT IDgrupo,codHab FROM alojado_en WHERE IDgrupo=:grupoA OR IDgrupo=:grupoB");
        query.bindValue(":grupoA",IDgrupoA);
        query.bindValue(":grupoB",IDgrupoB);

        if(!query.exec()) {
            qDebug() << "ERROR en MyHabitacionModels->intercambiarHabitacionesGrupos: " << query.lastError().text();
            _db->database().rollback();
            return;
        }

        query.first();
        if(query.value(0).toInt()==IDgrupoA) {
            codigoA=query.value(1).toString();
            query.next();
            codigoB=query.value(1).toString();
        }
        else {
            codigoB=query.value(1).toString();
            query.next();
            codigoA=query.value(1).toString();
        }

        //Preparar listas
        QVariantList grupos,codigos;
        grupos << IDgrupoA << IDgrupoB;
        codigos << codigoB << codigoA;

        query.prepare("UPDATE alojado_en SET codHab=:codigo WHERE IDgrupo=:grupo");
        query.bindValue(":codigo",codigos);
        query.bindValue(":grupo",grupos);

        if(!query.execBatch()) {
            qDebug() << "ERROR en MyHabitacionModels->intercambiarHabitacionesGrupos: " << query.lastError().text();
            _db->database().rollback();
            return;
        }

        _db->database().commit();

        _modeloAlojamientos->select();
        emit reloadAlojamientos();

    }
}

bool MyHabitacionModels::isOpen(Habitacion *hab)
{
    if(!_modeloHabitacion)
        return false;

    return _modeloHabitacion->estaAbierta(hab->codigo);
}

void MyHabitacionModels::setLlaveHabitacion(Habitacion *hab)
{
    if(_modeloHabitacion) {
        _modeloHabitacion->setLlaveHabitacion(hab->codigo,hab->llaveEntregada);
    }
}

QVariant MyHabitacionModels::grupoAlojadoEn(QString codHabitacion)
{
    if(_modeloAlojamientos) {
        _modeloAlojamientos->select();
        int numRows=_modeloAlojamientos->rowCount();
        for(int i=0;i<numRows;i++) {
            if(_modeloAlojamientos->index(i,1).data(Qt::EditRole).toString()==codHabitacion)
                return _modeloAlojamientos->index(i,0).data(Qt::EditRole);
        }
    }

    return QVariant();
}

QVariant MyHabitacionModels::habitacionDelGrupo(QString idGrupo)
{
    if(_modeloAlojamientos) {
        _modeloAlojamientos->select();
        int numRows=_modeloAlojamientos->rowCount();
        for(int i=0;i<numRows;i++) {
            if(_modeloAlojamientos->index(i,0).data(Qt::EditRole).toString()==idGrupo)
                return _modeloAlojamientos->index(i,1).data(Qt::EditRole);
        }
    }

    return QVariant();
}
