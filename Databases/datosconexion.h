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

#ifndef DATOSCONEXION_H
#define DATOSCONEXION_H

#include <QString>
#include <QPair>

/*!
  Contiene los datos basicos de una conexion a base de datos
  @brief
  Gracias a esta clase, podemos guardar una configuracion de conexion
  de base de datos de forma sencilla. Utilizado por AcolitoDB y sus
  herederos
  */

class DatosConexion
{
public:
    DatosConexion();
    DatosConexion(const DatosConexion &d);
    DatosConexion(QString host,int port,QString database,QString connection);

    ~DatosConexion();

    void setLogin(QPair<QString,QString> login);
    void setPort(int port);
    void setHostName(QString host);
    void setDatabaseName(QString database);
    void setConnectionName(QString connection);
    void setNombre(QString nombre);

    QPair<QString,QString> getLogin() const;
    int getPort() const;
    QString getHostName() const;
    QString getDatabaseName() const;
    QString getConnectionName() const;

private:

    int connectionPort; ///< Puerto de conexion
    QPair<QString,QString> login;   ///< Login de la conexion
    QString hostName;   ///< Direccion del host
    QString databaseName; ///< Nombre de la base de datos o esquema
    QString connectionName; ///< Nombre de la conexion
};

#endif // DATOSCONEXION_H
