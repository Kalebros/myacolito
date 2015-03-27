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

#ifndef MYHABITACIONMODELS_H
#define MYHABITACIONMODELS_H

#include "myacolitomodels.h"
#include "mymodelohabitaciones.h"
#include "mymodelotiposhab.h"

class MyHabitacionModels : public MyAcolitoModels
{
    Q_OBJECT
public:
    explicit MyHabitacionModels(QObject *parent = 0,MyAcolitoDB *db=0);
    
    virtual bool isActive() const;

    /*!
      Devuelve el modelo base de habitaciones
      */

    QSortFilterProxyModel *getModeloHabitaciones();

    /*!
      Devuelve el modelo base de alojados
      */

    QSortFilterProxyModel *getModeloAlojamientos();

    /*!
      Devuelve el modelo base de tipos de habitaciones
      */

    QSortFilterProxyModel *getModeloTipoHabitaciones();

signals:

    void reloadAlojamientos();

    void cambiosTiposHabitaciones();
    
public slots:
    
    virtual void deleteModelos();
    virtual void loadModelos();

    void reloadModeloAlojamientos();

    /*!
      Añade una nueva habitacion
      */

    void nuevaHabitacion(Habitacion *habitacion);

    /*!
      Nuevo tipo de habitacion
      */

    void nuevoTipoHabitacion(QVariantMap datos);

    /*!
      Borra un tipo de habitacion
      */

    void borraTipoHabitacion(QString tipo,QString sustituto);

    /*!
      Añade una lista de habitaciones
      */

    void nuevaListaHabitaciones(QList<Habitacion*> listadoHabitaciones);

    /*!
      Borra una habitacion
      */

    void borraHabitacion(Habitacion *habitacion);

    /*!
      Borra una lista de habitaciones
      */

    void borraListaHabitaciones(QList<Habitacion*> listadoHabitaciones);

    /*!
      Aloja a un grupo en una habitacion
      */

    void alojarGrupoHabitacion(int IDgrupo,QString codHab);

    /*!
      Intercambia dos grupos de habitacion
      */

    void intercambiarHabitacionesGrupos(int IDgrupoA,int IDgrupoB);

    /*!
      Devuelve si una habitacion esta abierta o no
      */

    bool isOpen(Habitacion *hab);

    /*!
      Cambia el estado de una habitacion
      */

    void setLlaveHabitacion(Habitacion *hab);

    /*!
      Devuelve el grupo alojado en una habitacion
      */

    QVariant grupoAlojadoEn(QString codHabitacion);

    /*!
      Devuelve la habitacion en la que se aloja un grupo
      */

    QVariant habitacionDelGrupo(QString idGrupo);

    /*!
      Desaloja una habitacion
      */

    void desalojarHabitacion(QString codHabitacion);

private:

    MyModeloHabitaciones *_modeloHabitacion;
    MyModeloTiposHab *_modeloTipoHab;         ///< Tabla de tipos de habitacion
    QSqlTableModel *_modeloAlojamientos;    ///< Tabla de personas alojadas por habitacion

private slots:

};

#endif // MYHABITACIONMODELS_H
