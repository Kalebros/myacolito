#include "mymensajesmodels.h"

MyMensajesModels::MyMensajesModels(QObject *parent, MyAcolitoDB *db) :
    MyAcolitoModels(parent,db)
{
    _modeloBase=0;
}

bool MyMensajesModels::isActive() const
{
    if(_db && _modeloBase)
        return true;
    else return false;
}

void MyMensajesModels::deleteModelos()
{
    if(_modeloBase) {
        delete _modeloBase;
        _modeloBase=0;
    }
}

void MyMensajesModels::loadModelos()
{
    if(_db && !_modeloBase) {
        _modeloBase=new QSqlTableModel(this,_db->database());
        _modeloBase->setTable("mensajes");
        _modeloBase->setEditStrategy(QSqlTableModel::OnFieldChange);
        _modeloBase->select();
    }
}

QSortFilterProxyModel *MyMensajesModels::getModeloMensajes()
{
    QSortFilterProxyModel *res=0;

    if(_db) {
        if(!_modeloBase) {
            loadModelos();
        }
        res=new QSortFilterProxyModel(this);
        res->setSourceModel(_modeloBase);
    }

    return res;
}

bool MyMensajesModels::existeTablaMensajes() const
{
    if(!_db)
        return false;

    if(!_db->isConnected())
        return false;

    bool res=false;
    {
        QSqlQuery query(_db->database());
        QString sentenciaBusqueda="SELECT count(TABLE_NAME) FROM information_schema.tables "
                "WHERE table_schema='%1' AND table_name='mensajes'";
        sentenciaBusqueda=sentenciaBusqueda.arg(_db->getDatosConexion().getDatabaseName());

        //Aunque esto en realidad representa un error
        if(query.exec(sentenciaBusqueda))
        {
            query.next();
            if(query.value(0).toInt()==1)
                res=true;
        }
    }

    return res;
}

bool MyMensajesModels::creaTablaMensajes()
{
    if(!_db)
        return false;

    if(!_db->isConnected())
        return false;

    bool res=false;

    {
        _db->database().transaction();
        QSqlQuery query(_db->database());
        query.prepare("CREATE TABLE `mensajes` ( "
                      "`idMensaje` int(11) NOT NULL AUTO_INCREMENT, "
                      "`textoMensaje` text, "
                      "PRIMARY KEY (`idMensaje`) "
                      ") ENGINE=InnoDB DEFAULT CHARSET=latin1");

        if(query.exec()) {
            _db->database().commit();
            res=true;
        }
        else _db->database().rollback();
    }
    return res;
}

bool MyMensajesModels::addMensaje(QString mensaje)
{
    if(!_db)
        return false;

    if(!_db->isConnected())
        return false;

    if(!_modeloBase)
        return false;

    bool res=false;
    QSqlRecord record=_modeloBase->record();
    record.setGenerated(0,false);
    record.setValue(1,mensaje);
    if(_modeloBase->insertRecord(-1,record)) {
        _modeloBase->submitAll();
        _modeloBase->select();
        res=true;
    }

    return false;
}

bool MyMensajesModels::removeMensaje(int id)
{
    if(!_db)
        return false;

    if(!_db->isConnected())
        return false;

    if(!_modeloBase)
        return false;

    bool res=false;

    int numMensajes=_modeloBase->rowCount();
    bool found=false;
    for(int i=0;i<numMensajes && !found;i++) {
        if(_modeloBase->index(i,0).data().toInt()==id) {
            found=true;
            res=_modeloBase->removeRow(i);
            _modeloBase->submitAll();
            _modeloBase->select();
        }
    }

    return res;
}
