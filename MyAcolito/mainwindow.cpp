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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup();
    setEstado(Desconectado);
    readSettings();

    //Establecer el path de busqueda de librerias
    qDebug() << qApp->libraryPaths();

    QString dir=qApp->applicationDirPath();
    QString pathDll=dir+tr("/dll");
    QString pathPlugins=dir+tr("/plugins");
    qDebug() << pathDll << pathPlugins;

    QStringList paths;
    paths << pathDll << pathPlugins << dir;

    QApplication::instance()->setLibraryPaths(paths);
    qDebug() << qApp->libraryPaths();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    writeSettings();
    QMainWindow::closeEvent(e);
}

MainWindow::EstadoConexion MainWindow::getEstado() const
{
    return estadoConexion;
}

void MainWindow::setEstado(EstadoConexion est)
{
    LogDialog *pantallaLog=new LogDialog(this,qApp->applicationDirPath().append("/conexiones.db"));
    switch(est) {
    case Conectado:
        if(pantallaLog->exec()==QDialog::Accepted) {
            DatosConexion conexionSeleccionada=pantallaLog->getConexion();
            //Conectar usando esta conexion
            if(!dbEventos->connectToServer(conexionSeleccionada)) {
                QMessageBox::warning(this,tr("MyAcolito - Fallo durante conexion"),dbEventos->lastError().second);
                ui->actionConectar->setChecked(false);
                break;
            }

            if(!dbEventos->existeDatabase())
                dbEventos->createDatabase();
            else
                dbEventos->openDatabase();
        }
        ui->actionConectar->setText(tr("Desconectar"));
        lanzarDialogoEventos();
        break;
    case Desconectado:
        ui->actionConectar->setText(tr("Conectar"));
        dbEventos->disconnect();
        emit clearAcolitos();
        break;
    }
    delete pantallaLog;
}

void MainWindow::setup()
{
    conexionEvento=0;

    connect(ui->actionConectar,SIGNAL(triggered(bool)),this,SLOT(switchEstadoConexion(bool)));
    connect(ui->actionOpciones,SIGNAL(triggered()),this,SLOT(verPreferencias()));
    dbEventos=MyEventoDB::getInstance(this);
    database=MyDatabase::getInstance(this);

    asistenteEventos=new WizardEvento(this);
    asistenteEventos->setIcono(QIcon(":/iconos/MyAcolito/iconoApp"));
    asistenteEventos->setWindowModality(Qt::WindowModal);

    connect(asistenteEventos,SIGNAL(datosEvento(Evento*,QStandardItemModel*)),this,SLOT(crearEvento(Evento*,QStandardItemModel*)));
    connect(dbEventos,SIGNAL(nuevoEvento(Evento*,QStandardItemModel*)),database,SLOT(crearDatabase(Evento*,QStandardItemModel*)));

    //Preparar el sliding stacked widget
    ui->stackedWidget->setAnimation(QEasingCurve::InOutBack);
    ui->stackedWidget->setSpeed(1500);
    gPaginas=new GestorPaginas(this);

    QActionGroup *accionesAcolito=new QActionGroup(ui->acolitoToolBar);

    accionesAcolito->addAction(ui->actionAccion1);
    accionesAcolito->addAction(ui->actionAccion2);
    accionesAcolito->addAction(ui->actionAccion3);
    accionesAcolito->setExclusive(true);

    ui->acolitoToolBar->addAction(ui->actionAccion1);
    ui->acolitoToolBar->addAction(ui->actionAccion2);
    ui->acolitoToolBar->addAction(ui->actionAccion3);

    //Añadir los widgets (acolitos)
    acolitoAsistentes=new MyAcolitoAsistentes(this,(MyDatabase*)database);
    acolitoHabitaciones=new MyAcolitoHabitaciones(this,(MyDatabase*)database);
    acolitoActividades=new MyAcolitoActividades(this,(MyDatabase*)database);

    gPaginas->setSlidingStackedWidget(ui->stackedWidget);
    gPaginas->addPageWidget(ui->actionAccion1,acolitoAsistentes);
    gPaginas->addPageWidget(ui->actionAccion2,acolitoHabitaciones);
    gPaginas->addPageWidget(ui->actionAccion3,acolitoActividades);

    //Conectar aqui los acolitos con las bases de datos
    connect(this,SIGNAL(reloadAcolitos(AcolitoDatabase*)),acolitoAsistentes,SLOT(loadAcolito(AcolitoDatabase*)));
    connect(this,SIGNAL(reloadAcolitos(AcolitoDatabase*)),acolitoHabitaciones,SLOT(loadAcolito(AcolitoDatabase*)));
    connect(this,SIGNAL(reloadAcolitos(AcolitoDatabase*)),acolitoActividades,SLOT(loadAcolito(AcolitoDatabase*)));
    connect(this,SIGNAL(clearAcolitos()),acolitoAsistentes,SLOT(clear()));
    connect(this,SIGNAL(clearAcolitos()),acolitoHabitaciones,SLOT(clear()));
    connect(this,SIGNAL(clearAcolitos()),acolitoActividades,SLOT(clear()));
    connect(database,SIGNAL(nuevaDatabase()),acolitoAsistentes,SLOT(reloadModelos()));
    connect(database,SIGNAL(nuevaDatabase()),acolitoHabitaciones,SLOT(reloadModelos()));
    connect(database,SIGNAL(nuevaDatabase()),acolitoActividades,SLOT(reloadModelos()));

    connect(ui->actionA_adir_asistentes,SIGNAL(triggered()),acolitoAsistentes,SLOT(dialogoNuevosAsistentes()));
    connect(ui->actionNueva_habitaci_n,SIGNAL(triggered()),acolitoHabitaciones,SLOT(dialogoNuevaHabitacion()));
    connect(ui->actionNueva_lista_de_habitaciones,SIGNAL(triggered()),acolitoHabitaciones,SLOT(dialogoNuevaListaHabitaciones()));
    connect(ui->actionCategorias_de_asistentes,SIGNAL(triggered()),acolitoAsistentes,SLOT(dialogoCategorias()));
    connect(ui->actionTipos_de_habitaciones,SIGNAL(triggered()),acolitoHabitaciones,SLOT(dialogoTiposHabitacion()));
    connect(ui->actionTipos_de_actividad,SIGNAL(triggered()),acolitoActividades,SLOT(dialogoTiposActividad()));
    connect(ui->actionLocalizaciones,SIGNAL(triggered()),acolitoActividades,SLOT(dialogoLocalizaciones()));
    connect(ui->actionNueva_actividad,SIGNAL(triggered()),acolitoActividades,SLOT(dialogoNuevaActividad()));
    connect(ui->actionImprimir_actividades_por_dias,SIGNAL(triggered()),acolitoActividades,SLOT(dialogoImpresionPorDias()));


    //DEBUG
    connect(ui->actionAgrupar_asistentes,SIGNAL(triggered()),acolitoAsistentes,SLOT(dialogoAgruparAsistentes()));

    //Preparar reportes
    gestorReportes=new BReportManager(this);
    gestorReportes->addFactory(new FactoriaActividades(gestorReportes));
    gestorReportes->addFactory(new FactoriaAsistentes(gestorReportes));
    gestorReportes->addFactory(new FactoriaHabitaciones(gestorReportes));

    connect(gestorReportes,SIGNAL(error(QString)),this,SLOT(error(QString)));

    connect(acolitoActividades,SIGNAL(solicitarReporteActividades(QString,QVariantMap,ReportOptions)),gestorReportes,SLOT(basicReport(QString,QVariantMap,ReportOptions)));
    connect(acolitoActividades,SIGNAL(solicitarReportePorDia(QString,QList<QVariantMap>,ReportOptions)),gestorReportes,SLOT(batchReport(QString,QList<QVariantMap>,ReportOptions)));

    connect(acolitoAsistentes,SIGNAL(solicitarReporte(QString,QVariantMap,ReportOptions)),gestorReportes,SLOT(basicReport(QString,QVariantMap,ReportOptions)));

    connect(acolitoHabitaciones,SIGNAL(solicitarReporteHabitacion(QString,QVariantMap,ReportOptions)),gestorReportes,SLOT(basicReport(QString,QVariantMap,ReportOptions)));

    //Preparar informes
    gInformes=new GestorInformes(this);
    gInformes->setDatabase(database);

    connect(gInformes,SIGNAL(solicitarInforme(QString,QVariantMap,ReportOptions)),gestorReportes,SLOT(basicReport(QString,QVariantMap,ReportOptions)));
    connect(ui->actionInforme_actividades_por_dias,SIGNAL(triggered()),gInformes,SLOT(reporteActividadesPorDias()));
    connect(ui->actionResumen_por_categor_as,SIGNAL(triggered()),gInformes,SLOT(reporteActividadesPorTipos()));
    connect(ui->actionResumen_por_lugares,SIGNAL(triggered()),gInformes,SLOT(reporteActividadesPorLocalizacion()));
    connect(ui->actionAsistentes_por_categorias,SIGNAL(triggered()),gInformes,SLOT(reporteAsistentesPorCategorias()));

    //Preparar scripts
    gestorScripts=new SManager(this);
    connect(gestorScripts,SIGNAL(initScript(SMScript*)),this,SLOT(scriptIniciado(SMScript*)));
    connect(gestorScripts,SIGNAL(finishedScript(SMScript*)),this,SLOT(scriptFinalizado(SMScript*)));

    progresoScriptDialog=new QProgressDialog(this);
    progresoScriptDialog->setModal(true);
    progresoScriptDialog->setWindowTitle("MyAcolito - Progreso de script");
    progresoScriptDialog->setRange(0,0);

    connect(progresoScriptDialog,SIGNAL(canceled()),gestorScripts,SLOT(cancelScript()));

    //Procesado de documentos JSON
    parserJson=new AcolitoJSONParser(this,database);

    //Procesamiento de carpetas y demas
    connect(ui->actionProcesar_carpeta_de_actividades,SIGNAL(triggered()),this,SLOT(procesarCarpeta()));

    //Dialogo de mensajes
    connect(ui->actionMensajes,SIGNAL(triggered()),this,SLOT(dialogoMensajes()));

    //Otras acciones
    connect(ui->actionAcerca_de_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
}

void MainWindow::switchEstadoConexion(bool estado)
{
    if(estado)
        setEstado(Conectado);
    else setEstado(Desconectado);
}

void MainWindow::verPreferencias()
{
    PreferenciasDialog *dialogo=new PreferenciasDialog(this,qApp->applicationDirPath().append("/conexiones.db"),
                                                       qApp->applicationDirPath().append("/scriptSto.db"));

    connect(dialogo,SIGNAL(ejecutarScript(SMScript*)),gestorScripts,SLOT(execScript(SMScript*)));

    dialogo->exec();

    delete dialogo;
}

void MainWindow::lanzarDialogoEventos()
{
    EventoDialog dialogo(this,dbEventos->getModeloEventos());

    connect(&dialogo,SIGNAL(lanzarAsistenteEventos()),asistenteEventos,SLOT(lanzarAsistente()));

    if(dialogo.exec()==QDialog::Accepted) {
        Evento *eventoActual=dialogo.getDatosEvento();
        abrirEvento(eventoActual);
        delete eventoActual;
    }
}

void MainWindow::error(QString mensaje)
{
    QMessageBox::warning(this,tr("MyAcolito - Se ha producido un error"),mensaje);
}

void MainWindow::crearEvento(Evento *ev, QStandardItemModel *categorias)
{
    dbEventos->addEvento(*ev,categorias);
}

void MainWindow::abrirEvento(Evento *ev)
{
    DatosConexion *datos=new DatosConexion(dbEventos->getDatosConexion());

    datos->setDatabaseName(ev->database());
    MyAcolitoDB *nuevaConexion=new MyAcolitoDB(qobject_cast<QObject*>(this),0,tr("EventoAuxiliar"));

    if(!nuevaConexion->connectToServer(*datos)) {
        error(tr("En abrirEvento: ")+nuevaConexion->lastError().second);
        return;
    }

    //Voy a tener que eliminar esta comprobacion porque falla
    //y todavia no se por que
    /*
    if(!nuevaConexion->existeDatabase()) {
        error(tr("No existe la base de datos de ")+ev->nombre()+tr("\n")+nuevaConexion->lastError().second);
        return;
    }
    */

    if(conexionEvento) {
        conexionEvento->solicitarDesconexion();
        while(conexionEvento->isConnected())
        {}
        delete conexionEvento;
    }

    conexionEvento=nuevaConexion;

    database->setAcolitoDatabase(conexionEvento);
    conexionEvento->setNombreConexion(tr("Evento"));

    conexionEvento->openDatabase();

    database->conectar();

    emit reloadAcolitos(database);
}

void MainWindow::writeSettings()
{
    QSettings settings(tr("ARM"),tr("MyAcolito"),this);

    settings.beginGroup(tr("MainWindow"));
    settings.setValue(tr("size"),size());
    settings.setValue(tr("pos"),pos());
    settings.setValue(tr("maximized"),this->isMaximized());
    settings.setValue(tr("fullScreen"),this->isFullScreen());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings(tr("ARM"),tr("MyAcolito"),this);

    settings.beginGroup(tr("MainWindow"));
    QSize sizeWindow=settings.value(tr("size"),QSize(400,400)).toSize();
    QPoint posWindow=settings.value(tr("pos"),QPoint(200,200)).toPoint();
    bool isMaximized=settings.value(tr("maximized"),0).toBool();
    settings.endGroup();

    resize(sizeWindow);
    move(posWindow);
    if(isMaximized)
        showMaximized();
}

void MainWindow::mostrarDebug()
{
//    ui->tableView->setModel(database->getModeloAsistentes());
}

void MainWindow::procesarCarpeta()
{
    ProcesadorActividades dialogo(this,database);

    dialogo.exec();
}

void MainWindow::scriptIniciado(SMScript *s)
{
    progresoScriptDialog->setLabelText(s->getNombre()+" lanzado... esperando resultados");
    progresoScriptDialog->reset();
    progresoScriptDialog->setRange(0,0);
    progresoScriptDialog->show();
}

void MainWindow::scriptFinalizado(SMScript *s)
{
    progresoScriptDialog->setLabelText(s->getNombre()+" terminado.");
    progresoScriptDialog->reset();
    progresoScriptDialog->setRange(0,1);
    progresoScriptDialog->setValue(1);
    progresoScriptDialog->close();

    //PRUEBAS
    QJsonDocument *salidaScript=gestorScripts->getScriptResults();

    if(salidaScript) {
        qDebug() << "Documento CORRECTO";
    }

    if(!parserJson->parseJSONDocument(salidaScript))
        qDebug() << "ERROR en ParserJSON";
}

void MainWindow::dialogoMensajes()
{
    MensajesDialog dialogo(this,database);

    dialogo.exec();
}
