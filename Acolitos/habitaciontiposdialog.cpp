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

#include "habitaciontiposdialog.h"
#include "ui_habitaciontiposdialog.h"

HabitacionTiposDialog::HabitacionTiposDialog(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::HabitacionTiposDialog),
    database(db)
{
    ui->setupUi(this);

    modeloTiposHabitacion=database->getModeloTiposHabitaciones();

    ui->vistaTipos->setModel(modeloTiposHabitacion);

    ui->vistaTipos->setModelColumn(0);

    ui->categoriaComboBox->setModel(database->getModeloCategorias());
    ui->categoriaComboBox->setModelColumn(0);
    ui->categoriaComboBox->setCurrentIndex(0);

    mapeador=new QDataWidgetMapper(this);
    mapeador->setModel(modeloTiposHabitacion);

    mapeador->addMapping(ui->tipoLineEdit,0,"text");
    mapeador->addMapping(ui->plazasSpinBox,1,"value");
    mapeador->addMapping(ui->categoriaComboBox,2,"fechaSeleccionada");

    mapeador->toFirst();

    connect(ui->vistaTipos->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));

    connect(ui->nuevoTipoButton,SIGNAL(clicked()),this,SLOT(addTipoHabitacion()));
    connect(ui->delTipoButton,SIGNAL(clicked()),this,SLOT(borrarTipoHabitacion()));
}

HabitacionTiposDialog::~HabitacionTiposDialog()
{
    delete ui;
}

void HabitacionTiposDialog::addTipoHabitacion()
{
    NuevoTipoHabDialog dialogo(this,database);

    dialogo.exec();

    database->addNuevoTipoHabitacion(dialogo.getDatosTipoHabitacion());
}

void HabitacionTiposDialog::borrarTipoHabitacion()
{
    QString tipoABorrar=ui->tipoLineEdit->text();

    QString mensaje=tr("Antes de eliminar el tipo %1, debe seleccionar al que lo sustituira en las habitaciones marcadas como tales.").arg(tipoABorrar);

    QStringList listaCategorias;

    for(int i=0;i<modeloTiposHabitacion->rowCount();i++) {
        if(modeloTiposHabitacion->index(i,0).data(Qt::EditRole).toString()!=tipoABorrar)
            listaCategorias << modeloTiposHabitacion->index(i,0).data(Qt::EditRole).toString();
    }

    bool ok;
    QString tipoSustituto=QInputDialog::getItem(this,tr("MyAcolito - Borrar tipo de habitacion"),mensaje,listaCategorias,0,0,&ok);

    if(ok)
        database->borrarTipoHabitacion(tipoABorrar,tipoSustituto);
}
