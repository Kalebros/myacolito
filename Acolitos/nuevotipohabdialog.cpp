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

#include "nuevotipohabdialog.h"
#include "ui_nuevotipohabdialog.h"

NuevoTipoHabDialog::NuevoTipoHabDialog(QWidget *parent,AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::NuevoTipoHabDialog),
    database(db)
{
    ui->setupUi(this);

    ui->categoriaComboBox->setModel(database->getModeloCategorias());
    ui->categoriaComboBox->setModelColumn(0);

    ui->categoriaComboBox->setCurrentIndex(0);

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(vigilaBotones(QAbstractButton*)));
}

NuevoTipoHabDialog::~NuevoTipoHabDialog()
{
    delete ui;
}

QVariantMap NuevoTipoHabDialog::getDatosTipoHabitacion()
{
    return datosTipoHab;
}

void NuevoTipoHabDialog::vigilaBotones(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::AcceptRole) {
        //Comprueba que el nombre sea correcto
        if(ui->tipoLineEdit->text().isNull() || ui->tipoLineEdit->text().isEmpty()) {
            QMessageBox::warning(this,tr("MyAcolito - Warning"),tr("No se ha introducido un nombre valido para el tipo de habitacion."));
            return;
        }
        datosTipoHab.insert("nombreTipo",ui->tipoLineEdit->text());
        datosTipoHab.insert("plazas",ui->plazasSpinBox->value());
        datosTipoHab.insert("categoriaPermitida",ui->categoriaComboBox->currentText());
    }
}
