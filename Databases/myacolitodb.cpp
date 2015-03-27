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

#include "myacolitodb.h"

MyAcolitoDB::MyAcolitoDB(QObject *parent, DatosConexion *datos, QString cName) :
    AcolitoDB(parent,datos),
    _cName(cName)
{
}

bool MyAcolitoDB::isConnected() const
{
    return (_db.isOpen() && _db.isValid());
}

void MyAcolitoDB::disconnect()
{
    if(_db.isOpen())
            _db.close();
    QSqlDatabase::removeDatabase(_cName);
    _db=QSqlDatabase();
}

bool MyAcolitoDB::connectToServer(DatosConexion datos)
{
    if(isConnected())
        disconnect();

    _db=QSqlDatabase::addDatabase("QMYSQL",_cName);

    _db.setHostName(datos.getHostName());
    _db.setPort(datos.getPort());
    _db.setUserName(datos.getLogin().first);
    _db.setPassword(datos.getLogin().second);

    if(!_db.open()) {
        setLastError(QPair<int,QString>(0,tr("No se pudo abrir la conexion:\n")+_db.lastError().text()));
        _db=QSqlDatabase();
        return false;
    }

    setDatosConexion(datos);
    return true;
}

bool MyAcolitoDB::existeDatabase()
{
    if(!isConnected())
        return false;
    {
        QSqlQuery query(_db);
        query.prepare(AcolitoDB::_queryExists);
        query.bindValue(":database",getDatosConexion().getDatabaseName());

        if(!query.exec()) {
            QString error=query.lastError().text();
            QPair<int,QString> mensaje(1,error);
            setLastError(mensaje);
            return false;
        }

        query.next();
        if(query.size()>1)
            return true;
        else return false;
    }
    return false;
}

bool MyAcolitoDB::createDatabase(QString pathScript)
{
    SQLScript reader(pathScript);

    QStringList script=reader.getScriptProcesado();

    {
        QSqlQuery query(_db);

        _db.transaction();
        foreach(QString sentencia,script) {
            query.prepare(sentencia);
            if(!query.exec()) {
                QPair<int,QString> error(2,tr("Se produjo el siguiente error durante la creacion de una base de datos:\n")+
                                         query.lastError().text());
                setLastError(error);
                _db.rollback();
                return false;
            }
        }
        _db.commit();
    }
    _db.close();
    _db.setDatabaseName(getDatosConexion().getDatabaseName());
    if(!_db.open()) {
        QPair<int,QString> error(3,tr("Se produjo el siguiente error al abrir la base de datos:\n")+_db.lastError().text());
        setLastError(error);
        return false;
    }
    return true;
}

bool MyAcolitoDB::openDatabase()
{
    if(_db.isValid()) {
        if(_db.isOpen() && _db.databaseName()==getDatosConexion().getDatabaseName())
            return true;
        else if(_db.isOpen()) { //La base de datos tiene otro nombre
            _db.close();
            _db.setDatabaseName(getDatosConexion().getDatabaseName());
            _db.open();
        }
        else {
            _db.setDatabaseName(getDatosConexion().getDatabaseName());
            _db.open();
        }
        return true;
    }
    return false;
}

void MyAcolitoDB::solicitarDesconexion()
{
    emit aboutToDisconnect();
}

void MyAcolitoDB::setNombreConexion(QString name)
{
    if(QSqlDatabase::contains(_cName)) {
        QSqlDatabase::cloneDatabase(_db,name);
    }
    _cName=name;
}
