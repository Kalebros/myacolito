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

#include "myacolitoactividades.h"
#include "ui_myacolitoactividades.h"

MyAcolitoActividades::MyAcolitoActividades(QWidget *parent, MyDatabase *db) :
    MyAcolito(parent,db),
    ui(new Ui::MyAcolitoActividades),
    modeloActividades(0),
    mapeador(0),
    filtroTipos(0),
    filtroFechas(0)
{
    ui->setupUi(this);

    qRegisterMetaType<ReportOptions>("ReportOptions");

    setup();
}

MyAcolitoActividades::~MyAcolitoActividades()
{
    delete ui;
}

void MyAcolitoActividades::setup()
{
    ui->searchLine->setEmptyMessage(tr("Buscar actividades por nombre..."));
    ui->labelTodasJornadas->setVisible(false);

    //Preparar boton de todas las jornadas

    ui->todaJornadaButton->setIconoFalse(new QIcon(":/acolitos/iconos/actDiaria"));
    ui->todaJornadaButton->setIconoTrue(new QIcon(":/acolitos/iconos/actTodasJornadas"));

    connect(ui->todaJornadaButton,SIGNAL(clicked()),this,SLOT(setModoTodoJornadas()));
    connect(ui->todaJornadaButton,SIGNAL(changedValorActual(int)),this,SLOT(actualizarWidgetHorarios(int)));
    connect(ui->vistaButton,SIGNAL(clicked()),this,SLOT(dialogoVistaCompleta()));

    connect(ui->imprimirButton,SIGNAL(clicked()),this,SLOT(solicitarImpresion()));
    connect(ui->exportButton,SIGNAL(clicked()),this,SLOT(solicitarImportarDialogo()));

    QAction *delAction=new QAction(this);
    delAction->setText(tr("Borra esta actividad"));

    connect(delAction,SIGNAL(triggered()),this,SLOT(deleteActividad()));

    ui->vistaActividades->addAction(delAction);
    ui->vistaActividades->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MyAcolitoActividades::reloadModelos()
{
    if(modeloActividades) {
        delete modeloActividades;
        modeloActividades=0;
    }

    if(filtroTipos) {
        delete filtroTipos;
        filtroTipos=0;
    }

    if(filtroFechas) {
        delete filtroFechas;
        filtroFechas=0;
    }

    //Preparar comboBox
    ui->lugarComboBox->setModel(database->getModeloLocalizaciones());
    ui->tipoComboBox->setModel(database->getModeloTiposActividad());
    ui->diaComboBox->setModel(database->getModeloFechas());

    modeloActividades=database->getModeloActividades();

    //Prepara filtros
    setupFiltros();

    //Prepara vista
    setupVista();

    //Prepara mapedor de datos
    setupMapeador();

}

void MyAcolitoActividades::setupMapeador()
{
    if(mapeador) {
        mapeador->disconnect();

        delete mapeador;
        mapeador=0;
    }

    mapeador=new QDataWidgetMapper(this);

    //Cambiar el modelo para adecuarlo a los filtros
    mapeador->setModel(filtroFechas);

    mapeador->addMapping(ui->idLabel,0,"text");
    mapeador->addMapping(ui->actividadLine,1,"text");
    mapeador->addMapping(ui->lugarComboBox,19,"texto");
    mapeador->addMapping(ui->tipoComboBox,15,"texto");
    mapeador->addMapping(ui->diaComboBox,9,"texto");
    mapeador->addMapping(ui->descBrowser,16,"plainText");
    mapeador->addMapping(ui->reqBrowser,17,"plainText");
    mapeador->addMapping(ui->minimoSpinBox,13,"value");
    mapeador->addMapping(ui->maximoSpinBox,14,"value");
    mapeador->addMapping(ui->finTimeEdit,12,"time");
    mapeador->addMapping(ui->inicioTimeEdit,11,"time");
    mapeador->addMapping(ui->todaJornadaButton,10,"modoBinario");

    mapeador->toFirst();

    connect(ui->vistaActividades->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));
}

void MyAcolitoActividades::setupVista()
{
    ui->vistaActividades->setModel(filtroFechas);

    //Ajuste de version
#if QT_VERSION >=0x050000
    ui->vistaActividades->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
#else
    ui->vistaActividades->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
#endif
    ui->vistaActividades->setColumnHidden(0,true);
    ui->vistaActividades->setColumnHidden(2,true);
    ui->vistaActividades->setColumnHidden(3,true);
    ui->vistaActividades->setColumnHidden(4,true);
    ui->vistaActividades->setColumnHidden(5,true);
    ui->vistaActividades->setColumnHidden(6,true);
    ui->vistaActividades->setColumnHidden(7,true);
    ui->vistaActividades->setColumnHidden(8,true);
    ui->vistaActividades->setColumnHidden(10,true);
    ui->vistaActividades->setColumnHidden(11,true);
    ui->vistaActividades->setColumnHidden(12,true);
    ui->vistaActividades->setColumnHidden(13,true);
    ui->vistaActividades->setColumnHidden(14,true);
    ui->vistaActividades->setColumnHidden(16,true);
    ui->vistaActividades->setColumnHidden(17,true);
    ui->vistaActividades->setColumnHidden(18,true);
    ui->vistaActividades->setColumnHidden(19,true);


    //Delegados
    ui->vistaActividades->setItemDelegateForColumn(9,new FechasDelegate(this,database));
    ui->vistaActividades->setItemDelegateForColumn(15,new TipoDelegate(this,database));
}

void MyAcolitoActividades::setupFiltros()
{
    //TODO: implementar
    //Nombre: en modelo base
    modeloActividades->setFilterKeyColumn(1);
    modeloActividades->setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(ui->searchLine,SIGNAL(textChanged(QString)),modeloActividades,SLOT(setFilterFixedString(QString)));

    //Filtro de tipos
    if(filtroTipos) {
        delete filtroTipos;
        filtroTipos=0;
    }
    filtroTipos=new QSortFilterProxyModel(this);
    filtroTipos->setSourceModel(modeloActividades);

    filtroTipos->setFilterKeyColumn(15);

    loadTipoComboBox();

    //Filtro de fechas

    if(filtroFechas) {
        delete filtroFechas;
        filtroFechas=0;
    }

    filtroFechas=new QSortFilterProxyModel(this);
    filtroFechas->setSourceModel(filtroTipos);

    filtroFechas->setFilterKeyColumn(9);

    loadFechasComboBox();
}

void MyAcolitoActividades::clear()
{
    if(modeloActividades) {
        delete modeloActividades;
        modeloActividades=0;
    }

    if(filtroFechas) {
        delete filtroFechas;
        filtroFechas=0;
    }

    if(filtroTipos) {
        delete filtroTipos;
        filtroTipos=0;
    }
}

void MyAcolitoActividades::dialogoTiposActividad()
{
    TipoActividadDialog dialogo(this,database);

    dialogo.exec();
}

void MyAcolitoActividades::dialogoLocalizaciones()
{
    LocalizacionesDialog dialogo(this,database);

    dialogo.exec();
}

void MyAcolitoActividades::dialogoNuevaActividad()
{
    AddActividadDialog dialogo(this,database);

    dialogo.exec();
}

void MyAcolitoActividades::setModoTodoJornadas()
{
    if(ui->todaJornadaButton->getEstado()==1)
        database->activarTodasLasJornadas(ui->idLabel->text().toInt(),false);
    else database->activarTodasLasJornadas(ui->idLabel->text().toInt(),true);
}

void MyAcolitoActividades::actualizarWidgetHorarios(int modo)
{
    if(modo) {
        //Activado todas las jornadas, ocultar dias y horas y mostrar label
        ui->labelDia->setVisible(false);
        ui->diaComboBox->setVisible(false);
        ui->labelInicio->setVisible(false);
        ui->inicioTimeEdit->setVisible(false);
        ui->labelFin->setVisible(false);
        ui->finTimeEdit->setVisible(false);

        ui->labelTodasJornadas->setVisible(true);
    }
    else {
        ui->labelTodasJornadas->setVisible(false);
        ui->labelDia->setVisible(true);
        ui->diaComboBox->setVisible(true);
        ui->labelInicio->setVisible(true);
        ui->inicioTimeEdit->setVisible(true);
        ui->finTimeEdit->setVisible(true);
        ui->labelFin->setVisible(true);
    }
}

void MyAcolitoActividades::filtrarPorTipos(QString tipo)
{
    if(filtroTipos) {
        if(tipo==QString("Todos los tipos"))
            filtroTipos->setFilterFixedString(QString());

        else filtroTipos->setFilterFixedString(tipo);
    }
}

void MyAcolitoActividades::filtrarPorFechas(QString fecha)
{
    if(filtroFechas) {
        if(fecha==QString("Todos los dias"))
            filtroFechas->setFilterFixedString(QString());
        else filtroFechas->setFilterFixedString(fecha);
    }
}

void MyAcolitoActividades::loadTipoComboBox()
{
    if(filtroTipos) {
        ui->filtroTipoBox->disconnect();

        QAbstractItemModel *modelo=ui->tipoComboBox->model();

        ui->filtroTipoBox->clear();
        ui->filtroTipoBox->addItem(tr("Todos los tipos"));

        int nElementos=modelo->rowCount();
        for(int i=0;i<nElementos;i++)
            ui->filtroTipoBox->addItem(modelo->index(i,0).data().toString());

        connect(ui->filtroTipoBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(filtrarPorTipos(QString)));
        ui->filtroTipoBox->setCurrentIndex(0);
    }
}

void MyAcolitoActividades::loadFechasComboBox()
{
    if(filtroFechas) {
        ui->filtroDiaBox->disconnect();

        QSortFilterProxyModel *modelo=database->getModeloFechas();

        ui->filtroDiaBox->clear();
        ui->filtroDiaBox->addItem(tr("Todos los dias"));
        int nElementos=modelo->rowCount();
        for(int i=0;i<nElementos;i++)
            ui->filtroDiaBox->addItem(modelo->index(i,0).data().toString());

        connect(ui->filtroDiaBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(filtrarPorFechas(QString)));
        ui->filtroDiaBox->setCurrentIndex(0);
    }
}

void MyAcolitoActividades::vigilaCambiosModoJornadas(QModelIndex indexA, QModelIndex indexB)
{
    //TODO: implementar
}

void MyAcolitoActividades::deleteActividad()
{
    QModelIndexList listaSeleccion=ui->vistaActividades->selectionModel()->selectedRows();

    if(!listaSeleccion.isEmpty()) {
        int res=QMessageBox::warning(this,tr("MyAcolito - Advertencia"),tr("Esta a punto de eliminar una actividad; esta es una operacion\nque no se puede deshacer. Esta seguro?"),QMessageBox::Yes,QMessageBox::No);
        if(res==QMessageBox::Yes) {
            QModelIndex indice=listaSeleccion.first();
            int id=ui->vistaActividades->model()->index(indice.row(),0).data().toInt();

            database->delActividad(id);
        }
    }
}

void MyAcolitoActividades::dialogoVistaCompleta()
{
    if(database) {
        int idActividad=ui->idLabel->text().toInt();

        ActCompletaDialog dialogo(this,idActividad,database);

        dialogo.exec();
    }
}

void MyAcolitoActividades::solicitarImpresion()
{
    QVariantMap actividad=datosActividadMostrada();
    emit solicitarReporteActividades(tr("FactoriaActividades::ActividadReport"),actividad,Print);
}

void MyAcolitoActividades::solicitarImportarDialogo()
{
    QVariantMap actividad=datosActividadMostrada();
    emit solicitarReporteActividades(tr("FactoriaActividades::ActividadReport"),actividad,ShowOnDialog);
}

void MyAcolitoActividades::solicitarImpresion(QSortFilterProxyModel *actividadesDelDia)
{
    QList<QVariantMap> listaActividades=datosListaActividades(actividadesDelDia);
    emit solicitarReportePorDia(tr("FactoriaActividades::ActividadReport"),listaActividades,Print);
}

QVariantMap MyAcolitoActividades::datosActividadMostrada()
{
    QVariantMap datosActividad;

    //Consultar actividadreport.h en ReportesAcolito para los identificadores adecuados
    datosActividad.insert(tr("nombre"),ui->actividadLine->text());
    datosActividad.insert(tr("id"),ui->idLabel->text());
    datosActividad.insert(tr("categoria"),ui->tipoComboBox->currentText());
    datosActividad.insert(tr("responsable"),database->getOrganizadorActividad(ui->idLabel->text().toInt()));
    datosActividad.insert(tr("fecha"),ui->diaComboBox->currentText());
    datosActividad.insert(tr("inicio"),ui->inicioTimeEdit->time());
    datosActividad.insert(tr("fin"),ui->finTimeEdit->time());
    datosActividad.insert(tr("minParticipantes"),ui->minimoSpinBox->value());
    datosActividad.insert(tr("maxParticipantes"),ui->maximoSpinBox->value());
    datosActividad.insert(tr("descripcion"),ui->descBrowser->toPlainText());
    datosActividad.insert(tr("requisitos"),ui->reqBrowser->toPlainText());
    datosActividad.insert(tr("lugar"),ui->lugarComboBox->currentText());

    return datosActividad;
}

QList<QVariantMap> MyAcolitoActividades::datosListaActividades(QSortFilterProxyModel *modActividades)
{
    QList<QVariantMap> resultado;

    //La posicion de los distintos elementos de la actividad puede
    //cambiar, comprobar en Databases/MyModeloActividades
    int numActividades=modActividades->rowCount();
    for(int i=0;i<numActividades;i++) {
        QVariantMap datosActividad;
        datosActividad.insert("nombre",modActividades->index(i,1).data(Qt::EditRole));
        datosActividad.insert("id",modActividades->index(i,0).data(Qt::EditRole));
        datosActividad.insert("categoria",modActividades->index(i,15).data(Qt::EditRole));
        datosActividad.insert("responsable",modActividades->index(i,3).data(Qt::EditRole));
        datosActividad.insert("fecha",modActividades->index(i,9).data(Qt::EditRole));
        datosActividad.insert("inicio",modActividades->index(i,11).data(Qt::EditRole));
        datosActividad.insert("fin",modActividades->index(i,12).data(Qt::EditRole));
        datosActividad.insert("minParticipantes",modActividades->index(i,13).data(Qt::EditRole));
        datosActividad.insert("maxParticipantes",modActividades->index(i,14).data(Qt::EditRole));
        datosActividad.insert("descripcion",modActividades->index(i,16).data(Qt::EditRole));
        datosActividad.insert("requisitos",modActividades->index(i,17).data(Qt::EditRole));
        datosActividad.insert("lugar",modActividades->index(i,19).data(Qt::EditRole));

        resultado.append(datosActividad);
    }

    return resultado;
}

void MyAcolitoActividades::dialogoImpresionPorDias()
{
    ImprimirActividadDialog dialogo(this,database);

    connect(&dialogo,SIGNAL(imprimirDia(QSortFilterProxyModel*)),this,SLOT(solicitarImpresion(QSortFilterProxyModel*)));

    dialogo.exec();
}
