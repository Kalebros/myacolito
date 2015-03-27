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

#include "acolitodb.h"

QString AcolitoDB::_queryExists=QString("SHOW DATABASES LIKE :database ");

AcolitoDB::AcolitoDB(QObject *parent, DatosConexion *datos) :
    QObject(parent)
{
    this->data=datos;
}

void AcolitoDB::setDatosConexion(DatosConexion datos)
{
    if(data)
        delete data;
    data=new DatosConexion(datos);
}

void AcolitoDB::setDatosConexion(DatosConexion *datos)
{
    if(data)
        delete data;
    data=datos;
}

DatosConexion AcolitoDB::getDatosConexion() const
{
    return *data;
}

void AcolitoDB::setLastError(QPair<int, QString> codigo)
{
    _error=codigo;
}

QPair<int,QString> AcolitoDB::lastError() const
{
    return _error;
}
