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

#include "addpersonasdialog.h"
#include "ui_addpersonasdialog.h"

addPersonasDialog::addPersonasDialog(QWidget *parent,AcolitoDatabase *database) :
    QDialog(parent),
    ui(new Ui::addPersonasDialog),
    db(database)
{
    ui->setupUi(this);
    setup();
}

addPersonasDialog::~addPersonasDialog()
{
    delete ui;
}

void addPersonasDialog::setup()
{
    setModal(true);
    modeloPersonas=new QStandardItemModel();
    mapeador=new QDataWidgetMapper(this);

    ui->vistaPersonas->setModel(modeloPersonas);
    ui->vistaPersonas->setModelColumn(Nombre);   //Columna del nombre

    //Prepara el mapeador
    mapeador->setModel(modeloPersonas);
    mapeador->addMapping(ui->nombreEdit,Nombre);
    mapeador->addMapping(ui->dniEdit,DNI);
    mapeador->addMapping(ui->tlfEdit,Telefono);
    mapeador->addMapping(ui->mailEdit,Correo);
    mapeador->addMapping(ui->categoriaComboBox,Categoria,"texto");
    mapeador->addMapping(ui->checkBox,PlazaPagada,"estado");
    mapeador->addMapping(ui->comentarioTextEdit,Comentario,"plainText");
    mapeador->toFirst();

    connect(ui->vistaPersonas->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));

    //
    connect(ui->addPersonaButton,SIGNAL(clicked()),this,SLOT(addPersona()));
    connect(ui->delPersonaButton,SIGNAL(clicked()),this,SLOT(borraPersona()));
    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(compruebaBoton(QAbstractButton*)));

    ui->categoriaComboBox->setModel(db->getModeloCategorias());
    ui->categoriaComboBox->setModelColumn(0);

    connect(this,SIGNAL(nuevoGrupoPersonas(QList<Asistente*>)),db,SLOT(addGrupoAsistentes(QList<Asistente*>)));
}

void addPersonasDialog::reloadDialog()
{
    //Borrar registros anteriores
    int numRegistros=modeloPersonas->rowCount();
    for(int i=0;i<numRegistros;i++) {
        QList<QStandardItem*> lista=modeloPersonas->takeRow(0);
        foreach(QStandardItem *item,lista) {
            delete item;
        }
    }

    //Recargar modelo de columna
    // @todo Implementar modelo de columna
    ui->categoriaComboBox->setModel(db->getModeloCategorias());
    ui->categoriaComboBox->setModelColumn(0);
    limpiaCampos();
}

void addPersonasDialog::mostrarDialogo()
{
    reloadDialog();
    show();
}

void addPersonasDialog::addPersona()
{
    QList<QStandardItem *> nuevoRegistro;

    for(int i=0;i<=Comentario;i++) {
        QStandardItem *item=new QStandardItem();
        if(i==Nombre)
            item->setText(tr("Nuevo asistente"));
        if(i==Categoria)
            item->setText(ui->categoriaComboBox->itemText(0));
        if(i==PlazaPagada)
            item->setData(QVariant(false),Qt::EditRole);
        if(i==Comentario)
            item->setText(QString());
        nuevoRegistro.append(item);
    }
    modeloPersonas->appendRow(nuevoRegistro);
}

void addPersonasDialog::compruebaBoton(QAbstractButton *boton)
{
    QDialogButtonBox::StandardButton rol=ui->buttonBox->standardButton(boton);

    switch(rol) {
    case QDialogButtonBox::Discard :    //Descartar
        close();
        break;
    case QDialogButtonBox::Apply :  //Enviar
        sendGrupo();
        break;
    default:
        break;
    }
}

void addPersonasDialog::borraPersona()
{
    QItemSelectionModel *listaSeleccion=ui->vistaPersonas->selectionModel();

    QModelIndexList lista=listaSeleccion->selectedIndexes();

    qSort(lista);

    foreach(QModelIndex indice,lista)
        modeloPersonas->removeRow(indice.row());
}

void addPersonasDialog::sendGrupo()
{
    QList<Asistente*> listaGrupo;
    Asistente *asis;
    int numRegistros=modeloPersonas->rowCount();
    for(int i=0;i<numRegistros;i++) {
        QList<QStandardItem*> registro=modeloPersonas->takeRow(0);
        asis=new Asistente;
        asis->nombre=registro.at(Nombre)->text();
        asis->DNI=registro.at(DNI)->text();
        asis->telefono=registro.at(Telefono)->text();
        asis->mail=registro.at(Correo)->text();
        asis->comentarios=registro.at(Comentario)->text();
        if(asis->comentarios.isNull())
            asis->comentarios=QString(" ");
        asis->pago=registro.at(PlazaPagada)->data(Qt::EditRole).toBool();
        asis->tipo=registro.at(Categoria)->text();

        listaGrupo << asis;
    }

    emit this->nuevoGrupoPersonas(listaGrupo);
    limpiaCampos();
}

void addPersonasDialog::limpiaCampos()
{
    addPersona();
    ui->vistaPersonas->setCurrentIndex(modeloPersonas->index(0,0));
}
