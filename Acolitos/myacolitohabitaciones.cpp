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

#include "myacolitohabitaciones.h"
#include "ui_myacolitohabitaciones.h"

MyAcolitoHabitaciones::MyAcolitoHabitaciones(QWidget *parent,MyDatabase *db) :
    MyAcolito(parent,db),
    ui(new Ui::MyAcolitoHabitaciones),
    modeloHabitaciones(0),
    modeloAsistentes(0),
    filtroHabLibre(0),
    filtroHabAbierta(0),
    filtroReservada(0),
    mapeador(0)
{
    ui->setupUi(this);
    setup();

    //Iconos por defecto para BooleanBallDelegate
    _okIcono=QIcon(":/acolitos/iconos/okIcon");
    _falseIcono=QIcon(":/acolitos/iconos/falseIcon");

    //Iconos por defecto para habitacion cerrada y abierta
    _openIcono=QIcon(":/acolitos/iconos/habAbierta");
    _closeIcono=QIcon(":/acolitos/iconos/habCerrada");

}

MyAcolitoHabitaciones::~MyAcolitoHabitaciones()
{
    delete ui;
}

void MyAcolitoHabitaciones::setOkIcono(QIcon icono)
{
    _okIcono=icono;
}

void MyAcolitoHabitaciones::setFalseIcono(QIcon icono)
{
    _falseIcono=icono;
}

void MyAcolitoHabitaciones::setHabAbiertaIcono(QIcon icono)
{
    _openIcono=icono;
}

void MyAcolitoHabitaciones::setHabCerradaIcono(QIcon icono)
{
    _closeIcono=icono;
}

void MyAcolitoHabitaciones::reloadModelos()
{
    if(modeloHabitaciones) {
        delete modeloHabitaciones;
        modeloHabitaciones=0;
    }

    if(modeloAsistentes) {
        delete modeloAsistentes;
        modeloAsistentes=0;
    }

    modeloHabitaciones=database->getModeloHabitaciones();
    modeloAsistentes=database->getModeloAsistentes();
    modeloAsistentes->setFilterKeyColumn(6);
    modeloAsistentes->setFilterFixedString(tr("-1"));

    //Busqueda por codigo
    modeloHabitaciones->setFilterKeyColumn(0);
    modeloHabitaciones->setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(ui->buscarLineEdit,SIGNAL(textChanged(QString)),modeloHabitaciones,SLOT(setFilterFixedString(QString)));

    //Preparar modelo de tipos de habitacion
    ui->tipoComboBox->setModel(database->getModeloTiposHabitaciones());
    ui->tipoComboBox->setModelColumn(0);

    //Filtros y mapeado
    setupFiltros();

    //TODO: Cambiar modelos por los modelos filtrados

    ui->vistaHabitaciones->setModel(filtroReservada);
    ui->vistaAlojados->setModel(modeloAsistentes);

    setupMapeador();
    setupVistas();
}

void MyAcolitoHabitaciones::clear()
{
    ui->vistaAlojados->setModel(0);
    ui->vistaHabitaciones->setModel(0);

    borrarFiltros();

    if(modeloHabitaciones) {
        delete modeloHabitaciones;
        modeloHabitaciones=0;
    }

    if(modeloAsistentes) {
        delete modeloAsistentes;
        modeloAsistentes=0;
    }
}

void MyAcolitoHabitaciones::showOpcionesVisualizacion(bool visualizar)
{
        ui->labelLibre->setVisible(visualizar);
        ui->labelLlaveEntregada->setVisible(visualizar);
        ui->labelOpcionesReserva->setVisible(visualizar);
        ui->siEntregadaCheckBox->setVisible(visualizar);
        ui->siLibreCheckBox->setVisible(visualizar);
        ui->siReservadaCheckBox->setVisible(visualizar);
        ui->noEntregadaCheckBox->setVisible(visualizar);
        ui->noLibreCheckBox->setVisible(visualizar);
        ui->noReservadaCheckBox->setVisible(visualizar);
}

void MyAcolitoHabitaciones::setup()
{
    ui->buscarLineEdit->setEmptyMessage(QString("Buscar codigo de habitacion..."));
    connect(ui->visualizarGroupBox,SIGNAL(toggled(bool)),this,SLOT(showOpcionesVisualizacion(bool)));

    ui->visualizarGroupBox->setChecked(false);

    ui->reservaButton->setIconoFalse(new QIcon(":/acolitos/iconos/noReserva"));
    ui->reservaButton->setIconoTrue(new QIcon(":/acolitos/iconos/reserva"));

    ui->llaveButton->setIconoFalse(new QIcon(":/acolitos/iconos/habCerrada"));
    ui->llaveButton->setIconoTrue(new QIcon(":/acolitos/iconos/habAbierta"));

    connect(ui->reservaButton,SIGNAL(clicked()),this,SLOT(reservarHabitacion()));
    connect(ui->llaveButton,SIGNAL(clicked()),this,SLOT(entregaLlave()));
    connect(ui->reservaButton,SIGNAL(changedValorActual(int)),this,SLOT(mostrarWidgetsReserva(int)));
    connect(ui->limpiarButton,SIGNAL(clicked()),this,SLOT(desalojarHabitacion()));
    connect(ui->imprimirButton,SIGNAL(clicked()),this,SLOT(imprimirReporteHabitacion()));
    connect(ui->reporteButton,SIGNAL(clicked()),this,SLOT(reporteHabitacion()));

    ui->reservaLabel->setVisible(false);
    ui->reservaBrowser->setVisible(false);
}

void MyAcolitoHabitaciones::setupMapeador()
{
    if(mapeador) {
        delete mapeador;
        mapeador=0;
    }

    mapeador=new QDataWidgetMapper(this);

    //TODO: conexiones del mapeador
    mapeador->setModel(filtroReservada);

    mapeador->addMapping(ui->codHabLine,0,"text");
    mapeador->addMapping(ui->tipoComboBox,1,"texto");
    mapeador->addMapping(ui->comentariosBrowser,2,"plainText");
    mapeador->addMapping(ui->llaveButton,3,"estadoBinario");
    mapeador->addMapping(ui->reservaButton,4,"estadoBinario");
    mapeador->addMapping(ui->reservaBrowser,5,"plainText");

    mapeador->toFirst();

    connect(ui->vistaHabitaciones->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->vistaHabitaciones->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(filtrarGrupoHabitacion(QModelIndex,QModelIndex)));
}

void MyAcolitoHabitaciones::setupFiltros()
{
    borrarFiltros();

    //Crear filtros
    filtroHabLibre=new QSortFilterProxyModel(this);
    filtroHabLibre->setSourceModel(modeloHabitaciones);
    filtroHabLibre->setFilterKeyColumn(6);

    filtroHabAbierta=new QSortFilterProxyModel(this);
    filtroHabAbierta->setSourceModel(filtroHabLibre);
    filtroHabAbierta->setFilterKeyColumn(3);

    filtroReservada=new QSortFilterProxyModel(this);
    filtroReservada->setSourceModel(filtroHabAbierta);
    filtroReservada->setFilterKeyColumn(4);

    //Preparar conexiones de filtrado
    connect(ui->siEntregadaCheckBox,SIGNAL(stateChanged(int)),this,SLOT(filtrarHabAbiertas(int)));
    connect(ui->noEntregadaCheckBox,SIGNAL(stateChanged(int)),this,SLOT(filtrarHabAbiertas(int)));

    connect(ui->siLibreCheckBox,SIGNAL(stateChanged(int)),this,SLOT(filtrarHabLibres(int)));
    connect(ui->noLibreCheckBox,SIGNAL(stateChanged(int)),this,SLOT(filtrarHabLibres(int)));

    connect(ui->siReservadaCheckBox,SIGNAL(stateChanged(int)),this,SLOT(filtrarHabReservadas(int)));
    connect(ui->noReservadaCheckBox,SIGNAL(stateChanged(int)),this,SLOT(filtrarHabReservadas(int)));
}

void MyAcolitoHabitaciones::setupVistas()
{
    ui->vistaHabitaciones->setItemDelegateForColumn(1,new TiposHabDelegate(this,database));
    ui->vistaHabitaciones->setItemDelegateForColumn(3,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/habAbierta"),QIcon(":/acolitos/iconos/habCerrada")));
    ui->vistaHabitaciones->setItemDelegateForColumn(4,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
    ui->vistaHabitaciones->setItemDelegateForColumn(6,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));

    ui->vistaHabitaciones->setColumnHidden(2,true);
    ui->vistaHabitaciones->setColumnHidden(5,true);

    //Vista alojados
    ui->vistaAlojados->setColumnHidden(0,true);
    ui->vistaAlojados->setColumnHidden(4,true);
    ui->vistaAlojados->setColumnHidden(5,true);
    ui->vistaAlojados->setColumnHidden(6,true);
    ui->vistaAlojados->setColumnHidden(8,true);
    ui->vistaAlojados->setColumnHidden(11,true);

    ui->vistaAlojados->setItemDelegateForColumn(7,new CategoriasDelegate(this,database));

    ui->vistaAlojados->setItemDelegateForColumn(9,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
    ui->vistaAlojados->setItemDelegateForColumn(10,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
    ui->vistaAlojados->setItemDelegateForColumn(11,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
}

void MyAcolitoHabitaciones::borrarFiltros()
{
    if(filtroHabLibre) {
        disconnect(filtroHabLibre);
        delete filtroHabLibre;
        filtroHabLibre=0;
    }

    if(filtroHabAbierta) {
        disconnect(filtroHabAbierta);
        delete filtroHabAbierta;
        filtroHabAbierta=0;
    }

    if(filtroReservada) {
        disconnect(filtroReservada);
        delete filtroReservada;
        filtroReservada=0;
    }
}

void MyAcolitoHabitaciones::filtrarHabLibres(int estado)
{
    bool siChecked,noChecked;

    siChecked=ui->siLibreCheckBox->isChecked();
    noChecked=ui->noLibreCheckBox->isChecked();

    if(filtroHabLibre) {

        if((!siChecked && !noChecked) || (siChecked && noChecked)) {
            filtroHabLibre->setFilterRegExp(tr("0|1"));
        } else if(siChecked && !noChecked) {
            filtroHabLibre->setFilterRegExp(tr("1"));
        } else if(!siChecked && noChecked) {
            filtroHabLibre->setFilterRegExp(tr("0"));
        }
    }
}

void MyAcolitoHabitaciones::filtrarHabAbiertas(int estado)
{
    bool siChecked,noChecked;

    siChecked=ui->siEntregadaCheckBox->isChecked();
    noChecked=ui->noEntregadaCheckBox->isChecked();


    if(filtroHabAbierta) {

        if((!siChecked && !noChecked) || (siChecked && noChecked)) {
            filtroHabAbierta->setFilterRegExp(tr("0|1"));
        } else if(siChecked && !noChecked) {
            filtroHabAbierta->setFilterRegExp(tr("1"));
        } else if(!siChecked && noChecked) {
            filtroHabAbierta->setFilterRegExp(tr("0"));
        }
    }
}

void MyAcolitoHabitaciones::filtrarHabReservadas(int estado)
{
    bool siChecked,noChecked;

    siChecked=ui->siReservadaCheckBox->isChecked();
    noChecked=ui->noReservadaCheckBox->isChecked();


    if(filtroReservada) {

        if((!siChecked && !noChecked) || (siChecked && noChecked)) {
            filtroReservada->setFilterRegExp(tr("0|1"));
        } else if(siChecked && !noChecked) {
            filtroReservada->setFilterRegExp(tr("1"));
        } else if(!siChecked && noChecked) {
            filtroReservada->setFilterRegExp(tr("0"));
        }
    }
}

void MyAcolitoHabitaciones::reservarHabitacion()
{
    int numRegistrosVisibles=filtroReservada->rowCount();

    bool found=false;
    QModelIndex indiceActual;
    for(int i=0;i<numRegistrosVisibles && !found;i++) {
        if(filtroReservada->index(i,0).data(Qt::EditRole).toString()==ui->codHabLine->text()) {
            found=true;
            indiceActual=filtroReservada->index(i,4); ///Indice del dato activado
            int estadoActual=ui->reservaButton->getEstado();
            if(!estadoActual)
                filtroReservada->setData(filtroReservada->index(indiceActual.row(),5),tr("Escribe el concepto de reserva"),Qt::EditRole);
            filtroReservada->setData(indiceActual,QVariant(estadoActual),Qt::EditRole);
            mostrarWidgetsReserva(estadoActual);
        }
    }
}

void MyAcolitoHabitaciones::entregaLlave()
{
    int numRegistrosVisibles=filtroReservada->rowCount();

    bool found=false;
    QModelIndex indiceActual;
    for(int i=0;i<numRegistrosVisibles && !found;i++) {
        if(filtroReservada->index(i,0).data(Qt::EditRole).toString()==ui->codHabLine->text()) {
            found=true;
            indiceActual=filtroReservada->index(i,3); ///Indice del dato activado
            int estadoActual=ui->llaveButton->getEstado();
            filtroReservada->setData(indiceActual,QVariant(estadoActual),Qt::EditRole);
        }
    }

}

void MyAcolitoHabitaciones::mostrarWidgetsReserva(int estado)
{
    ui->reservaLabel->setVisible(estado);
    ui->reservaBrowser->setVisible(estado);
}

void MyAcolitoHabitaciones::desalojarHabitacion()
{
    if(database) {
        Habitacion *hab=new Habitacion(this);

        hab->codigo=ui->codHabLine->text();

        database->desalojarHabitacion(hab);
        reloadFiltroHabitaciones();
    }
}

void MyAcolitoHabitaciones::dialogoNuevaHabitacion()
{
    AddHabitacionDialog dialogo(this,database);

    dialogo.exec();
}

void MyAcolitoHabitaciones::dialogoNuevaListaHabitaciones()
{
    AddListaHabitacionesDialog dialogo(this,database);
    dialogo.exec();
}

void MyAcolitoHabitaciones::dialogoTiposHabitacion()
{
    HabitacionTiposDialog dialogo(this,database);
    dialogo.exec();
}

void MyAcolitoHabitaciones::filtrarGrupoHabitacion(QModelIndex indiceA, QModelIndex indiceB)
{
    QString habitacion=filtroReservada->index(indiceA.row(),0).data(Qt::EditRole).toString();

    QVariant grupo=database->grupoAlojadoEn(habitacion);

    if(grupo.isNull() || !grupo.isValid())
        modeloAsistentes->setFilterFixedString(tr("-1"));

    else {
        QString filtro;
        QString id=grupo.toString();
        for(int i=0;i<id.count();i++) {
            filtro.append(id.at(i));
            filtro.append("{1,1}");
        }
        filtro.prepend('^');
        filtro.append("$");

        modeloAsistentes->setFilterRegExp(filtro);
    }
}

void MyAcolitoHabitaciones::reloadFiltroHabitaciones()
{
    QString habitacion=ui->codHabLine->text();

    QVariant grupo=database->grupoAlojadoEn(habitacion);

    if(grupo.isNull() || !grupo.isValid())
        modeloAsistentes->setFilterFixedString(tr("-1"));

    else {
        QString filtro;
        QString id=grupo.toString();
        for(int i=0;i<id.count();i++) {
            filtro.append(id.at(i));
            filtro.append("{1,1}");
        }
        filtro.prepend('^');
        filtro.append("$");

        modeloAsistentes->setFilterRegExp(filtro);
    }
}

void MyAcolitoHabitaciones::reporteHabitacion()
{
    QVariantMap datosHabitacion=generarDatosHabitacion();

    emit solicitarReporteHabitacion("FactoriaHabitaciones::HabitacionReport",datosHabitacion,ShowOnDialog);
}

void MyAcolitoHabitaciones::imprimirReporteHabitacion()
{
    QVariantMap datosHabitacion=generarDatosHabitacion();

    emit solicitarReporteHabitacion("FactoriaHabitaciones::HabitacionReport",datosHabitacion,Print);
}

QVariantMap MyAcolitoHabitaciones::generarDatosHabitacion()
{
    QVariantMap datosHabitacion;

    if(database) {
        datosHabitacion.insert("codigo",ui->codHabLine->text());
        datosHabitacion.insert("tipo",ui->tipoComboBox->currentText());
        datosHabitacion.insert("comentarios",ui->comentariosBrowser->toPlainText());
        if(ui->reservaLabel->isVisible()) {
            datosHabitacion.insert("reservada",true);
            datosHabitacion.insert("cReserva",ui->reservaBrowser->toPlainText());
        }
        else datosHabitacion.insert("reservada",false);
        QVariant grupo=database->grupoAlojadoEn(ui->codHabLine->text());
        if(grupo.isNull() || !grupo.isValid())
            datosHabitacion.insert("libre",QVariant(1));
        else datosHabitacion.insert("libre",QVariant(0));
        datosHabitacion.insert("abierta",database->estaAbierta(ui->codHabLine->text()));

        if(grupo.isValid()) {
            QSortFilterProxyModel *modeloAsistentes=database->getModeloAsistentes();
            modeloAsistentes->sort(1,Qt::AscendingOrder);
            modeloAsistentes->setFilterKeyColumn(6);    //Filtrar por IDgrupo, consultar en Databases/mymodeloasistentes
            modeloAsistentes->setFilterFixedString(grupo.toString());

            int numAsistentes=modeloAsistentes->rowCount();
            QList<QVariant> listaAlojados;
            for(int i=0;i<numAsistentes;i++) {
                QVariantMap asistente;
                asistente.insert("nombre",modeloAsistentes->index(i,1).data(Qt::EditRole));
                asistente.insert("DNI",modeloAsistentes->index(i,2).data(Qt::EditRole));
                asistente.insert("correo",modeloAsistentes->index(i,3).data(Qt::EditRole));
                asistente.insert("tipo",modeloAsistentes->index(i,7).data(Qt::EditRole));
                asistente.insert("pago",modeloAsistentes->index(i,9).data(Qt::EditRole));
                listaAlojados.append(asistente);
            }
            if(!listaAlojados.isEmpty())
                datosHabitacion.insert("alojados",listaAlojados);
        }
    }

    return datosHabitacion;
}
