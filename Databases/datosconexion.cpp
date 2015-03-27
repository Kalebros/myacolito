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

#include "datosconexion.h"

DatosConexion::DatosConexion()
    : connectionPort(-1),
      hostName(QString()),
      databaseName(QString()),
      connectionName(QString())

{
    login=QPair<QString,QString>();
}

DatosConexion::DatosConexion(const DatosConexion &d)
    : connectionPort(d.connectionPort),
      hostName(d.hostName),
      databaseName(d.databaseName),
      connectionName(d.connectionName)
{
    login=QPair<QString,QString>(d.login.first,d.login.second);
}

DatosConexion::DatosConexion(QString host, int port, QString database, QString connection)
    : connectionPort(port),
      hostName(host),
      databaseName(database),
      connectionName(connection)
{
    login=QPair<QString,QString>();
}

DatosConexion::~DatosConexion()
{
    connectionPort=-1;
    hostName.clear();
    databaseName.clear();
    connectionName.clear();
    login.first.clear();
    login.second.clear();
}

void DatosConexion::setLogin(QPair<QString,QString> login)
{
    this->login.first=login.first;
    this->login.second=login.second;
}

void DatosConexion::setPort(int port)
{
    connectionPort=port;
}

void DatosConexion::setHostName(QString host)
{
    hostName=host;
}

void DatosConexion::setDatabaseName(QString database)
{
    databaseName=database;
}

void DatosConexion::setConnectionName(QString connection)
{
    connectionName=connection;
}

QPair<QString,QString> DatosConexion::getLogin() const
{
    return login;
}

int DatosConexion::getPort() const
{
    return connectionPort;
}

QString DatosConexion::getHostName() const
{
    return hostName;
}

QString DatosConexion::getDatabaseName() const
{
    return databaseName;
}

QString DatosConexion::getConnectionName() const
{
    return connectionName;
}
