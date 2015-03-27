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

#ifndef MYEVENTODB_H
#define MYEVENTODB_H

#include <QSortFilterProxyModel>

#include "myacolitodb.h"
#include "acolitodb.h"
#include "sqlscript.h"
#include "mymodeloeventos.h"

/*!
  Acceso a la base de datos de Eventos en MySQL
  */

class MyEventoDB : public QObject
{
    Q_OBJECT
public:

    /*!
      Devuelve la instancia unica de MyEventoDB.
      @brief
      En caso de no existir, crea una nueva instancia
      @param parent Padre del objeto
      @param datos  Datos de configuracion
      @note Modifica el objeto
      @note Si ya existia instancia, los datos de configuracion
      se obvian.
      */

    static MyEventoDB *getInstance(QObject *parent=0,DatosConexion *datos=0);

    bool connectToServer(DatosConexion datos);
    bool isConnected() const;
    void disconnect();
    bool existeDatabase();
    bool createDatabase();
    bool openDatabase();

    QPair<int,QString> lastError() const;

    /*!
      Devuelve un modelo sobre los eventos de la base de datos
      */

    QSortFilterProxyModel *getModeloEventos();

    /*!
      Devuelve los datos de un evento
      @param ev Nombre del evento
      @return Datos del evento
      @note No modifica el objeto
      @pre La base de datos tiene que estar abierta
      @note Las modificaciones del valor devuelto no
      se aplican a la base de datos.
      */

    Evento *getEvento(QString ev);

    /*!
      Devuelve los datos de la conexion actual
      */
    DatosConexion getDatosConexion() const;

    /*!
      Devuelve la conexion utilizada con el servidor
      */

    MyAcolitoDB *getConexion();

signals:

    void nuevoEvento(Evento *ev,QStandardItemModel *categorias);
    
public slots:

    void reloadModeloEventos();

    void addEvento(Evento ev,QStandardItemModel *categorias);

private:

    /*!
      Constructor de la clase
      @note Es privado para utilizarse el patron Singleton
      */
    explicit MyEventoDB(QObject *parent = 0,DatosConexion *datos=0);

    static MyEventoDB *singleInstance; ///< Instancia unica de la base de datos
    static QString _queryGetEvento;     ///< Query para obtener los datos de un evento
    static QString _queryAddEvento;     ///< Query para incluir un nuevo evento en la lista
    static QString _queryAddFecha;      ///< Query para incluir una nueva fecha en un evento

    MyAcolitoDB *_database; ///< Base de datos

    MyModeloEventos *_modeloBase;    ///< Modelo base de eventos;

};

#endif // MYEVENTODB_H
