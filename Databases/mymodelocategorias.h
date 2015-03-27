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

#ifndef MYMODELOCATEGORIAS_H
#define MYMODELOCATEGORIAS_H

#include <QAbstractTableModel>

/*!
  Modelo de categorias
  */

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QString>
#include <QSqlDatabase>
#include <QIcon>
#include <QList>
#include <QMap>
#include <QDate>
#include <QVariant>
#include <QVariantMap>

#include <QDebug>
#include <QSqlError>

class MyModeloCategorias : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyModeloCategorias(QObject *parent = 0,const QSqlDatabase db=QSqlDatabase());

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    
signals:
    
    void cambiosCategoria();

public slots:

    void actualizaNumeroCategoria(const QString categoria);

    void creaNuevaCategoria(QVariantMap nuevaCategoria);

    void borraCategoria(QString categoria,QString catSubstituto);

private:

    QSqlTableModel *_tablaCategorias;   ///< Tabla de categorias
    QMap<QString,int> _mapaPlazas;      ///< Mapa de plazas
    
    /*!
      Devuelve la posicion dentro del modelo de un registro por su nombre de categoria
      */

    int categoriaRow(QString nombre);

    /*!
      Recupera el numero de plazas de cada categoria
      */

    void updateNumPlazas();
};

#endif // MYMODELOCATEGORIAS_H
