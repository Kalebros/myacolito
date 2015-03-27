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

#ifndef MYMODELOHABITACIONES_H
#define MYMODELOHABITACIONES_H

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

/*!
  Clase base de habitaciones
  */

class Habitacion : public QObject
{
    Q_OBJECT

public:

    explicit Habitacion(QObject *parent=0) :
        QObject(parent),
        codigo(QString()),
        tipo(QString()),
        comentario(QString()),
        llaveEntregada(false),
        estaReservada(false),
        conceptoReserva(QString())
    {}

public:

    QString codigo; ///< Codigo identificador de la habitacion
    QString tipo;   ///< Tipo de la habitacion
    QString comentario; ///< Comentario de la habitacion
    bool llaveEntregada;    ///< Marcador de llave
    bool estaReservada; ///< Marcador de reserva
    QString conceptoReserva;    ///< Concepto de reserva
};

class MyModeloHabitaciones : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyModeloHabitaciones(QObject *parent = 0,const QSqlDatabase db=QSqlDatabase());

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool estaAbierta(QString codHab);
    void setLlaveHabitacion(QString codHab,bool estado);

signals:

    void cambiosCodigoHabitaciones();
    
public slots:

    /*!
      Añade una nueva habitacion al sistema
      @param habitacion Datos de la nueva habitacion
      @note Modifica el objeto y la base de datos que hay debajo
      @pre La habitacion ha de incluir un codigo univoco
      @pre La habitacion ha de incluir un tipo existente en el sistema
      */

    void addNuevaHabitacion(Habitacion *habitacion);

    /*!
      Borra una habitacion del sistema
      @param habitacion Datos de la habitacion a eliminar
      @note Modifica el objeto y la base de datos que hay debajo
      @pre La habitacion ha de incluir un codigo univoco
      */

    void deleteHabitacion(Habitacion *habitacion);

    /*!
      @overload
      */

    void deleteHabitacion(QString codHab);

    /*!
      Actualiza los tipos de habitacion
      */

    void updateTiposHabitacion();

    /*!
      Desaloja una habitacion
      */

    void desalojarHabitacion(QString codHab);

    /*!
      Actualiza el recuento de habitaciones libres
      */

    void updateHabitacionesLibres();

private:

    QSqlTableModel *_modeloHabitaciones;
    QSqlTableModel *_modeloAlojados;

    QIcon iconoOk;
    QIcon iconoError;

    /*!
      Devuelve el indice dentro del modelo de habitaciones de una habitacion
      */

    QModelIndex getModelIndexOf(QString codHab);
    
};

#endif // MYMODELOHABITACIONES_H
