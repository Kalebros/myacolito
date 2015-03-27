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

#include "mymodeloactividades.h"

MyModeloActividades::MyModeloActividades(QObject *parent, const QSqlDatabase db) :
    QAbstractTableModel(parent)
{

    if(!db.isOpen() || !db.isValid()) {
        _tablaActividades=0;
    }
    else {
        _tablaActividades=new QSqlTableModel(this,db);
        _tablaActividades->setTable(tr("actividad"));
        _tablaActividades->setSort(0,Qt::AscendingOrder);
        _tablaActividades->select();
    }

    //Inicializar iconos
    iconoOk=QIcon(":/iconos/iconoModelo");
    iconoError=QIcon(":/iconos/iconoError");
}

int MyModeloActividades::rowCount(const QModelIndex &parent) const
{
    if(_tablaActividades)
        return _tablaActividades->rowCount();

    else return 0;
}

int MyModeloActividades::columnCount(const QModelIndex &parent) const
{
    if(_tablaActividades)
        return _tablaActividades->columnCount();
    else return 0;
}

QVariant MyModeloActividades::data(const QModelIndex &index, int role) const
{
    if(!_tablaActividades || !index.isValid())
        return QVariant();

    if(index.column()==10 && role==Qt::DecorationRole) {
        if(_tablaActividades->data(_tablaActividades->index(index.row(),index.column()),Qt::EditRole).toBool())
            return QVariant(iconoOk);
        else return QVariant(iconoError);
    }

    return _tablaActividades->data(_tablaActividades->index(index.row(),index.column()),role);
}

QVariant MyModeloActividades::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole) {
        switch(section) {
        case 0:
            res=QString(tr("ID"));
            break;
        case 1:
            res=QString(tr("Actividad"));
            break;
        case 2:
            res=QString(tr("Asociacion"));
            break;
        case 3:
            res=QString(tr("Responsable"));
            break;
        case 4:
            res=QString(tr("Correo"));
            break;
        case 5:
            res=QString(tr("Coorganizadores"));
            break;
        case 6:
            res=QString(tr("MatPropio"));
            break;
        case 7:
            res=QString(tr("MatOrganizacion"));
            break;
        case 8:
            res=QString(tr("nEspacio"));
            break;
        case 9:
            res=QString(tr("DiaActividad"));
            break;
        case 10:
            res=QString(tr("TodaJornadas"));
            break;
        case 11:
            res=QString(tr("hInicio"));
            break;
        case 12:
            res=QString(tr("hFin"));
            break;
        case 13:
            res=QString(tr("MaximoP"));
            break;
        case 14:
            res=QString(tr("MinimoP"));
            break;
        case 15:
            res=QString(tr("Tipo"));
            break;
        case 16:
            res=QString(tr("Descripcion"));
            break;
        case 17:
            res=QString(tr("Requisitos"));
            break;
        case 18:
            res=QString(tr("Observaciones"));
            break;
        case 19:
            res=QString(tr("Lugar"));
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

Qt::ItemFlags MyModeloActividades::flags(const QModelIndex &index) const
{
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
}

bool MyModeloActividades::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool res=_tablaActividades->setData(_tablaActividades->index(index.row(),index.column()),value,role);
    _tablaActividades->submit();

    emit dataChanged(index,index);
    return res;
}

void MyModeloActividades::addActividad(QVariantMap datosActividad)
{
    if(_tablaActividades) {
        int nActividades=_tablaActividades->rowCount();
        QSqlQuery insertAct(_tablaActividades->database());

        _tablaActividades->database().transaction();

        insertAct.prepare("INSERT INTO Actividad "
                          "(nombre,asociacion,responsable,correo,coorganizadores,materialPropio,materialOrganizacion,necesidadesEspacio,dia,todaJornada,horaInicio,horaFin,minimoParticipantes,maximoParticipantes,tActividad,descripcion,requisitos,observaciones,lugar) "
                          "VALUES "
                          "(:nombre,:asociacion,:responsable,:correo,:coorganizadores,:mPropio,:mOrganizacion,:espacio,:dia,:todaJornada,:horaInicio,:horaFin,:minimoPart,:maxPart,:tipoAct,:descripcion,:requisitos,:observaciones,:lugar)");

        insertAct.bindValue(":nombre",datosActividad["nombre"]);
        insertAct.bindValue(":asociacion",datosActividad["asociacion"]);
        insertAct.bindValue(":responsable",datosActividad["responsable"]);
        insertAct.bindValue(":correo",datosActividad["correo"]);
        if(datosActividad["coorganizadores"].isNull() || !datosActividad["coorganizadores"].isValid())
            insertAct.bindValue(":coorganizadores",QString(" "));
        else
            insertAct.bindValue(":coorganizadores",datosActividad["coorganizadores"]);
        insertAct.bindValue(":mPropio",datosActividad["mPropio"]);
        insertAct.bindValue(":mOrganizacion",datosActividad["mOrganizacion"]);
        insertAct.bindValue(":espacio",datosActividad["espacio"]);
        insertAct.bindValue(":dia",datosActividad["dia"]);
        insertAct.bindValue(":todaJornada",datosActividad["todaJornada"]);
        insertAct.bindValue(":horaInicio",datosActividad["horaInicio"]);
        insertAct.bindValue(":horaFin",datosActividad["horaFin"]);
        insertAct.bindValue(":minimoPart",datosActividad["minimoPart"]);
        insertAct.bindValue(":maxPart",datosActividad["maximoPart"]);
        insertAct.bindValue(":tipoAct",datosActividad["tipo"]);
        insertAct.bindValue(":descripcion",datosActividad["descripcion"]);
        insertAct.bindValue(":requisitos",datosActividad["requisitos"]);
        insertAct.bindValue(":observaciones",datosActividad["observaciones"]);
        insertAct.bindValue(":lugar",datosActividad["lugar"]);

        if(!insertAct.exec()) {
            qDebug() << "ERROR en MyModeloActividades->addActividad( "+datosActividad["nombre"].toString()+"):"+insertAct.lastError().text();
            _tablaActividades->database().rollback();
            return;
        }

        _tablaActividades->database().commit();

        _tablaActividades->select();
        beginInsertRows(QModelIndex(),nActividades,nActividades);
        endInsertRows();
        //Si es necesario, emitir tambien para actualizar todos los datos de la tabla completa
    }
}

void MyModeloActividades::addListaActividad(QList<QVariantMap> listaActiviades)
{
    foreach(QVariantMap actividad,listaActiviades)
        addActividad(actividad);
}

void MyModeloActividades::delActividad(int idActividad)
{
    if(_tablaActividades) {
        int numActividades=_tablaActividades->rowCount();
        QSqlQuery query(_tablaActividades->database());

        _tablaActividades->database().transaction();

        query.prepare("DELETE FROM actividad WHERE idActividad=:id");
        query.bindValue(":id",idActividad);

        if(!query.exec()) {
            qDebug() << tr("ERROR en MyModeloActividades->delActividad: ")+query.lastError().text();
            _tablaActividades->database().rollback();
            return;
        }

        _tablaActividades->select();

        beginResetModel();
        endResetModel();
    }
}

void MyModeloActividades::delActividad(QVariant idActividad)
{
    delActividad(idActividad.toInt());
}

void MyModeloActividades::activarTodasLasJornadas(int idActividad, bool modo)
{
    if(_tablaActividades) {
        int nAct=_tablaActividades->rowCount();
        bool found=false;
        for(int i=0;i<nAct && !found;i++) {
            if(_tablaActividades->record(i).value(tr("idActividad")).toInt()==idActividad) {
                found=true;
                QModelIndex indice=_tablaActividades->index(i,10);
                _tablaActividades->setData(indice,QVariant(modo));
                emit dataChanged(indice,indice);
            }
        }
    }
}

void MyModeloActividades::activarTodasLasJornadas(QVariant idActividad, bool modo)
{
    activarTodasLasJornadas(idActividad.toInt(),modo);
}

bool MyModeloActividades::getTodasLasJornadas(int idActividad)
{
    bool res=false;

    if(_tablaActividades) {
        int nAct=_tablaActividades->rowCount();
        bool found=false;
        for(int i=0;i<nAct && !found;i++) {
            if(_tablaActividades->record(i).value("idActividad").toInt()==idActividad) {
                found=true;
                res=_tablaActividades->record(i).value("todaJornada").toBool();
            }
        }
    }

    return res;
}

void MyModeloActividades::cambiarTipoActividades(QString tipoOrigen, QString tipoDestino)
{
    if(_tablaActividades) {
        int nAct=_tablaActividades->rowCount();
        for(int i=0;i<nAct;i++) {
            if(_tablaActividades->record(i).value(tr("tActividad")).toString()==tipoOrigen) {
                QModelIndex indice=_tablaActividades->index(i,15);
                _tablaActividades->setData(indice,QVariant(tipoDestino));
            }
        }
        _tablaActividades->submitAll();
        _tablaActividades->select();
        emit dataChanged(index(0,15),index(nAct,15));
    }
}

void MyModeloActividades::cambiarLocalizaciones(QString lugarOrigen,QString lugarDestino)
{
    if(_tablaActividades) {
        int nAct=_tablaActividades->rowCount();
        for(int i=0;i<nAct;i++) {
            if(_tablaActividades->record(i).value(tr("lugar")).toString()==lugarOrigen) {
                QModelIndex indice=_tablaActividades->index(i,19);
                _tablaActividades->setData(indice,QVariant(lugarDestino));
            }
        }
        _tablaActividades->submitAll();
        _tablaActividades->select();
        emit dataChanged(index(0,19),index(nAct,19));
    }
}

void MyModeloActividades::reloadLugares()
{
    if(_tablaActividades) {
        _tablaActividades->select();
        int nAct=_tablaActividades->rowCount();
        emit dataChanged(index(0,19),index(nAct-1,19));
    }
}

void MyModeloActividades::reloadTipos()
{
    if(_tablaActividades) {
        _tablaActividades->select();
        int nAct=_tablaActividades->rowCount();
        emit dataChanged(index(0,19),index(nAct-1,19));
    }
}

QString MyModeloActividades::getOrganizador(int idActividad)
{
    QString res;
    if(_tablaActividades) {
        int numActividades=_tablaActividades->rowCount();

        bool found=false;
        for(int i=0;i<numActividades && !found;i++) {
            QSqlRecord actividad=_tablaActividades->record(i);

            if(actividad.value("idActividad").toInt()==idActividad) {
                found=true;
                res=actividad.value("responsable").toString();
            }
        }
    }

    return res;
}
