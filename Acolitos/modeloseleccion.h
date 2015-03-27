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

#ifndef MODELOSELECCION_H
#define MODELOSELECCION_H

#include <QSortFilterProxyModel>
#include <QList>
#include <QVariant>
#include <QVariantList>

class ModeloSeleccion : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int colSeleccion READ getColumnaSeleccionable() WRITE setColumnaSeleccionable())
    Q_PROPERTY(int colIdentificador READ getColumnaIdentificador() WRITE setColumnaIdentificador())

public:
    explicit ModeloSeleccion(QObject *parent = 0);

    /*!
      Establece la columna que es seleccionable
      */

    void setColumnaSeleccionable(int columna);

    /*!
      Devuelve la columna que es seleccionable
      */

    int getColumnaSeleccionable() const;

    /*!
      Establece la columna que contiene los datos que queremos usar como indicador
      */

    void setColumnaIdentificador(int columna);

    /*!
      Devuelve la columna identificadora
      */

    int getColumnaIdentificador() const;

    /*!
      Devuelve los identificadores seleccionados
      */

    QVariantList elementosMarcados() const;

    /*!
      Activa el modo de seleccion exclusivo
      */

    void setSeleccionExclusiva(bool modo);

    /**********************************************/
    // Modificaciones sobre data y setData
    /**********************************************/

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    
signals:
    
public slots:

    /*!
      Marca un elemento (fila)
      */

    void marcar(int fila,bool estado);

private:

    int _columnaSeleccion;      ///< Columna que permite seleccion
    int _columnaIdentificador;  ///< Columna de identificadores seleccionables
    bool _seleccionExclusiva;    ///< Indica el modo de seleccion, multiple o exclusivo

    QVariantList _listaSeleccion;    ///< Lista de datos seleccionados
};

#endif // MODELOSELECCION_H
