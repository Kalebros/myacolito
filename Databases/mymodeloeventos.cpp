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

#include "mymodeloeventos.h"

#include <QDebug>


MyModeloEventos::MyModeloEventos(QObject *parent, const QSqlDatabase &db) :
    QAbstractListModel(parent)
{
    _modeloNombres=0;
    _modeloFechas=0;
    setupModel(db);
}

void MyModeloEventos::setOkIcon(QIcon icono)
{
    okIcon=icono;
}

void MyModeloEventos::setErrorIcon(QIcon icono)
{
    errorIcon=icono;
}

QIcon MyModeloEventos::getOkIcon() const
{
    return okIcon;
}

QIcon MyModeloEventos::getErrorIcon() const
{
    return errorIcon;
}

void MyModeloEventos::addEvento(Evento *ev, QStandardItemModel *categorias)
{

    int numEventos=_modeloNombres->rowCount();

    beginInsertRows(QModelIndex(),numEventos,numEventos+1);
    QSqlRecord nuevoEvento=_modeloNombres->record();
    nuevoEvento.clearValues();

    nuevoEvento.setValue(tr("nombre"),ev->nombre());
    nuevoEvento.setValue(tr("comentario"),ev->comentario());
    nuevoEvento.setValue(tr("nombreDatabase"),ev->database());

    _modeloNombres->insertRecord(-1,nuevoEvento);
    _modeloNombres->submitAll();
    _modeloNombres->select();

    /*
      Esto es extremadamente feo, pero no he conseguido que funcione de otra
      forma. En teoria, deberia de poder recuperar los datos consultando el
      _modeloNombres, pero no me devuelve el identificador del ultimo evento
      insertado correctamente.
      */

    int idEvento;
    {
        QSqlQuery query(_modeloNombres->database());
        query.prepare("SELECT idEvento FROM listado WHERE nombre=:nombre");
        query.bindValue(tr(":nombre"),ev->nombre());
        query.exec();
        query.next();
        idEvento=query.value(0).toInt();
    }

    /***/

    QSqlRecord nuevasFechas=_modeloFechas->record();
    QList<QDate> listadoFechas=ev->getFechas();
    foreach(QDate fecha,listadoFechas) {
        nuevasFechas.clearValues();
        nuevasFechas.setValue(tr("idEvento"),idEvento);
        nuevasFechas.setValue(tr("fechaEvento"),fecha);
        _modeloFechas->insertRecord(-1,nuevasFechas);
    }
    _modeloFechas->submitAll();
    _modeloFechas->select();
    endInsertRows();

    emit this->nuevoEvento(ev,categorias);
}

void MyModeloEventos::reload()
{
    QAbstractListModel::beginResetModel();
    _modeloNombres->select();
    _modeloFechas->select();
    QAbstractListModel::endResetModel();
    /*
    foreach(ItemEvento *evento,listaEventos)
        delete evento;

    listaEventos.clear();
    QAbstractItemModel::reset();

    setupModel();
    */
}

void MyModeloEventos::setupModel(const QSqlDatabase &db)
{
    //Cargar las tablas de listado y de fechas
    if(_modeloNombres) {
        delete _modeloNombres;
        _modeloNombres=0;
    }

    if(_modeloFechas) {
        delete _modeloFechas;
        _modeloFechas=0;
    }

    _modeloNombres=new QSqlTableModel(this,db);
    _modeloFechas=new QSqlTableModel(this,db);

    _modeloNombres->setTable(tr("listado"));
    _modeloFechas->setTable(tr("fechas"));

    _modeloNombres->select();
    _modeloFechas->select();

    /*
    QDir directorioPrincipal;
    QDir directorioEvento;
    MyModeloEventos::ItemEvento *evento;
    directorioPrincipal.setPath(qApp->applicationDirPath()+"/Eventos");
    QStringList listadoDirectoriosEventos=directorioPrincipal.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    qDebug() << listadoDirectoriosEventos;
    foreach(QString directorio,listadoDirectoriosEventos) {
        evento=new MyModeloEventos::ItemEvento();
        evento->setDirectorio(directorioPrincipal.path()+'/'+directorio);

        qDebug() << evento->getDirectorio();
        //Prepara la conexion de listado
        */

        /*! @todo Modificar conexion para soportar tambien listado MySQL... o modificar el modelo */

    /*
        if(!QSqlDatabase::contains("listado")) {
            QSqlDatabase::addDatabase("QSQLITE","listado");
        }
        else QSqlDatabase::database("listado").close();

        QSqlDatabase db=QSqlDatabase::database("listado");
        db.setDatabaseName(evento->getDirectorio()+"/Evento.db");
        if(!db.open()) {
            evento->setNombre("No se encontro evento en el directorio");
            evento->setComentario(db.lastError().text());
            evento->setIcono(&errorIcon);

            qDebug() << db.lastError().text();
        } else {    //Revisar los datos de evento para generar el item
            QSqlQuery query(db);
            query.prepare("SELECT nombre,comentario FROM metaDatos");
            query.exec();
            query.first();
            evento->setNombre(query.value(0).toString());
            evento->setComentario(query.value(1).toString());
            query.clear();
            query.exec("SELECT diaEvento FROM Fechas ORDER BY diaEvento ASC");
            while(query.next())
                evento->addFecha(query.value(0).toDate());
            evento->setIcono(&okIcon);
        }
        this->listaEventos.append(evento);
    }
    if(QSqlDatabase::contains("listado")) {
        QSqlDatabase::database("listado").close();
        QSqlDatabase::removeDatabase("listado");
    }

    */
}

MyModeloEventos::~MyModeloEventos()
{
    if(_modeloNombres)
        delete _modeloNombres;

    if(_modeloFechas)
        delete _modeloFechas;

    /*
    foreach(ItemEvento *evento,listaEventos)
        delete evento;

    listaEventos.clear();
    */
}

/*
MyModeloEventos::ItemEvento::ItemEvento()
{
}

MyModeloEventos::ItemEvento::~ItemEvento()
{
    fechas.clear();
}

void MyModeloEventos::ItemEvento::setNombre(QString nom)
{
    this->nombre=nom;
}

void MyModeloEventos::ItemEvento::setComentario(QString comentario)
{
    this->comentario=comentario;
}

void MyModeloEventos::ItemEvento::setDirectorio(QString dir)
{
    this->directorio=dir;
}

void MyModeloEventos::ItemEvento::addFecha(QDate fecha)
{
    this->fechas.append(fecha);
}

void MyModeloEventos::ItemEvento::setIcono(QIcon *icono)
{
    this->iconoAsociado=icono;
}

QIcon *MyModeloEventos::ItemEvento::getIcono()
{
    return iconoAsociado;
}

QString MyModeloEventos::ItemEvento::getNombre() const
{
    return nombre;
}

QString MyModeloEventos::ItemEvento::getDirectorio() const
{
    return directorio;
}

QString MyModeloEventos::ItemEvento::getFechasString() const
{
    QString res;
    QSet<QString> listaAnyos;
    QList<QString> listaNoRepetida;

    foreach(QDate dia,fechas) {
        res.append(dia.toString("dd MMMM, "));
        listaAnyos.insert(dia.toString("yyyy"));
    }

    listaNoRepetida=listaAnyos.toList();
    res.remove(res.size()-2,2);
    res.append(" - ");
    foreach(QString year,listaNoRepetida) {
        res.append(year+", ");
    }

    res.remove(res.size()-2,2);
    return res;
}

QList<QDate> MyModeloEventos::ItemEvento::getListaFechas() const
{
    return fechas;
}

QString MyModeloEventos::ItemEvento::getComentario() const
{
    return comentario;
}
*/

int MyModeloEventos::rowCount(const QModelIndex &/*parent*/) const
{
    return _modeloNombres->rowCount();
    /*
    return this->listaEventos.count();
    */
}

QVariant MyModeloEventos::data(const QModelIndex &index, int role) const
{

    if(!index.isValid())
        return QVariant();

    QVariant res;

    switch(role)
    {
    case Icono:
        res=okIcon;
        break;
    case Nombre :  // Devolver el nombre, columna 1 de la tabla
        res=_modeloNombres->data(_modeloNombres->index(index.row(),1),Qt::DisplayRole);
        break;
    case Directorio:    //No hay directorio en este caso, devolvemos el nombre de la base de datos
        res=_modeloNombres->data(_modeloNombres->index(index.row(),3),Qt::DisplayRole);
        break;
    case Fechas :
        res=getFechasString(index);
        //res=listaEventos.at(index.row())->getFechasString();
        break;
    case Comentario:    //Devolver comentario, columna 2 de la tabla
        res=_modeloNombres->data(_modeloNombres->index(index.row(),2),Qt::DisplayRole);
        break;
    default:
        break;
    }

    return res;
}

QString MyModeloEventos::getFechasString(const QModelIndex idx) const
{
    int identificador=_modeloNombres->data(_modeloNombres->index(idx.row(),0),Qt::DisplayRole).toInt();

    QList<QDate> listadoFechas;

    int num=_modeloFechas->rowCount();

    for(int i=0;i<num;i++) {
        if(_modeloFechas->data(_modeloFechas->index(i,0)).toInt()==identificador) {
            QDate fecha=_modeloFechas->data(_modeloFechas->index(i,1)).toDate();
            listadoFechas.append(fecha);
        }
    }

    return convertToString(listadoFechas);
}

QString MyModeloEventos::convertToString(QList<QDate> lista) const
{
    QString res;
    QSet<QString> listaAnyos;
    QList<QString> listaNoRepetida;

    foreach(QDate dia,lista) {
        res.append(dia.toString("dd MMMM, "));
        listaAnyos.insert(dia.toString("yyyy"));
    }

    listaNoRepetida=listaAnyos.toList();
    res.remove(res.size()-2,2);
    res.append(" - ");
    foreach(QString year,listaNoRepetida) {
        res.append(year+", ");
    }

    res.remove(res.size()-2,2);
    return res;
}
