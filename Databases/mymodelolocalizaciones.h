/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ram�rez Marti
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

#ifndef MYMODELOLOCALIZACIONES_H
#define MYMODELOLOCALIZACIONES_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QModelIndexList>
#include <QVariant>
#include <QVariantMap>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#include <QSqlDatabase>
#include <QSqlTableModel>

class MyModeloLocalizaciones : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyModeloLocalizaciones(QObject *parent = 0,const QSqlDatabase db=QSqlDatabase());

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /*!
      A�ade una nueva localizacion
      */

    void addLocalizacion(QString lugar);

    /*!
      Elimina un tipo de actividad
      */

    void delLocalizacion(QString lugar);

signals:

    void nuevaLocalizacion();
    void borradaLocalizacion();
    void editadaLocalizacion();

public slots:

private:

    QSqlTableModel *_tablaLugar;   ///< Tabla de localizaciones
};

#endif // MYMODELOLOCALIZACIONES_H
