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

#ifndef CONEXIONESSQL_H
#define CONEXIONESSQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QVariantList>
#include <QSqlTableModel>
#include <QModelIndex>

#include <QMetaObject>
#include <QFile>
#include <QList>

#include <QSortFilterProxyModel>
#include <QDebug>

#include <Databases/datosconexion.h>

/*!
  Modelo especial para trabajar con las conexiones
  */

class ModeloConexiones : public QSqlTableModel
{
    Q_OBJECT

public:

    explicit ModeloConexiones(QObject *parent=0,QSqlDatabase db=QSqlDatabase())
        : QSqlTableModel(parent,db)
    {}

    bool setData(const QModelIndex &index, const QVariant &value, int role);
};


/*!
  Clase para poder almacenar y recuperar datos de conexiones SQL
  @brief
  Funciona sobre un archivo SQLite; esta clase es más comoda para
  trabajar con multiples conexiones almacenadas que el sistema de
  settings.
  */

class ConexionesSQL : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ isReady)

public:

    /*!
      Constructor
      @param parent Padre del objeto
      @param path   Path al archivo SQLite donde se almacenan las conexiones
      @note Modifica el objeto
      */
    explicit ConexionesSQL(QObject *parent = 0,QString path=QString());
    ~ConexionesSQL();

    /*!
      Indica si el objeto esta preparado para trabajar con el
      @return bool
      - @b TRUE: El objeto esta listo
      - @b FALSE: El objeto no esta preparado o se ha producido un error
      @note No modifica el objeto
      */
    bool isReady() const;

    /*!
      Indica si el almacen esta vacio o no
      @return bool
      - @b TRUE: El almacen esta vacio
      - @b FALSE: El almacen no esta vacio
      @pre El objeto esta listo
      @note Modifica el objeto
      */

    bool isEmpty();

    /*!
      Devuelve un modelo ordenable con los datos de la
      conexion
      @return QSortFilterProxyModel, o 0 en caso de error.
      @pre El objeto esta listo
      @note No modifica el objeto
      */
    QSortFilterProxyModel *modeloConexiones();

    /*!
      Devuelve la conexion predeterminada del almacen
      @return QString, nombre de la conexion predeterminada, o vacio si no esta listo el objeto
      @pre El objeto esta listo
      @note No modifica el objeto
      */
    QString conexionPredeterminada() const;

    /*!
      Devuelve los datos de una conexion en formato DatosConexion
      @param nombre Nombre de la conexion
      @return Datos de la conexion o un objeto vacio
      @pre El objeto esta listo
      @note No modifica el objeto
      */

    DatosConexion getConexion(QString nombre);

public slots:

    /*!
      Introduce una nueva conexion de datos en el almacen
      @param datos  Datos de la conexion
      @param driver Driver Qt utilizado por la conexion
      @param isPredeter Indica si es una conexion predeterminada
      @pre El objeto esta listo
      @note Modifica el objeto
      */
    void addConexion(DatosConexion datos,QString driver,bool isPredeter);

    /*!
      @overload
      */
    void addConexion(QVariantList datosConexion);

    /*!
      Elimina una conexion del almacen
      @param nConexion  Nombre de la conexion
      @pre El objeto esta listo
      @note Modifica el objeto
      */
    void removeConexion(QString nConexion);

    /*!
      Establece una conexion predeterminada
      @param nConexion  Nombre de la conexion predeterminada
      @pre El objeto esta listo
      @note Modifica el objeto
      */
    void setConexionPredeterminada(QString nConexion);
    
signals:

    /*!
      Indica que se han producido cambios dentro del storage
      */
    void changesInDatabase();
    
public slots:

private:

    static QString _script; ///< Script de construccion para el archivo SQLite
    static QString _insert; ///< Script de insercion de datos
    static QString _remove; ///< Script de borrado de datos
    static QString _getDatos;   ///< Obtiene los datos de una conexion
    static QString _predeter;   ///< Conexion predeterminada
    static QString _predeterReset;  ///< Reseteo de predeterminados
    static QString _numConexiones;        ///< Cuenta de conexiones
    static int _contador;   ///< Contador de conexiones, para asegurar conexiones individuales

    QString _pathArchivo;   ///< Path al archivo SQLite que almacena las conexiones
    QString _nombreCon;     ///< Nombre de la conexion del archivo
    QSqlDatabase _db;       ///< Conexion a la base de datos
    bool _ready;            ///< Flag de estado

    ModeloConexiones *_modeloBase; ///< Modelo base de datos

};

#endif // CONEXIONESSQL_H
