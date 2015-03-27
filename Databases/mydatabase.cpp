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

#include "mydatabase.h"

MyDatabase *MyDatabase::_instancia=0;
QString MyDatabase::_insertCategorias=QString("INSERT INTO %1.Categoria(nombre,precio,maximo,fechaInicio,fechaLimite) "
                                              "VALUES(?,?,?,?,?)");
QString MyDatabase::_insertFechas=QString("INSERT INTO %1.fechasPermitidas(fechaEvento) VALUES (?)");

MyDatabase::MyDatabase(QObject *parent) :
    AcolitoDatabase(parent),
    _db(0)
{
    _gModelosAsistentes=new MyAsistenteModels(this);
    _gModelosCategorias=new MyCategoriaModels(this);
    _gModelosHabitaciones=new MyHabitacionModels(this);
    _gModelosActividades=new MyActividadModels(this);
    _gModelosMensajes=new MyMensajesModels(this);
    _modeloFechas=0;

    connect(_gModelosAsistentes,SIGNAL(borradaPlaza(QString)),_gModelosCategorias,SLOT(updateCategoria(QString)));
    connect(_gModelosAsistentes,SIGNAL(nuevaPlaza(QString)),_gModelosCategorias,SLOT(updateCategoria(QString)));

    connect(_gModelosCategorias,SIGNAL(cambiosCategoria()),_gModelosAsistentes,SLOT(recargarDatosCategorias()));
    connect(_gModelosAsistentes,SIGNAL(cambiosGrupo()),_gModelosHabitaciones,SLOT(reloadModeloAlojamientos()));
    connect(_gModelosHabitaciones,SIGNAL(reloadAlojamientos()),this,SIGNAL(reloadAlojamiento()));

}


MyDatabase *MyDatabase::getInstance(QObject *parent)
{
    if(!_instancia)
        _instancia=new MyDatabase(parent);

    return _instancia;
}

AcolitoDB *MyDatabase::getAcolitoDatabase()
{
    return _db;
}

void MyDatabase::setAcolitoDatabase(AcolitoDB *db)
{
    MyAcolitoDB *aux=qobject_cast<MyAcolitoDB*>(db);
    if(_db) {
        this->disconnect(SLOT(desconectar()));
    }
    _db=aux;
    connect(_db,SIGNAL(aboutToDisconnect()),this,SLOT(desconectar()));
}

void MyDatabase::crearDatabase(Evento *ev, QStandardItemModel *categorias)
{
    //ADVERTENCIA: Antes de lanzar las sentencias, hay que introducir el nombre
    // del esquema en ellas mediante el metodo arg() del QString

    //No se puede utilizar el esquema de MyAcolitoDB porque es necesario modificar
    //el script con el nombre del evento

    QSqlDatabase db=MyEventoDB::getInstance()->getConexion()->database();

    SQLScript script(":/scripts/scriptDatabase");

    QStringList scriptPreProcesado=script.getScriptProcesado();
    {
        db.transaction();
        QSqlQuery query(db);
        foreach(QString comando,scriptPreProcesado) {
            QString sentencia=comando.arg(ev->database());
            if(!query.exec(sentencia)) {
                qDebug() << "ERROR en MyDatabase->crearDatabase(base): " << query.lastError().text();
                db.rollback();
                return;
            }
        }

        //Incluir los datos de categorias
        //Preparar listas en batch
        //ORDEN: 0- nombre 1-entrada 2-salida 3-precio 4-plazas
        QVariantList nombresCategorias,preciosCategorias,plazasCategorias,entradas,salidas;
        int numCategorias=categorias->rowCount();
        for(int i=0;i<numCategorias;i++) {
            nombresCategorias << categorias->item(i,0)->data(Qt::EditRole);
            preciosCategorias << categorias->item(i,3)->data(Qt::EditRole);
            plazasCategorias << categorias->item(i,4)->data(Qt::EditRole);

            //Es necesario transformar las fechas de QString a QDate
            QDate fechaEntrada=QDate::fromString(categorias->item(i,1)->data(Qt::EditRole).toString()
                                                 , tr("dd 'de' MMMM,yyyy"));
            QDate fechaSalida=QDate::fromString(categorias->item(i,2)->data(Qt::EditRole).toString()
                                                , tr("dd 'de' MMMM,yyyy"));

            entradas << fechaEntrada;
            salidas << fechaSalida;
        }

        QString sentencia=MyDatabase::_insertCategorias.arg(ev->database());
        query.prepare(sentencia);
        query.addBindValue(nombresCategorias);
        query.addBindValue(preciosCategorias);
        query.addBindValue(plazasCategorias);
        query.addBindValue(entradas);
        query.addBindValue(salidas);

        if(!query.execBatch()) {
            qDebug() << "ERROR en MyDatabase->crearDatabase(): " << query.lastError().text();
            db.rollback();
            return;
        }
        db.commit();

        //Incluir los datos de fechas
        sentencia=MyDatabase::_insertFechas.arg(ev->database());
        query.prepare(sentencia);

        QVariantList listaFechas;
        QList<QDate> fechas=ev->getFechas();
        foreach(QDate fecha,fechas)
            listaFechas << fecha;

        query.addBindValue(listaFechas);

        if(!query.execBatch()) {
            qDebug() << "ERROR en MyDatabase->crearDatabase(): " << query.lastError().text();
            db.rollback();
            return;
        }
        db.commit();

        //Triggers y procedimientos
        //@todo INCOMPLETO

        db.transaction();

        SQLScript sProcedures(":/scripts/scriptProcedures");

        QStringList proceduresPreProcesado=sProcedures.getScriptProcesado();

        foreach(QString procedure,proceduresPreProcesado) {
            QString sentencia=procedure.arg(ev->database());
            if(!query.exec(sentencia)) {
                qDebug() << "ERROR en MyDatabase->crearDatabase(procedures): " << query.lastError().text();
                db.rollback();
                return;
            }
        }

        SQLScript sTriggers(":/scripts/scriptTriggers");
        QStringList triggersPreProcesado=sTriggers.getScriptProcesado();

        foreach(QString trigger,triggersPreProcesado) {
            QString sentencia=trigger.arg(ev->database());
            if(!query.exec(sentencia)) {
                qDebug() << "ERROR en MyDatabase->crearDatabase(triggers): " << query.lastError().text();
                db.rollback();
                return;
            }
        }

        db.commit();
    }

}

void MyDatabase::desconectar()
{
    //Destruir los modelos del database
    if(_gModelosAsistentes->isActive())
        _gModelosAsistentes->deleteModelos();

    if(_gModelosCategorias->isActive())
        _gModelosCategorias->deleteModelos();

    if(_gModelosHabitaciones->isActive())
        _gModelosHabitaciones->deleteModelos();

    if(_gModelosActividades->isActive())
        _gModelosActividades->deleteModelos();

    if(_gModelosMensajes->isActive())
        _gModelosMensajes->deleteModelos();

    if(_modeloFechas) {
        delete _modeloFechas;
        _modeloFechas=0;
    }

    //Desconectar la base de datos actual
    _db->disconnect();
}

void MyDatabase::conectar()
{
    //Crear generadores de modelos
    if(_gModelosAsistentes->isActive())
        _gModelosAsistentes->deleteModelos();

    _gModelosAsistentes->setAcolitoDB(_db);
    _gModelosAsistentes->loadModelos();

    if(_gModelosCategorias->isActive())
        _gModelosCategorias->deleteModelos();

    _gModelosCategorias->setAcolitoDB(_db);
    _gModelosCategorias->loadModelos();

    if(_gModelosHabitaciones->isActive())
        _gModelosHabitaciones->deleteModelos();

    _gModelosHabitaciones->setAcolitoDB(_db);
    _gModelosHabitaciones->loadModelos();

    if(_gModelosActividades->isActive())
        _gModelosActividades->deleteModelos();

    _gModelosActividades->setAcolitoDB(_db);
    _gModelosActividades->loadModelos();

    if(_gModelosMensajes->isActive())
        _gModelosMensajes->deleteModelos();

    _gModelosMensajes->setAcolitoDB(_db);
    if(_gModelosMensajes->existeTablaMensajes())
        _gModelosMensajes->loadModelos();

    //Crear modelos

    //Solicitar recarga
    emit nuevaDatabase();
}

QSortFilterProxyModel *MyDatabase::getModeloCategorias()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosCategorias)
        res=_gModelosCategorias->getModeloCategorias();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloAsistentes()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosAsistentes)
        res=_gModelosAsistentes->getModeloAsistentes();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloHabitaciones()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosHabitaciones)
        res=_gModelosHabitaciones->getModeloHabitaciones();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloAlojamientos()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosHabitaciones)
        res=_gModelosHabitaciones->getModeloAlojamientos();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloTiposHabitaciones()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosHabitaciones)
        res=_gModelosHabitaciones->getModeloTipoHabitaciones();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloActividades()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosActividades)
        res=_gModelosActividades->getModeloActividades();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloLocalizaciones()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosActividades)
        res=_gModelosActividades->getModeloLocalizaciones();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloTiposActividad()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosActividades)
        res=_gModelosActividades->getModeloTiposActividad();

    return res;
}

QSortFilterProxyModel *MyDatabase::getModeloMensajes()
{
    QSortFilterProxyModel *res=0;
    if(_gModelosMensajes)
        res=_gModelosMensajes->getModeloMensajes();

    return res;
}

void MyDatabase::addNuevoAsistente(Asistente *asis)
{
    if(_gModelosAsistentes)
        _gModelosAsistentes->addAsistente(asis);
}

void MyDatabase::addGrupoAsistentes(QList<Asistente *> listaAsis)
{
    if(_gModelosAsistentes) {
        foreach(Asistente *asis,listaAsis)
            _gModelosAsistentes->addAsistente(asis);

        if(listaAsis.count()>1)
            _gModelosAsistentes->agruparAsistentes(listaAsis);
    }
}

void MyDatabase::eliminarAsistentes(QList<Asistente *> listaAsis)
{
    //TODO: Implementar
    if(_gModelosAsistentes) {
        foreach(Asistente *asis,listaAsis)
            _gModelosAsistentes->eliminarAsistente(asis);

        //Recargar modelos que dependan de este
    }
}

void MyDatabase::agruparAsistentes(QList<Asistente *> listaAsis)
{
    if(_gModelosAsistentes) {
        _gModelosAsistentes->agruparAsistentes(listaAsis);
    }
}

void MyDatabase::nuevoGrupoAsistentes(QList<Asistente*> listaAsis)
{
    if(_gModelosAsistentes) {
        _gModelosAsistentes->nuevoGrupoAsistentes(listaAsis);
    }
}

QIcon MyDatabase::getAsistentesOkIcon() const
{
    QIcon res;
    if(_gModelosAsistentes)
        res=_gModelosAsistentes->getIconoOk();

    return res;
}

QIcon MyDatabase::getAsistentesFalseIcon() const
{
    QIcon res;
    if(_gModelosAsistentes)
        res=_gModelosAsistentes->getIconoFalse();

    return res;
}

void MyDatabase::addNuevaHabitacion(Habitacion *habitacion)
{
    if(_gModelosHabitaciones)
        _gModelosHabitaciones->nuevaHabitacion(habitacion);
}

void MyDatabase::addNuevoTipoHabitacion(QVariantMap datosTipo)
{
    if(_gModelosHabitaciones)
        _gModelosHabitaciones->nuevoTipoHabitacion(datosTipo);
}

void MyDatabase::borrarTipoHabitacion(QString tipo, QString tipoSubst)
{
    if(_gModelosHabitaciones)
        _gModelosHabitaciones->borraTipoHabitacion(tipo,tipoSubst);
}

void MyDatabase::alojarGrupo(Asistente *asis, Habitacion *hab)
{
    if(_gModelosHabitaciones) {
        _gModelosHabitaciones->alojarGrupoHabitacion(asis->idGrupo,hab->codigo);
    }
}

void MyDatabase::realojarAsistente(Asistente *asis, Habitacion *hab)
{
    if(_gModelosHabitaciones && _gModelosAsistentes) {
        int idGrupo=_gModelosHabitaciones->grupoAlojadoEn(hab->codigo).toInt();
        _gModelosAsistentes->cambiarAsistenteDeGrupo(asis,idGrupo);
        _gModelosHabitaciones->reloadModeloAlojamientos();
    }
}

void MyDatabase::separarAsistente(Asistente *asis)
{
    if(_gModelosAsistentes) {
        _gModelosAsistentes->separarAsistente(asis);
        _gModelosHabitaciones->reloadModeloAlojamientos();
    }
}

void MyDatabase::intercambiarHabitaciones(Asistente *asistenteA, Asistente *asistenteB)
{
    if(_gModelosHabitaciones) {
        _gModelosHabitaciones->intercambiarHabitacionesGrupos(asistenteA->idGrupo,asistenteB->idGrupo);
    }
}

void MyDatabase::desalojarHabitacion(Habitacion *hab)
{
    if(_gModelosHabitaciones) {
        _gModelosHabitaciones->desalojarHabitacion(hab->codigo);
    }
}

bool MyDatabase::estaAbierta(QString codHab)
{
    if(codHab==tr("Sin asignar"))
        return false;

    if(_gModelosHabitaciones) {
        Habitacion *hab=new Habitacion(this);
        hab->codigo=codHab;

        return _gModelosHabitaciones->isOpen(hab);
    }
    else return false;
}

void MyDatabase::abrirHabitacion(QString codHab, bool estado)
{
    if(_gModelosHabitaciones) {
        Habitacion *hab=new Habitacion(this);
        hab->codigo=codHab;
        hab->llaveEntregada=estado;

        _gModelosHabitaciones->setLlaveHabitacion(hab);
    }
}

void MyDatabase::crearCategoriaAsistentes(QVariantMap datosCategoria)
{
    if(_gModelosCategorias) {
        _gModelosCategorias->crearNuevaCategoria(datosCategoria);
    }
}

void MyDatabase::borrarCategoriaAsistentes(QString categoria, QString sustituta)
{
    if(_gModelosCategorias) {
        _gModelosCategorias->borrarCategoria(categoria,sustituta);
    }
}

QSortFilterProxyModel *MyDatabase::getModeloFechas()
{
    if(!_modeloFechas) {
        _modeloFechas=new QSqlQueryModel(this);

        _modeloFechas->setQuery("SELECT * FROM fechaspermitidas ORDER BY fechaEvento ASC",_db->database());

        _modeloFechas->setHeaderData(0,Qt::Horizontal,tr("Fechas"));
    }

    QSortFilterProxyModel *res=new QSortFilterProxyModel(this);
    res->setSourceModel(_modeloFechas);

    return res;
}

QVariant MyDatabase::grupoAlojadoEn(QString codHabitacion)
{
    if(_gModelosHabitaciones)
        return _gModelosHabitaciones->grupoAlojadoEn(codHabitacion);
    else return QVariant();
}

QVariant MyDatabase::habitacionDelGrupo(QString idGrupo)
{
    if(_gModelosHabitaciones)
        return _gModelosHabitaciones->habitacionDelGrupo(idGrupo);
    else return QVariant();
}

void MyDatabase::activarTodasLasJornadas(int idActividad, bool modo)
{
    if(_gModelosActividades)
        _gModelosActividades->activarTodasLasJornadas(idActividad,modo);
}

bool MyDatabase::getModoTodasLasJornadas(int idActividad)
{
    if(_gModelosActividades)
        return _gModelosActividades->getModoTodasLasJornadas(idActividad);
    else return false;
}

void MyDatabase::addTipoActividad(QString tipo)
{
    if(_gModelosActividades)
        _gModelosActividades->addTipoActividad(tipo);
}

void MyDatabase::delTipoActividad(QString tipo, QString sustituto)
{
    if(_gModelosActividades)
        _gModelosActividades->delTipoActividad(tipo,sustituto);
}

void MyDatabase::addLocalizacion(QString lugar)
{
    if(_gModelosActividades)
        _gModelosActividades->addLocalizacion(lugar);
}

void MyDatabase::delLocalizacion(QString lugar, QString sustituto)
{
    if(_gModelosActividades)
        _gModelosActividades->delLocalizacion(lugar,sustituto);
}

void MyDatabase::addActividad(QVariantMap actividad)
{
    if(_gModelosActividades)
        _gModelosActividades->addActividad(actividad);
}

void MyDatabase::addListaActividades(QList<QVariantMap> listaActividades)
{
    if(_gModelosActividades)
        _gModelosActividades->addListaActividades(listaActividades);
}

void MyDatabase::delActividad(int idActividad)
{
    if(_gModelosActividades)
        _gModelosActividades->delActividad(idActividad);
}

QString MyDatabase::getOrganizadorActividad(int idActividad)
{
    if(_gModelosActividades)
        return _gModelosActividades->getOrganizador(idActividad);
    return QString();
}

bool MyDatabase::haPagado(QString dni)
{
    if(_gModelosAsistentes)
        return _gModelosAsistentes->getPagoAsistente(dni);
    else return 0;
}

bool MyDatabase::existeTablaMensajes() const
{
    if(_gModelosMensajes)
        return _gModelosMensajes->existeTablaMensajes();

    return false;
}

bool MyDatabase::crearTablaMensajes()
{
    if(_gModelosMensajes)
        return _gModelosMensajes->creaTablaMensajes();

    return false;
}

bool MyDatabase::nuevoMensaje(QString mensaje)
{
    if(_gModelosMensajes)
        return _gModelosMensajes->addMensaje(mensaje);

    return false;
}

bool MyDatabase::borrarMensaje(int idMensaje)
{
    if(_gModelosMensajes)
        return _gModelosMensajes->removeMensaje(idMensaje);

    return false;
}

QList<int> MyDatabase::getIdMensaje(QString texto) const
{
    /// @todo Implementar
    return QList<int>();
}
QString MyDatabase::getMensajeById(int id) const
{
    /// @todo Implementar
    return QString();
}
