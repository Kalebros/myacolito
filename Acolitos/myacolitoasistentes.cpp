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

#include "myacolitoasistentes.h"
#include "ui_myacolitoasistentes.h"

MyAcolitoAsistentes::MyAcolitoAsistentes(QWidget *parent, MyDatabase *db) :
    MyAcolito(parent,db),
    modeloAsistentes(0),
    modeloAlojamientos(0),
    ui(new Ui::MyAcolitoAsistentes)
{
    ui->setupUi(this);

    okIcon=QIcon(":/acolitos/iconos/okIcon");
    falseIcon=QIcon(":/acolitos/iconos/falseIcon");

    filtroPagos=0;
    filtroActivo=0;
    filtroBusqueda=0;
    filtroVistaGrupos=0;
    mapeadorBase=0;

    setup();

}

MyAcolitoAsistentes::~MyAcolitoAsistentes()
{
    delete ui;
}

void MyAcolitoAsistentes::reloadModelos()
{
    modeloAsistentes=MyAcolito::database->getModeloAsistentes();
    modeloAlojamientos=MyAcolito::database->getModeloAlojamientos();

    setupFiltros();

    ui->vistaAsistentes->setModel(filtroActivo);
    ui->vistaGrupo->setModel(filtroVistaGrupos);

    setupVista();
    setupMapeador();

    connect(MyAcolito::database,SIGNAL(reloadAlojamiento()),this,SLOT(recargarDatosHabitacion()));
}

void MyAcolitoAsistentes::clear()
{
    ui->vistaAsistentes->setModel(0);

    deleteFiltros();

    if(modeloAsistentes) {
        delete modeloAsistentes;
        modeloAsistentes=0;
    }
}

void MyAcolitoAsistentes::dialogoNuevosAsistentes()
{
    addPersonasDialog dialogo(this,database);

    dialogo.exec();
}

void MyAcolitoAsistentes::dialogoAgruparAsistentes()
{
    AddGrupoDialog dialogo(this,database);
    dialogo.exec();
}

void MyAcolitoAsistentes::dialogoAlojarGrupo()
{
    QVariant grupo=ui->grupoButton->getValor();
    AlojarGrupoDialog dialogo(this,grupo,database);
    dialogo.exec();
}

void MyAcolitoAsistentes::dialogoAlojarAsistente()
{
    QVariant asistente(ui->idLabel->text());
    AlojarAsistenteDialog dialogo(this,asistente,database);

    dialogo.exec();
}

void MyAcolitoAsistentes::eliminarAsistentes()
{
    if(QMessageBox::warning(this,tr("Advertencia - Eliminando asistentes"),tr("Está a punto de eliminar a uno o varios asistentes de la base de datos,\n y esta no es una operación que se pueda deshacer.\n\nDesea eliminar a los asistentes seleccionados?"),
                         QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) {
        QModelIndexList listaIndices=ui->vistaAsistentes->selectionModel()->selectedRows();
        QList<Asistente *> listaAsistentes;
        foreach(QModelIndex indice,listaIndices) {
            Asistente *asis=new Asistente(this);
            asis->DNI=filtroActivo->index(indice.row(),2).data(Qt::EditRole).toString();
            asis->tipo=filtroActivo->index(indice.row(),7).data(Qt::EditRole).toString();
            listaAsistentes.append(asis);
        }

        if(listaAsistentes.count())
            database->eliminarAsistentes(listaAsistentes);
    }
}

void MyAcolitoAsistentes::separarAsistente()
{
    if(database) {
        int identificador=ui->idLabel->text().toInt();
        Asistente *asis=new Asistente(this);
        asis->idAsis=identificador;

        database->separarAsistente(asis);
    }
}

void MyAcolitoAsistentes::setOkIcon(QIcon icon)
{
    okIcon=icon;
}

void MyAcolitoAsistentes::setFalseIcon(QIcon icon)
{
    falseIcon=icon;
}

void MyAcolitoAsistentes::setupVista()
{
    //No mostrar: identificador,telefono,comentarios,idgrupo,idplaza,
    //vistaAsistentes
    ui->vistaAsistentes->setColumnHidden(0,true);
    ui->vistaAsistentes->setColumnHidden(4,true);
    ui->vistaAsistentes->setColumnHidden(5,true);
    ui->vistaAsistentes->setColumnHidden(6,true);
    ui->vistaAsistentes->setColumnHidden(8,true);
    ui->vistaAsistentes->setColumnHidden(11,true);

    ui->vistaAsistentes->setItemDelegateForColumn(7,new CategoriasDelegate(this,database));

    ui->vistaAsistentes->setItemDelegateForColumn(9,new BooleanIconDelegate(this,okIcon,falseIcon));
    ui->vistaAsistentes->setItemDelegateForColumn(10,new BooleanIconDelegate(this,okIcon,falseIcon));
    ui->vistaAsistentes->setItemDelegateForColumn(11,new BooleanIconDelegate(this,okIcon,falseIcon));

    ui->vistaAsistentes->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked);

    //Cambiar el codigo si la versión de Qt es 5 o superior
#if QT_VERSION >= 0x050000
    ui->vistaAsistentes->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->vistaAsistentes->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->vistaAsistentes->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
#else
    ui->vistaAsistentes->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    ui->vistaAsistentes->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->vistaAsistentes->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
#endif

    //vistaGrupos
    ui->vistaGrupo->setColumnHidden(0,true);
    ui->vistaGrupo->setColumnHidden(4,true);
    ui->vistaGrupo->setColumnHidden(5,true);
    ui->vistaGrupo->setColumnHidden(6,true);
    ui->vistaGrupo->setColumnHidden(8,true);
    ui->vistaGrupo->setColumnHidden(11,true);

    ui->vistaGrupo->setItemDelegateForColumn(7,new CategoriasDelegate(this,database));

    ui->vistaGrupo->setItemDelegateForColumn(9,new BooleanIconDelegate(this,okIcon,falseIcon));
    ui->vistaGrupo->setItemDelegateForColumn(10,new BooleanIconDelegate(this,okIcon,falseIcon));
    ui->vistaGrupo->setItemDelegateForColumn(11,new BooleanIconDelegate(this,okIcon,falseIcon));

    ui->vistaGrupo->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked);

    //Ajuste de version
#if QT_VERSION >=0x050000
    ui->vistaGrupo->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->vistaGrupo->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->vistaGrupo->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
#else
    ui->vistaGrupo->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    ui->vistaGrupo->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
    ui->vistaGrupo->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
#endif
}

void MyAcolitoAsistentes::setup()
{
    ui->visualizacionBox->setChecked(false);
    showOpcionesVisualizacion(false);

    //Preparar botones
    QIcon *iconoActivo,*iconoFalso;

    iconoActivo=new QIcon(":/acolitos/iconos/okIcon");
    iconoFalso=new QIcon(":/acolitos/iconos/falseIcon");

    openIcon=QIcon(":/acolitos/iconos/habAbierta");
    closeIcon=QIcon(":/acolitos/iconos/habCerrada");

    ui->botonActivo->setIconoTrue(iconoActivo);
    ui->botonActivo->setIconoFalse(iconoFalso);

    connect(ui->visualizacionBox,SIGNAL(toggled(bool)),this,SLOT(showOpcionesVisualizacion(bool)));

    //Preparar acciones de las vistas
    QList<QAction*> listaAcciones;

    QAction *accion_borrarAsistente=new QAction(tr("Borrar asistentes seleccionados"),this);
    connect(accion_borrarAsistente,SIGNAL(triggered()),this,SLOT(eliminarAsistentes()));

    listaAcciones.append(accion_borrarAsistente);

    ui->vistaAsistentes->insertActions(0,listaAcciones);
    ui->vistaAsistentes->setContextMenuPolicy(Qt::ActionsContextMenu);

    //Otras conexiones
    connect(ui->llaveButton,SIGNAL(clicked()),this,SLOT(entregarLlaveHabitacion()));
    connect(ui->habButton,SIGNAL(clicked()),this,SLOT(dialogoAlojarGrupo()));
    connect(ui->grupoButton,SIGNAL(clicked()),this,SLOT(dialogoAlojarAsistente()));
    connect(ui->separarButton,SIGNAL(clicked()),this,SLOT(separarAsistente()));
    connect(ui->imprimirButton,SIGNAL(clicked()),this,SLOT(printAsistente()));
    connect(ui->exportarButton,SIGNAL(clicked()),this,SLOT(exportarAsistente()));
}

void MyAcolitoAsistentes::dialogoCategorias()
{
    AsistenteCategoriasDialog dialogo(this,database);

    dialogo.exec();
}

void MyAcolitoAsistentes::showOpcionesVisualizacion(bool estado)
{
    ui->pagoLabel->setVisible(estado);
    ui->llegadaLabel->setVisible(estado);
    ui->siPagoCheckBox->setVisible(estado);
    ui->noPagoCheckBox->setVisible(estado);
    ui->siLlegadaCheckBox->setVisible(estado);
    ui->noLlegadaCheckBox->setVisible(estado);
}

void MyAcolitoAsistentes::setupFiltros()
{
    deleteFiltros();

    //Filtro de pagos
    filtroPagos=new QSortFilterProxyModel(this);

    filtroPagos->setSourceModel(modeloAsistentes);
    filtroPagos->setFilterKeyColumn(9); //< Columna de pagos: 9

    connect(ui->siPagoCheckBox,SIGNAL(stateChanged(int)),this,SLOT(setFiltradoPago(int)));
    connect(ui->noPagoCheckBox,SIGNAL(stateChanged(int)),this,SLOT(setFiltradoPago(int)));

    //Filtro de activos
    filtroActivo=new QSortFilterProxyModel(this);

    filtroActivo->setSourceModel(filtroPagos);
    filtroActivo->setFilterKeyColumn(10);    //Columna de activos: 10

    connect(ui->siLlegadaCheckBox,SIGNAL(stateChanged(int)),this,SLOT(setFiltradoActivo(int)));
    connect(ui->noLlegadaCheckBox,SIGNAL(stateChanged(int)),this,SLOT(setFiltradoActivo(int)));

    //Filtro de busqueda
    modeloAsistentes->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(ui->searchLine,SIGNAL(textChanged(QString)),modeloAsistentes,SLOT(setFilterFixedString(QString)));
    connect(ui->searchComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setColumnaBusqueda(int)));

    //Filtro de vista de grupos
    filtroVistaGrupos=database->getModeloAsistentes();
    filtroVistaGrupos->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filtroVistaGrupos->setFilterKeyColumn(6); //Columna de idGrupo

    setFiltradoActivo(0);
    setFiltradoPago(0);
    setColumnaBusqueda(ui->searchComboBox->currentIndex());
}

void MyAcolitoAsistentes::setupMapeador()
{
    if(mapeadorBase) {
        delete mapeadorBase;
        mapeadorBase=0;
    }

    //Preparar comboBox de categorias
    ui->categoriasComboBox->setModel(database->getModeloCategorias());
    ui->categoriasComboBox->setModelColumn(0);

    mapeadorBase=new QDataWidgetMapper(this);
    mapeadorBase->setModel(ui->vistaAsistentes->model());

    mapeadorBase->addMapping(ui->idLabel,0,"text");
    mapeadorBase->addMapping(ui->nombreLine,1,"text");
    mapeadorBase->addMapping(ui->dniLine,2,"text");
    mapeadorBase->addMapping(ui->mailLine,3,"text");
    mapeadorBase->addMapping(ui->telefonoLine,4,"text");
    mapeadorBase->addMapping(ui->comentariosBrowser,5,"plainText");
    mapeadorBase->addMapping(ui->categoriasComboBox,7,"texto");
    mapeadorBase->addMapping(ui->botonActivo,10,"estadoBinario");
    mapeadorBase->addMapping(ui->grupoButton,6,"valorBoton");

    mapeadorBase->toFirst();

    connect(ui->vistaAsistentes->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeadorBase,SLOT(setCurrentModelIndex(QModelIndex)));

    connect(ui->botonActivo,SIGNAL(clicked()),this,SLOT(cambiarEstadoAsistente()));
    connect(ui->grupoButton,SIGNAL(valueChangedRegExp(QString)),filtroVistaGrupos,SLOT(setFilterRegExp(QString)));

    //Modelo de alojamientos
    modeloAlojamientos->setFilterKeyColumn(0);

    connect(ui->grupoButton,SIGNAL(valueChangedRegExp(QString)),this,SLOT(filtrarAlojamiento(QString)));
}

void MyAcolitoAsistentes::filtrarAlojamiento(QString grupo)
{
    if(modeloAlojamientos) {
        modeloAlojamientos->setFilterRegExp(grupo);
        QString codigo=modeloAlojamientos->index(0,1).data(Qt::EditRole).toString();
        ui->habLabel->setText(codigo);

        setBotonLlave();
    }
}

void MyAcolitoAsistentes::setColumnaBusqueda(int index)
{
    switch(index) {
    case 0:
        modeloAsistentes->setFilterKeyColumn(1);
        break;
    case 1:
        modeloAsistentes->setFilterKeyColumn(2);
        break;
    case 2:
        modeloAsistentes->setFilterKeyColumn(3);
        break;
    default:
        break;
    }
    ui->searchLine->clear();
}

void MyAcolitoAsistentes::cambiarEstadoAsistente()
{
    int numRegistrosVisibles=filtroActivo->rowCount();

    bool found=false;
    QModelIndex indiceActual;
    for(int i=0;i<numRegistrosVisibles && !found;i++) {
        if(filtroActivo->index(i,0).data(Qt::EditRole).toInt()==ui->idLabel->text().toInt()) {
            found=true;
            indiceActual=filtroActivo->index(i,10); ///Indice del dato activado
            int estadoActual=ui->botonActivo->getEstado();
            filtroActivo->setData(indiceActual,QVariant(estadoActual),Qt::EditRole);
        }
    }
}

void MyAcolitoAsistentes::deleteFiltros()
{
    if(filtroActivo) {
        delete filtroActivo;
        filtroActivo=0;
    }

    if(filtroPagos) {
        delete filtroPagos;
        filtroPagos=0;
    }

    if(filtroBusqueda) {
        delete filtroBusqueda;
        filtroBusqueda=0;
    }

    if(filtroVistaGrupos) {
        delete filtroVistaGrupos;
        filtroVistaGrupos=0;
    }
}

void MyAcolitoAsistentes::setFiltradoActivo(int estado)
{
    bool siChecked,noChecked;

    siChecked=ui->siLlegadaCheckBox->isChecked();
    noChecked=ui->noLlegadaCheckBox->isChecked();

    if(filtroActivo) {

        if((!siChecked && !noChecked) || (siChecked && noChecked)) {
            filtroActivo->setFilterRegExp(tr("0|1"));
        } else if(siChecked && !noChecked) {
            filtroActivo->setFilterRegExp(tr("1"));
        } else if(!siChecked && noChecked) {
            filtroActivo->setFilterRegExp(tr("0"));
        }
    }
}

void MyAcolitoAsistentes::setFiltradoPago(int estado)
{
    bool siChecked,noChecked;

    siChecked=ui->siPagoCheckBox->isChecked();
    noChecked=ui->noPagoCheckBox->isChecked();


    if(filtroPagos) {

        if((!siChecked && !noChecked) || (siChecked && noChecked)) {
            filtroPagos->setFilterRegExp(tr("0|1"));
        } else if(siChecked && !noChecked) {
            filtroPagos->setFilterRegExp(tr("1"));
        } else if(!siChecked && noChecked) {
            filtroPagos->setFilterRegExp(tr("0"));
        }
    }
}

void MyAcolitoAsistentes::setBotonLlave()
{
    QString habitacion=ui->habLabel->text();

    bool abierta=database->estaAbierta(habitacion);

    switch(abierta) {
    case 0:
        ui->llaveButton->setIcon(openIcon);
        ui->llaveButton->setText(tr("Entregar llave de la habitacion"));
        break;
    case 1:
        ui->llaveButton->setIcon(closeIcon);
        ui->llaveButton->setText(tr("Recoger llave de la habitacion"));
        break;
    default:
        break;
    }
}

void MyAcolitoAsistentes::recargarDatosHabitacion()
{
    QString habitacion=modeloAlojamientos->index(0,1).data(Qt::EditRole).toString();
    ui->habLabel->setText(habitacion);

    setBotonLlave();
}

void MyAcolitoAsistentes::entregarLlaveHabitacion()
{
    if(modeloAlojamientos) {
        QString habitacion=ui->habLabel->text();

        if(habitacion!=tr("Sin asignar")) {
            bool llave=database->estaAbierta(habitacion);
            database->abrirHabitacion(habitacion,!llave);
            setBotonLlave();
        }
    }
}

void MyAcolitoAsistentes::printAsistente()
{
    QVariantMap asisDatos=getDatosAsistente();

    if(!asisDatos.isEmpty())
        emit solicitarReporte("FactoriaAsistentes::AsistenteReport",asisDatos,Print);
}

void MyAcolitoAsistentes::exportarAsistente()
{
    QVariantMap asisDatos=getDatosAsistente();
    if(!asisDatos.isEmpty())
        emit solicitarReporte("FactoriaAsistentes::AsistenteReport",asisDatos,ShowOnDialog);
}

void MyAcolitoAsistentes::depurar(QString cad)
{
    qDebug() << cad;
}

QVariantMap MyAcolitoAsistentes::getDatosAsistente()
{
    if(database) {
        QVariantMap asisDatos;

        asisDatos.insert("id",ui->idLabel->text());
        asisDatos.insert("nombre",ui->nombreLine->text());
        asisDatos.insert("dni",ui->dniLine->text());
        asisDatos.insert("telefono",ui->telefonoLine->text());
        asisDatos.insert("email",ui->mailLine->text());
        asisDatos.insert("categoria",ui->categoriasComboBox->currentText());
        asisDatos.insert("habitacion",ui->habLabel->text());
        asisDatos.insert("pago",database->haPagado(ui->dniLine->text()));
        asisDatos.insert("comentarios",ui->comentariosBrowser->toPlainText());

        //Recupera los datos de compañeros de grupo
        QVariantList compis;

        int numCompis=filtroVistaGrupos->rowCount();
        for(int i=0;i<numCompis;i++) {
            QVariantMap datAsis;
            //Consultar columna de datos en Databases/mymodeloasistente
            datAsis.insert("nombre",filtroVistaGrupos->index(i,1).data());
            datAsis.insert("dni",filtroVistaGrupos->index(i,2).data());
            datAsis.insert("correo",filtroVistaGrupos->index(i,3).data());
            datAsis.insert("pago",filtroVistaGrupos->index(i,9).data());
            datAsis.insert("categoria",filtroVistaGrupos->index(i,7).data());

            compis.append(datAsis);
        }

        asisDatos.insert("grupo",QVariant(compis));

        return asisDatos;
    }

    return QVariantMap();
}
