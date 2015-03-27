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

#ifndef ACOLITODB_H
#define ACOLITODB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <QDebug>

#include "datosconexion.h"

/*!
  Clase base para todas las bases de datos empleadas
  */

class AcolitoDB : public QObject
{
    Q_OBJECT
public:

    explicit AcolitoDB(QObject *parent = 0,DatosConexion *datos=0);

    DatosConexion getDatosConexion() const;

    /*!
      Devuelve el ultimo error producido
      */

    QPair<int,QString> lastError() const;

    /*!
      Conecta con un servidor de base de datos
      @param datos  Datos de la conexion
      @return bool
      -@b TRUE: Se ha podido conectar
      -@b FALSE: No se pudo conectar
      @note Modifica el objeto
      */

    virtual bool connectToServer(DatosConexion datos)=0;

    /*!
      Indica si el objeto esta o no conectado
      @return bool
      -@b TRUE: Esta conectado
      -@b FALSE: No esta conectado
      @note No modifica el objeto
      */

    virtual bool isConnected() const=0;

    /*!
      Comprueba si existe la base de datos solicitada
      @brief
      Es distinto conectar al servidor que abrir la base de datos
      @return bool
      - @b TRUE: Existe la base de datos
      - @b FALSE: No existe la base de datos
      @note Modifica el objeto
      */

    virtual bool existeDatabase()=0;

    /*!
      Crea la base de datos
      */

    virtual bool createDatabase(QString pathScript)=0;

    /*!
      Abre y conecta con la base de datos
      */

    virtual bool openDatabase()=0;

    /*!
      Desconecta el objeto
      @note Modifica el objeto
      */

    virtual void disconnect()=0;
    
signals:
    
public slots:

    /*!
      Establece nuevos datos para la conexion
      @param datos  Nuevos datos de conexion
      @note Modifica el objeto
      */

    void setDatosConexion(DatosConexion datos);

    /*!
      @overload
      */
    void setDatosConexion(DatosConexion *datos);

private:

    DatosConexion *data; ///< Datos de la conexion
    QPair<int,QString> _error;  ///< Ultimo error producido

protected:

    static QString _queryExists;

    /*!
      Establece el ultimo codigo de error producido
      */

    void setLastError(QPair<int,QString> codigo);    
};

#endif // ACOLITODB_H
