/***********************************************************************
** SManager, copyright @2013,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of SManager.
**
** SManager is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SManager is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with SManager.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "smstolite.h"

#include "smscript.h"
#include "smscriptlite.h"

int SMStoLite::_idConexion=0;
QString SMStoLite::_nombreConexionBase=QString("conexionStoLite");

SMStoLite::SMStoLite(QObject *parent, QString pathArchivo) :
    SMStorage(parent),
    _path(pathArchivo),
    _modeloScripts(0)
{
}

SMStoLite::~SMStoLite()
{
}

bool SMStoLite::isOpen() const
{
    return db.isOpen();
}

bool SMStoLite::openAlmacen()
{
    if(db.isOpen())
        return true;

    //Comprobar si existe el archivo
    bool existe=QFile::exists(_path);

    db=QSqlDatabase::addDatabase("QSQLITE",_nombreConexionBase+QString::number(_idConexion++));

    db.setDatabaseName(_path);

    if(!db.open()) {
        _lastError=tr("ERROR en almacen SMStoLite: ")+db.lastError().text();
        QSqlDatabase::removeDatabase(db.connectionName());
        return false;
    }

    if(!existe) { //Lanzar script para crear el almacen
        if(!SMStoLite::crearAlmacenLite(db)) {
            _lastError=tr("ERROR al crear almacen SMStoLite: ")+db.lastError().text();
            db.close();
            QSqlDatabase::removeDatabase(db.connectionName());
            return false;
        }
    }

    if(_modeloScripts) {
        delete _modeloScripts;
        _modeloScripts=0;
    }

    _modeloScripts=new QSqlTableModel(this,db);
    _modeloScripts->setTable("Script");
    _modeloScripts->setSort(1,Qt::AscendingOrder);
    _modeloScripts->select();

    return true;
}

bool SMStoLite::crearAlmacenLite(QSqlDatabase db)
{
    bool res=true;
    {
        QSqlQuery query(db);

        query.prepare("CREATE TABLE Script ( "
                      "idScript   INTEGER PRIMARY KEY AUTOINCREMENT "
                                         "NOT NULL, "
                      "nombre     TEXT    NOT NULL "
                                         "DEFAULT ( ' ' ), "
                      "path       TEXT    NOT NULL "
                                         "DEFAULT ( ' ' ), "
                      "argumentos TEXT    DEFAULT ( ' ' ) "
                                         "NOT NULL "
                      ")");
        res=query.exec();
    }

    return res;
}

void SMStoLite::closeAlmacen()
{
    if(!db.isOpen())
        return;

    if(_modeloScripts) {
        delete _modeloScripts;
        _modeloScripts=0;
    }

    db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
}

int SMStoLite::size() const
{
    int res=-1;

    if(!db.isOpen())
        return res;

    if(_modeloScripts)
        res=_modeloScripts->rowCount();

    return res;
}

SMScript *SMStoLite::getScript(int num)
{
    if(!db.isOpen())
        return 0;

    int id=_modeloScripts->index(num,0).data().toInt();

    return new SMScriptLite(this,id,this,_modeloScripts);
}

int SMStoLite::addScript(SMScript *script)
{
    int res=-1;
    if(!db.isOpen())
        return res;

    {
        db.transaction();
        QSqlQuery query(db);
        query.prepare("INSERT INTO Script(nombre,path,argumentos)"
                      "VALUES(?,?,?)");

        query.addBindValue(script->getNombre());
        query.addBindValue(script->getPath());
        query.addBindValue(script->getArgumentos());

        if(!query.exec()) {
            _lastError=QString("ERROR en SMStoLite::addScript: ")+query.lastError().text();
            db.rollback();
            res=-1;
        }
        else {
            db.commit();
            _modeloScripts->select();
            res=_modeloScripts->rowCount()-1;
        }
    }

    _modeloScripts->select();
    return res;
}

int SMStoLite::addScript(QString nombre,QString path,QString argumentos)
{
    int res=-1;
    if(!db.isOpen())
        return res;

    {
        db.transaction();
        QSqlQuery query(db);
        query.prepare("INSERT INTO Script(nombre,path,argumentos)"
                      "VALUES(?,?,?)");

        query.addBindValue(nombre);
        query.addBindValue(path);
        query.addBindValue(argumentos);

        if(!query.exec()) {
            _lastError=QString("ERROR en SMStoLite::addScript: ")+query.lastError().text();
            db.rollback();
            res=-1;
        }
        else {
            db.commit();
            _modeloScripts->select();
            res=_modeloScripts->rowCount()-1;
        }
    }

    _modeloScripts->select();
    return res;
}


bool SMStoLite::removeScript(int id)
{
    if(!db.isOpen())
        return false;

    bool res=_modeloScripts->removeRow(id);
    _modeloScripts->select();

    return res;
}

void SMStoLite::addAlmacen(SMStorage *almacen)
{
    if(!db.isOpen() || !almacen->isOpen())
        return;

    int nScripts=almacen->size();
    QStringList nombres,paths,argumentos;

    SMScript *script;
    for(int i=0;i<nScripts;i++) {
        script=almacen->getScript(i);
        nombres << script->getNombre();
        paths << script->getPath();
        argumentos << script->getArgumentos();
    }

    {
        db.transaction();
        QSqlQuery query(db);
        query.prepare("INSERT INTO Script(nombre,path,argumentos)"
                      "VALUES(?,?,?)");

        query.addBindValue(nombres);
        query.addBindValue(paths);
        query.addBindValue(argumentos);

        if(!query.execBatch()) {
            _lastError=QString("ERROR en SMStoLite::addScript: ")+query.lastError().text();
            db.rollback();
            return;
        }
        db.commit();
    }

    _modeloScripts->select();
}
