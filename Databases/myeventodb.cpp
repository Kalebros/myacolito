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

#include "myeventodb.h"

MyEventoDB *MyEventoDB::singleInstance=0;
QString MyEventoDB::_queryGetEvento=QString("SELECT nombre,comentario,nombreDatabase,fechaEvento FROM listado,fechas "
                                            "WHERE listado.idEvento=fechas.idEvento AND nombre=:evento");
QString MyEventoDB::_queryAddEvento=QString("INSERT INTO listado(nombre,comentario,nombreDatabase) "
                                            "VALUES(:nombre,:comentario,:database)");
QString MyEventoDB::_queryAddFecha=QString("INSERT INTO fechas(idEvento,fechaEvento) "
                                            "VALUES(:id,:fecha)");

Evento::Evento() :
    _nombre(),
    _database(),
    _fechas()
{}

void Evento::nombre(QString n)
{
    _nombre=n;
}

QString Evento::nombre() const {
    return _nombre;
}

void Evento::comentario(QString c) {
    _comentario=c;
}

QString Evento::comentario() const {
    return _comentario;
}

void Evento::database(QString d) {
    _database=d;
}

QString Evento::database() const {
    return _database;
}

void Evento::addFecha(QDate d) {
    if(!_fechas.contains(d))
        _fechas.append(d);
}

void Evento::addFecha(QList<QDate> lista) {
    _fechas=lista;
}

void Evento::addFecha(QList<QVariant> lista) {
    foreach(QVariant value,lista) {
        _fechas.append(value.toDate());
    }
}

void Evento::removeFecha(QDate d) {
    if(_fechas.contains(d))
        _fechas.removeOne(d);
}

QList<QDate> Evento::getFechas() const {
    return _fechas;
}

MyEventoDB::MyEventoDB(QObject *parent, DatosConexion *datos)
{
    _database= new MyAcolitoDB(parent,datos,tr("MyEvento"));
    _modeloBase=0;
}

MyEventoDB *MyEventoDB::getInstance(QObject *parent, DatosConexion *datos)
{
    if(!singleInstance)
        singleInstance=new MyEventoDB(parent,datos);

    return singleInstance;
}

bool MyEventoDB::isConnected() const
{
    return _database->isConnected();
}

void MyEventoDB::disconnect()
{
    if(_modeloBase) {
        _modeloBase->disconnect(SIGNAL(nuevoEvento(Evento*,QStandardItemModel*)));
        delete _modeloBase;
        _modeloBase=0;
    }
    _database->disconnect();
}

bool MyEventoDB::connectToServer(DatosConexion datos)
{
    return _database->connectToServer(datos);
}

bool MyEventoDB::existeDatabase()
{
    return _database->existeDatabase();
}

bool MyEventoDB::createDatabase()
{
    return _database->createDatabase(tr(":/scripts/scriptEvento"));
}

bool MyEventoDB::openDatabase()
{
    return _database->openDatabase();
}

QPair<int,QString> MyEventoDB::lastError() const
{
    return _database->lastError();
}

QSortFilterProxyModel *MyEventoDB::getModeloEventos()
{
    if(!_modeloBase) {
        _modeloBase=new MyModeloEventos(this,_database->database());
        _modeloBase->setOkIcon(QIcon(":/iconos/iconoModelo"));
        _modeloBase->setErrorIcon(QIcon(":/iconos/iconoModelo"));
        _modeloBase->reload();

        connect(_modeloBase,SIGNAL(nuevoEvento(Evento*,QStandardItemModel*)),this,SIGNAL(nuevoEvento(Evento*,QStandardItemModel*)));
    }

    QSortFilterProxyModel *res=new QSortFilterProxyModel(this);
    res->setSourceModel(_modeloBase);

    return res;
}

void MyEventoDB::reloadModeloEventos()
{
    if(_modeloBase) {
        _modeloBase->reload();
    }
}


Evento *MyEventoDB::getEvento(QString ev)
{
    Evento *res=new Evento();
    {
        QSqlQuery query(_database->database());
        query.prepare(MyEventoDB::_queryGetEvento);
        query.bindValue(":evento",ev);

        query.exec();

        query.next();
        res->nombre(query.value(0).toString());
        res->comentario(query.value(1).toString());
        res->database(query.value(2).toString());

        do {
            res->addFecha(query.value(3).toDate());
        } while(query.next());
    }

    return res;
}

void MyEventoDB::addEvento(Evento ev, QStandardItemModel *categorias)
{
    /*
    {
        _database->database().transaction();
        QSqlQuery query(_database->database());
        query.prepare(MyEventoDB::_queryAddEvento);
        query.bindValue(":nombre",ev.nombre());
        query.bindValue(":comentario",ev.comentario());
        query.bindValue(":database",ev.database());

        query.exec();

        query.prepare("SELECT idEvento FROM listado WHERE nombre=:nombre");
        query.bindValue(":nombre",ev.nombre());

        query.exec();
        query.next();

        int identificador=query.value(0).toInt();

        query.prepare(MyEventoDB::_queryAddFecha);

        QList<QDate> listaFechas=ev.getFechas();

        QVariantList listaIdentificador;
        QVariantList fechas;

        foreach(QDate value,listaFechas) {
            listaIdentificador << identificador;
            fechas << value;
        }

        query.bindValue(tr(":id"),listaIdentificador);
        query.bindValue(tr(":fecha"),fechas);

        query.execBatch();

        _database->database().commit();
    }
    _modeloBase->reload();
    */

    _modeloBase->addEvento(&ev,categorias);
}

DatosConexion MyEventoDB::getDatosConexion() const
{
    return _database->getDatosConexion();
}

MyAcolitoDB *MyEventoDB::getConexion()
{
    return _database;
}
