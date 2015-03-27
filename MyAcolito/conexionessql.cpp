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

#include "conexionessql.h"

QString ConexionesSQL::_script=QString("CREATE TABLE Conexiones ( "
                                       "idConexion INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "nombre TEXT, "
                                       "driver TEXT, "
                                       "host TEXT, "
                                       "databaseName TEXT, "
                                       "port INTEGER,"
                                       "predeterminado INTEGER DEFAULT 0"
                                       ")");

QString ConexionesSQL::_insert=QString("INSERT INTO Conexiones"
                                       "(nombre,driver,host,databaseName,port) "
                                       "VALUES(:nombre,:driver,:host,:database,:port)");

QString ConexionesSQL::_getDatos("SELECT nombre,host,port,databaseName "
                                 "FROM Conexiones "
                                 "WHERE nombre=:nombre");

QString ConexionesSQL::_remove=QString("DELETE FROM Conexiones "
                                       "WHERE nombre=:nombre");

QString ConexionesSQL::_predeter=QString("UPDATE Conexiones "
                                         "SET predeterminado=1 WHERE nombre=:name"
                                         );

QString ConexionesSQL::_predeterReset=QString("UPDATE Conexiones "
                                              "SET predeterminado=0 WHERE predeterminado=1");

QString ConexionesSQL::_numConexiones=QString("SELECT count(nombre) FROM Conexiones");
int ConexionesSQL::_contador=0;

ConexionesSQL::ConexionesSQL(QObject *parent, QString path) :
    QObject(parent)
{
    _pathArchivo=path;

    _nombreCon=metaObject()->className();
    _nombreCon.append(QString::number(_contador++));

    //Comprobar si existe el archivo
    if(path.isEmpty() || path.isNull()) {
        _ready=false;
        return;
    }

    QFile archivo(path);

    if(!archivo.exists())
    {
        _db=QSqlDatabase::addDatabase("QSQLITE",_nombreCon);
        _db.setDatabaseName(path);

        if(!_db.open()) {
            _ready=false;
            QSqlDatabase::removeDatabase(_nombreCon);
            return;
        }

        //Construccion de la base de datos
        QSqlQuery query(_db);
        query.prepare(ConexionesSQL::_script);

        if(!query.exec()) {
            _ready=false;
            _db.close();
            QSqlDatabase::removeDatabase(_nombreCon);
            return;
        }
    }
    else
    {
        _db=QSqlDatabase::addDatabase("QSQLITE",_nombreCon);
        _db.setDatabaseName(path);
        if(!_db.open()) {
            _ready=false;
            QSqlDatabase::removeDatabase(_nombreCon);
            return;
        }
    }

    _db.close();
    _modeloBase=0;
    _ready=true;
}

bool ConexionesSQL::isReady() const
{
    return _ready;
}

QSortFilterProxyModel *ConexionesSQL::modeloConexiones()
{
    _db.open();
    if(!isReady())
        return 0;

    if(!_modeloBase) {
        _modeloBase=new ModeloConexiones(this,_db);
        _modeloBase->setTable("Conexiones");
        _modeloBase->select();
    }

    QSortFilterProxyModel *res=new QSortFilterProxyModel(this);
    res->setSourceModel(_modeloBase);

    return res;
}

void ConexionesSQL::addConexion(QVariantList datosConexion)
{
    if(!isReady())
    { return; }

    {
        if(!_db.isOpen())
            _db.open();

        _db.transaction();
        QSqlQuery query(_db);

        query.prepare(ConexionesSQL::_insert);
        query.bindValue(":nombre",datosConexion.value(0).toString());
        query.bindValue(":driver",datosConexion.value(1).toString());
        query.bindValue(":host",datosConexion.value(2).toString());
        query.bindValue(":database",datosConexion.value(3).toString());
        query.bindValue(":port",datosConexion.value(4).toInt());

        if(!query.exec()) {
            _db.rollback();
            return;
        }
        else _db.commit();

        if(datosConexion.value(5).toBool())
            setConexionPredeterminada(datosConexion.value(0).toString());
    }

    if(_modeloBase)
        _modeloBase->select();

    emit changesInDatabase();

    _db.close();
}

void ConexionesSQL::addConexion(DatosConexion datos, QString driver, bool isPredeter)
{
    if(!isReady())
    { return; }

    {
        if(!_db.isOpen())
            _db.open();

        _db.transaction();

        QSqlQuery query(_db);

        query.prepare(ConexionesSQL::_insert);
        query.bindValue(":nombre",datos.getConnectionName());
        query.bindValue(":driver",driver);
        query.bindValue(":host",datos.getHostName());
        query.bindValue(":database",datos.getDatabaseName());
        query.bindValue(":port",datos.getPort());

        if(!query.exec()) {
            _db.rollback();
            return;
        }
        else _db.commit();

        if(isPredeter)
            setConexionPredeterminada(datos.getConnectionName());
    }

    if(_modeloBase)
        _modeloBase->select();

    emit changesInDatabase();

    _db.close();
}

void ConexionesSQL::removeConexion(QString nConexion)
{
    if(!isReady())
    { return; }

    {
        if(!_db.isOpen())
            _db.open();

        QSqlQuery query(_db);

        query.prepare(ConexionesSQL::_remove);
        query.bindValue(":nombre",nConexion);

        query.exec();
    }

    if(_modeloBase)
        _modeloBase->select();

    emit changesInDatabase();
    _db.close();
}

ConexionesSQL::~ConexionesSQL()
{
    if(_modeloBase) {
        _modeloBase->submitAll();
        delete _modeloBase;
    }

    QSqlDatabase::database(_nombreCon).close();
    QSqlDatabase::removeDatabase(_nombreCon);
}

QString ConexionesSQL::conexionPredeterminada() const
{
    QString res;
    if(!isReady() || !_modeloBase)
        return res;

    int rows=_modeloBase->rowCount();
    bool found=false;
    int predeter=_modeloBase->record().indexOf("predeterminado");
    for(int i=0;i<rows && !found;i++) {
        QModelIndex idx=_modeloBase->index(i,predeter);
        if(_modeloBase->data(idx).toInt()==1) {
            found=true;
            res=_modeloBase->data(_modeloBase->index(i,1)).toString();
        }
    }

    return res;
}

void ConexionesSQL::setConexionPredeterminada(QString nConexion)
{
    if(!isReady())
        return;

    {
        if(!_db.isOpen())
            _db.open();
        _db.transaction();

        QSqlQuery query(_db);
        if(!query.exec(ConexionesSQL::_predeterReset)) {
            _db.rollback();
            return;
        }
        query.prepare(ConexionesSQL::_predeter);
        query.bindValue(":name",nConexion);

        if(!query.exec())
            _db.rollback();
        else _db.commit();

        if(_modeloBase)
            _modeloBase->select();

    }
    emit changesInDatabase();
}

DatosConexion ConexionesSQL::getConexion(QString nombre)
{
    DatosConexion datos;

    if(!isReady())
        return datos;

    {
        if(!_db.isOpen())
            _db.open();

        QSqlQuery query(_db);

        query.prepare(ConexionesSQL::_getDatos);
        query.bindValue(":nombre",nombre);

        if(!query.exec()) {
            _db.close();
            return datos;
        }

        query.next();
        QSqlRecord record=query.record();

        datos.setConnectionName(query.value(record.indexOf("nombre")).toString());
        datos.setHostName(query.value(record.indexOf("host")).toString());
        datos.setPort(query.value(record.indexOf("port")).toInt());
        datos.setDatabaseName(query.value(record.indexOf("databaseName")).toString());
    }

    return datos;
}

bool ConexionesSQL::isEmpty()
{
    if(!isReady())
        return true;

    if(_modeloBase) {
        if(_modeloBase->rowCount()!=0)
            return false;
        else return true;
    }

    {
        if(!_db.isOpen())
            _db.open();

        QSqlQuery query(_db);

        if(!query.exec(ConexionesSQL::_numConexiones))
            return true;

        query.next();
        if(query.value(0).toInt()!=0)
            return false;
        else return true;
    }

    return true;
}

bool ModeloConexiones::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int cPredeterminada=QSqlTableModel::record().indexOf("predeterminado");

    if(index.column()!=cPredeterminada)     //Modificamos otra columna
        return QSqlTableModel::setData(index,value,role);

    if(value.toInt()==0)    //Eliminar una predeterminada no dispara nada
        return QSqlTableModel::setData(index,value,role);

    int numFilas=QSqlTableModel::rowCount();

    for(int i=0;i<numFilas;i++) {
        QModelIndex idx=QSqlTableModel::index(i,cPredeterminada);
        if(i==index.row())
            QSqlTableModel::setData(index,value,role);
        else
            QSqlTableModel::setData(idx,QVariant(0));
    }

    QSqlTableModel::submitAll();
    return true;
}
