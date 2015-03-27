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

#ifndef MYACOLITODB_H
#define MYACOLITODB_H

#include "acolitodb.h"
#include "sqlscript.h"

class MyAcolitoDB : public AcolitoDB
{
    Q_OBJECT
public:
    explicit MyAcolitoDB(QObject *parent = 0,DatosConexion *datos=0,QString cName=QString());

    virtual bool connectToServer(DatosConexion datos);
    virtual bool isConnected() const;
    virtual void disconnect();
    virtual bool existeDatabase();
    virtual bool createDatabase(QString pathScript);
    virtual bool openDatabase();

    QString getNombreConexion() const
    { return _cName; }

    /*!
      Cambia el nombre de la conexion
      @param name   Nuevo nombre de la conexion
      @note Modifica el objeto
      @note Utilizar unicamente cuando la conexion
      no tiene asociados ningun tipo de modelo o query
      */
    void setNombreConexion(QString name);

    QSqlDatabase database()
    { return _db; }
    
signals:
    
    void aboutToDisconnect();

public slots:

    void solicitarDesconexion();

private:

    QSqlDatabase _db;   ///< Conexion de base de datos
    QString _cName;     ///< Nombre de la conexion
};

#endif // MYACOLITODB_H
