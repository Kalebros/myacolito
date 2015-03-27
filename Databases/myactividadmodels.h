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

#ifndef MYACTIVIDADMODELS_H
#define MYACTIVIDADMODELS_H

#include "myacolitomodels.h"

#include "mymodeloactividades.h"
#include "mymodelotipoactividad.h"
#include "mymodelolocalizaciones.h"

class MyActividadModels : public MyAcolitoModels
{
    Q_OBJECT
public:
    explicit MyActividadModels(QObject *parent = 0,MyAcolitoDB *db=0);

    virtual bool isActive() const;

    /*!
      Devuelve el modelo de actividades
      */

    QSortFilterProxyModel *getModeloActividades();

    /*!
      Devuelve el modelo de localizaciones
      */

    QSortFilterProxyModel *getModeloLocalizaciones();

    /*!
      Devuelve el modelo de tipos de actividades
      */

    QSortFilterProxyModel *getModeloTiposActividad();

    /*!
      Activa el modo 'todas las jornadas' para una actividad
      */

    void activarTodasLasJornadas(int idActividad,bool modo);

    /*!
      Devuelve el modo 'todas las jornadas' para una actividad
      */

    bool getModoTodasLasJornadas(int idActividad);

    /*!
      Añade un nuevo tipo de actividades
      */

    void addTipoActividad(QString tipo);

    /*!
      Añade una actividad al sistema
      */

    void addActividad(QVariantMap actividad);

    /*!
      Elimina una actividad
      */

    void delActividad(int id);

    /*!
      @overload
      */

    void delActividad(QVariant id);

    /*!
      Añade una lista de actividades
      */

    void addListaActividades(QList<QVariantMap> listaActividades);

    /*!
      Borra un tipo de actividades
      */

    void delTipoActividad(QString tipo,QString sustituto);

    /*!
      Añade una nueva localizacion
      */

    void addLocalizacion(QString lugar);

    /*!
      Borra una localizacion
      */

    void delLocalizacion(QString lugar,QString sustituto);

    /*!
      Devuelve un organizador de una actividad
      */

    QString getOrganizador(int idActividad);
    
signals:
    
public slots:

    virtual void deleteModelos();
    virtual void loadModelos();

private slots:

private:

    MyModeloActividades* _modeloActividades;    ///< Modelo de actividades
    MyModeloTipoActividad *_modeloTipos;               ///< Modelo de tipos de actividades
    MyModeloLocalizaciones *_modeloLugares;             ///< Modelo de lugares para actividades
};

#endif // MYACTIVIDADMODELS_H
