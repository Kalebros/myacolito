#include "mensajesdialog.h"
#include "ui_mensajesdialog.h"

MensajesDialog::MensajesDialog(QWidget *parent,AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::MensajesDialog),
    database(db)
{
    ui->setupUi(this);
    modeloMensajes=0;

    ui->progressBar->setVisible(false);
    if(!database->getAcolitoDatabase()) {
        this->setEnabled(false);
        ui->stackedWidget->setCurrentIndex(2);  // Sin conexion
    }
    else if(!database->getAcolitoDatabase()->isConnected())
    {
        this->setEnabled(false);
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if(!database->existeTablaMensajes()) {
        ui->stackedWidget->widget(0)->setEnabled(false);
        ui->stackedWidget->widget(2)->setEnabled(false);
        ui->stackedWidget->widget(1)->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(1);  // Con conexion, pero no existe la tabla de mensajes
    }
    else {
        ui->stackedWidget->widget(1)->setEnabled(false);
        ui->stackedWidget->widget(2)->setEnabled(false);
        ui->stackedWidget->widget(0)->setEnabled(true);
        loadModeloMensajes();
        ui->stackedWidget->setCurrentIndex(0);  // con conexion y existe la tabla de mensajes
    }

    setup();
}

void MensajesDialog::setup()
{
    ui->vistaMensajes->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->vistaMensajes->addAction(ui->actionBorrar_mensajes_seleccionados);

    //Conectar acciones
    connect(ui->crearTablaButton,SIGNAL(clicked()),this,SLOT(crearTablaMensajes()));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addMensaje()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(borrarMensajes()));
    connect(ui->actionBorrar_mensajes_seleccionados,SIGNAL(triggered()),this,SLOT(borrarMensajes()));

}

MensajesDialog::~MensajesDialog()
{
    delete ui;
}

void MensajesDialog::crearTablaMensajes()
{
    ui->progressBar->setRange(0,0);
    ui->progressBar->setVisible(true);
    database->crearTablaMensajes();
    ui->progressBar->setRange(0,1);
    ui->progressBar->setValue(1);
    ui->stackedWidget->widget(0)->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(0);
    loadModeloMensajes();
    ui->stackedWidget->widget(1)->setEnabled(false);
    ui->progressBar->setVisible(false);
}

void MensajesDialog::loadModeloMensajes()
{
    if(modeloMensajes) {
        ui->vistaMensajes->setModel(0);
        delete modeloMensajes;
    }

    modeloMensajes=database->getModeloMensajes();
    ui->vistaMensajes->setModel(modeloMensajes);

    ui->vistaMensajes->horizontalHeader()->setSectionHidden(0,true);
    ui->vistaMensajes->horizontalHeader()->setStretchLastSection(true);
    ui->vistaMensajes->horizontalHeader()->setVisible(false);
}

void MensajesDialog::addMensaje()
{
    QString mensaje=QInputDialog::getText(
                this,
                "Nuevo mensaje",
                "Introduce el nuevo mensaje");

    if(mensaje.isEmpty() || mensaje.isNull())
        return;

    database->nuevoMensaje(mensaje);
}

void MensajesDialog::borrarMensajes()
{
    QModelIndexList indicesSeleccionados=ui->vistaMensajes->selectionModel()->selectedRows();

    if(indicesSeleccionados.isEmpty())
        return;

    QList<int> idSeleccionados;
    foreach(QModelIndex indice,indicesSeleccionados)
        idSeleccionados << modeloMensajes->index(indice.row(),0).data().toInt();

    foreach(int id,idSeleccionados)
        database->borrarMensaje(id);
}
