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

#ifndef MYMODELOACTIVIDADES_H
#define MYMODELOACTIVIDADES_H

#include <QAbstractTableModel>

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSortFilterProxyModel>
#include <QSqlDatabase>
#include <QString>
#include <QIcon>

#include <QDebug>
#include <QSqlError>

#include <QVariant>
#include <QVariantMap>


class MyModeloActividades : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyModeloActividades(QObject *parent = 0,const QSqlDatabase db=QSqlDatabase());

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /*!
      Añade una nueva actividad a la base de datos
      @param datosActividad Datos de la actividad
      @note Modifica el objeto
      @note Modifica la base de datos
      @note Se pueden introducir actividades con el mismo nombre. Vigilar.
      */

    void addActividad(QVariantMap datosActividad);

    /*!
      Añade una lista de actividades a la base de datos
      @param listaActividades   Lista de datos de las actividades
      @note Modifica el objeto
      @note Modifica la base de datos
      @note Se pueden introducir actividades con el mismo nombre. Vigilar
      */

    void addListaActividad(QList<QVariantMap> listaActiviades);

    /*!
      Elimina una actividad de la base de datos
      @param idActividad    Identificador univoco de la actividad
      @note Modifica el objeto
      @note Modifica la base de datos
      @note No es reversible
      */

    void delActividad(int idActividad);

    /*!
      @overload
      */

    void delActividad(QVariant idActividad);

    /*!
      Activia el modo 'todas las jornadas' para una actividad
      @param idActividad    Identificador univoco de la actividad
      @note Modifica el objet
      @note Modifica la base de datos
      */

    void activarTodasLasJornadas(int idActividad, bool modo);

    /*!
      @overload
      */

    void activarTodasLasJornadas(QVariant idActividad,bool modo);

    /*!
      Devuelve el modo 'todas las jornadas' de una actividad
      */

    bool getTodasLasJornadas(int idActividad);

    /*!
      Cambia todas las actividades de un tipo a otro tipo
      */

    void cambiarTipoActividades(QString tipoOrigen,QString tipoDestino);

    /*!
      Cambia todas las actividades de un lugar a otro lugar
      */

    void cambiarLocalizaciones(QString lugarOrigen,QString lugarDestino);

    /*!
      Devuelve el organizador de una actividad
      */

    QString getOrganizador(int idActividad);

signals:
    
public slots:

    void reloadTipos();
    void reloadLugares();

private:

    QSqlTableModel *_tablaActividades;   ///< Tabla de actividades
    
    QIcon iconoOk;
    QIcon iconoError;
};

#endif // MYMODELOACTIVIDADES_H
