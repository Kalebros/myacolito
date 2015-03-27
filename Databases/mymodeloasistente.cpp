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

#include "mymodeloasistente.h"

QString MyModeloAsistente::_selectDNI=QString("idAsistente=(SELECT idAsistente FROM Asistente WHERE DNI=%1)");

MyModeloAsistente::MyModeloAsistente(QObject *parent, const QSqlDatabase db) :
    QAbstractTableModel(parent)
{
    if(!db.isValid() || !db.isOpen()) {
        _tablaAsistentes=0;
        _tablaReservas=0;
        _tablaPlazas=0;
    }
    else {
        _tablaAsistentes=new QSqlTableModel(this,db);
        _tablaReservas=new QSqlTableModel(this,db);
        _tablaPlazas=new QSqlTableModel(this,db);

        _tablaAsistentes->setTable(tr("Asistente"));
        _tablaReservas->setTable(tr("reservas"));
        _tablaPlazas->setTable(tr("Plaza"));

        _tablaAsistentes->select();
        _tablaAsistentes->setSort(0,Qt::AscendingOrder);

        _tablaReservas->select();

        _tablaPlazas->select();
    }

    iconoOk=QIcon(":/iconos/iconoModelo");
    iconoError=QIcon(":/iconos/iconoError");
}

int MyModeloAsistente::rowCount(const QModelIndex &parent) const
{
    return _tablaAsistentes->rowCount(parent);
}

int MyModeloAsistente::columnCount(const QModelIndex &parent) const
{
    //Numero de registros de la tabla de asistentes(7) + datos de reservas(4 sin idAsis repetido) + tipo plaza

    return 12;
}

Qt::ItemFlags MyModeloAsistente::flags(const QModelIndex &index) const
{
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
}

QVariant MyModeloAsistente::data(const QModelIndex &index, int role) const
{
    QVariant res=QVariant();

    //Los primeros siete son de tablaAsistente
    if(!index.isValid()) {
        return res;
    }

    if(index.column()<7) {
        res=_tablaAsistentes->data(_tablaAsistentes->index(index.row(),index.column()),role);
    }
    else if (index.column()==7) {
        //Mostrar el nombre de la categoria
        setPlazaOn(index);
        res=_tablaPlazas->data(_tablaPlazas->index(0,1),role);
    }
    else {
        //Buscar el registro y devolver
        setReservaOn(index);
        res=_tablaReservas->data(_tablaReservas->index(0,index.column()-7),role);
        //Si solicitamos posicion de registro en pago,activa,expirado, devolver tambien icono
        if((index.column()>=9 && index.column()<=12) && role==Qt::DecorationRole) {
            setReservaOn(index);
            if(_tablaReservas->data(_tablaReservas->index(0,index.column()-7),Qt::EditRole).toBool())
                res=iconoOk;
            else res=iconoError;
        }
    }

    return res;
}

QIcon MyModeloAsistente::getIconoOk() const
{
    return iconoOk;
}

QIcon MyModeloAsistente::getIconoFalse() const
{
    return iconoError;
}

void MyModeloAsistente::setReservaOn(const QModelIndex &idx) const
{
    QString idAsis=_tablaAsistentes->data(_tablaAsistentes->index(idx.row(),0),Qt::EditRole).toString();
    _tablaReservas->setFilter(tr("idAsistente=")+idAsis);
}

void MyModeloAsistente::setReservaOnDNI(const QString dni) const
{
    QString sentenciaSelect=_selectDNI.arg(dni);

    _tablaReservas->setFilter(sentenciaSelect);
}

void MyModeloAsistente::setPlazaOn(const QModelIndex &idx) const
{
    setReservaOn(idx);
    QString idPlaza=_tablaReservas->data(_tablaReservas->index(0,1),Qt::EditRole).toString();
    _tablaPlazas->setFilter(tr("idPlaza=")+idPlaza);
}

bool MyModeloAsistente::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(index.column()<7) {
        _tablaAsistentes->setData(index,value,role);
        _tablaAsistentes->submit();
        _tablaAsistentes->sort(0,Qt::AscendingOrder);
        if(index.column()==6)   //Cambio en IDgrupo, hay que recargar el modelo de asignacion de habitacion
            emit cambiosGrupo();
    }
    else if(index.column()==7) {
        setPlazaOn(index);
        QString oldCategoria=_tablaPlazas->data(_tablaPlazas->index(0,1),Qt::EditRole).toString();
        QString newCategoria=index.data(Qt::EditRole).toString();
        _tablaPlazas->setData(_tablaPlazas->index(0,1),value,role);
        _tablaPlazas->submit();
        emit borradaPlaza(oldCategoria);
    }
    else if(index.column()<11) {
        setReservaOn(index);
        _tablaReservas->setData(_tablaReservas->index(0,index.column()-7),value,role);
        _tablaReservas->submit();
    }

    emit dataChanged(index,index);
    return true;
}

void MyModeloAsistente::reload()
{
    this->beginResetModel();
    _tablaAsistentes->submitAll();
    _tablaAsistentes->select();
    _tablaAsistentes->sort(0,Qt::AscendingOrder);
    _tablaReservas->submitAll();
    _tablaReservas->select();
    _tablaPlazas->submitAll();
    _tablaPlazas->select();
    this->endResetModel();
}

void MyModeloAsistente::addAsistente(Asistente *asis)
{
    beginInsertRows(QModelIndex(),_tablaAsistentes->rowCount(),_tablaAsistentes->rowCount());
    if(asis->idAsis==-1) //Crear nuevo asistente
    {
        //Insertar asistente
        QSqlRecord nAsistente=_tablaAsistentes->record();
        nAsistente.clearValues();
        nAsistente.setValue(tr("nombre"),asis->nombre);
        nAsistente.setValue(tr("dni"),asis->DNI);
        nAsistente.setValue(tr("mail"),asis->mail);
        nAsistente.setValue(tr("telefono"),asis->telefono);
        nAsistente.setValue(tr("comentarios"),asis->comentarios);
        nAsistente.setValue(tr("IDgrupo"),-1);
        if(!_tablaAsistentes->insertRecord(-1,nAsistente)) {
            qDebug() << "ERROR en tablaAsistentes: " << _tablaAsistentes->lastError().text();
        }
        _tablaAsistentes->submitAll();
        _tablaAsistentes->sort(0,Qt::AscendingOrder);

        //Crear plaza y asignarla
        {
            QSqlQuery query(_tablaPlazas->database());
            query.prepare("CALL AsignarPlazaDNI(:dni,:tipo)");
            query.bindValue(":dni",asis->DNI);
            query.bindValue(":tipo",asis->tipo);

            query.exec();
        }

        //Modificar valores
        {
            QSqlQuery query(_tablaReservas->database());
            query.prepare("CALL SetEstadoReservaDNI(:dni,:pago,:estado,:expirado)");
            query.bindValue(":dni",asis->DNI);
            query.bindValue(":pago",asis->pago);
            query.bindValue(":estado",asis->activo);
            query.bindValue(":expirado",asis->expirado);

            query.exec();
        }
        _tablaReservas->select();
        _tablaPlazas->select();
        emit nuevaPlaza(asis->tipo);
        emit cambiosGrupo();
    }
    endInsertRows();
}

void MyModeloAsistente::deleteAsistente(Asistente *asis)
{
    //TODO: testear
    QModelIndex indiceAsis=getModelIndexOf(asis->DNI);

    beginRemoveRows(QModelIndex(),indiceAsis.row(),indiceAsis.row());

    {
        QSqlQuery query(_tablaAsistentes->database());
        query.prepare("CALL BorrarAsistenteDNI(:dni)");
        query.bindValue(":dni",asis->DNI);
        query.exec();
    }

    _tablaAsistentes->select();
    _tablaReservas->select();
    _tablaPlazas->select();

    emit borradaPlaza(asis->tipo);
    emit cambiosGrupo();
    endRemoveRows();
}

QVariant MyModeloAsistente::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole) {
        switch(section) {
        case 0:
            res=QString(tr("IDAsistente"));
            break;
        case 1:
            res=QString(tr("Nombre"));
            break;
        case 2:
            res=QString(tr("DNI"));
            break;
        case 3:
            res=QString(tr("Mail"));
            break;
        case 4:
            res=QString(tr("Telefono"));
            break;
        case 5:
            res=QString(tr("Comentarios"));
            break;
        case 6:
            res=QString(tr("IDgrupo"));
            break;
        case 7:
            res=QString(tr("Categoria"));
            break;
        case 8:
            res=QString(tr("IDplaza"));
            break;
        case 9:
            res=QString(tr("Pago"));
            break;
        case 10:
            res=QString(tr("Activa"));
            break;
        case 11:
            res=QString(tr("Expirada"));
            break;
        default:
            break;
        }
    }
    else if(orientation==Qt::Vertical && role==Qt::DisplayRole) {
        res=QAbstractTableModel::headerData(section,orientation,role);
    }

    return res;
}

void MyModeloAsistente::agruparAsistentes(QList<Asistente *> lista)
{
    {
        QSqlQuery query(_tablaAsistentes->database());

        query.prepare("CALL AsignarAsistenteGrupoDNI(:dniAsis,:grupo)");

        //Recuperamos el grupo del primer asistente, todos los demas iran a ese grupo
        int idGrupo;
        {
            QSqlQuery qAux(_tablaAsistentes->database());
            qAux.prepare("SELECT IDgrupo from Asistente WHERE dni=:dni");
            qAux.bindValue(":dni",lista.first()->DNI);

            if(!qAux.exec()) {
                qDebug() << "ERROR en MyModeloAsistente->agruparAsistentes(): " << qAux.lastError().text();
                return;
            }
            qAux.next();

            idGrupo=qAux.value(0).toInt();
        }

        //El codigo comentado produce un error al intentar añadir asistentes desde un script,
        //posiblemente porque la tabla de asistentes no se actualiza con un select despues de añadir
        //a los anteriores
        /*
        _tablaAsistentes->setFilter(tr("DNI=")+lista.first()->DNI);
        int idGrupo=_tablaAsistentes->record(0).value(tr("IDgrupo")).toInt();
        */

        QList<QVariant> listaDni,listaGrupo;

        for(int i=1;i<lista.count();i++) {
            listaDni << lista.at(i)->DNI;
            listaGrupo << QVariant(idGrupo);
        }

        query.bindValue(":dniAsis",listaDni);
        query.bindValue(":grupo",listaGrupo);

        _tablaAsistentes->database().transaction();

        if(!query.execBatch()) {
            qDebug() << "ERROR en MyModeloAsistente->agruparAsistentes(): " << query.lastError().text();
            _tablaAsistentes->database().rollback();
            return;
        }

        _tablaAsistentes->database().commit();
        _tablaAsistentes->setFilter(QString());
        _tablaAsistentes->sort(0,Qt::AscendingOrder);

        //emitimos la señal de cambio de datos en los diferentes asistentes

        QModelIndex idxAsistente;
        foreach(Asistente *asis,lista) {
            idxAsistente=getModelIndexOf(asis->DNI);
            emit dataChanged(index(idxAsistente.row(),_tablaAsistentes->record().indexOf("IDgrupo")),
                             index(idxAsistente.row(),_tablaAsistentes->record().indexOf("IDgrupo")));
        }
        emit cambiosGrupo();

    }
}

void MyModeloAsistente::cambiarAsistenteDeGrupo(Asistente *asistente, int idGrupo)
{
    if(_tablaAsistentes) {
        int numAsistentes=_tablaAsistentes->rowCount();
        int i=0;
        bool found=false;
        QSqlRecord record;
        for(i=0;i<numAsistentes && !found;i++) {
            if(_tablaAsistentes->record(i).value("IDasistente").toInt()==asistente->idAsis)
                found=true;
        }

        if(found) {
            _tablaAsistentes->setData(_tablaAsistentes->index(i-1,6),idGrupo,Qt::EditRole);
            _tablaAsistentes->submitAll();
            emit dataChanged(index(i-1,6),index(i-1,6));
        }
    }
}

void MyModeloAsistente::nuevoGrupoAsistentes(QList<Asistente *> lista)
{
    {
        QSqlQuery query(_tablaAsistentes->database());

        QList<QVariant> listaDNI;

        foreach(Asistente *asis,lista)
            listaDNI << asis->DNI;

        //Separar a los asistentes en grupos individuales
        query.prepare("CALL SepararAsistenteGrupoDNI(:dni)");

        query.bindValue(":dni",listaDNI);

        _tablaAsistentes->database().transaction();

        if(!query.execBatch()) {
            qDebug() << "ERROR en MyModeloAsistente->nuevoGrupoAsistentes():" << query.lastError().text();
            _tablaAsistentes->database().rollback();
            return;
        }
        _tablaAsistentes->database().commit();

        //Agrupar asistentes
        agruparAsistentes(lista);
    }
}

void MyModeloAsistente::separarAsistente(int identificador)
{
    //Separar al asistente en un grupo
    QSqlQuery query(_tablaAsistentes->database());
    query.prepare("CALL SepararAsistenteGrupo(:identificador)");

    query.bindValue(":identificador",identificador);

    _tablaAsistentes->database().transaction();

    if(!query.exec()) {
        qDebug() << "ERROR en MyModeloAsistente->separarAsistente:" << query.lastError().text();
        _tablaAsistentes->database().rollback();
        return;
    }
    _tablaAsistentes->database().commit();

    int numFilas=_tablaAsistentes->rowCount();

    _tablaAsistentes->select();
    for(int i=0;i<numFilas;i++)
        emit dataChanged(index(i,6),index(i,6));
}

QModelIndex MyModeloAsistente::getModelIndexOf(QString dni)
{
    QModelIndex res;
    int numFilas=_tablaAsistentes->rowCount();
    bool found=false;
    for(int i=0;i<numFilas && !found;i++) {
        if(_tablaAsistentes->record(i).value(tr("DNI")).toString()==dni) {
            found=true;
            res=_tablaAsistentes->index(i,0);
        }
    }

    return res;
}

void MyModeloAsistente::updateCategorias()
{
    int numAsistentes=_tablaAsistentes->rowCount();
    for(int i=0;i<numAsistentes;i++) {
        emit dataChanged(index(i,7),
                         index(i,7));
    }
}

bool MyModeloAsistente::pagoAsistente(QString dni) const
{
    setReservaOnDNI(dni);
    QSqlRecord record=_tablaReservas->record(0);

    return record.value(record.indexOf("haPagado")).toBool();
}
