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

#include "nuevacategoriadialog.h"
#include "ui_nuevacategoriadialog.h"

NuevaCategoriaDialog::NuevaCategoriaDialog(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::NuevaCategoriaDialog),
    database(db)
{
    ui->setupUi(this);

    ui->fechaEntradaCombo->setModel(database->getModeloFechas());
    ui->fechaLimiteCombo->setModel(database->getModeloFechas());

    ui->fechaEntradaCombo->setModelColumn(0);
    ui->fechaEntradaCombo->setModel(0);

    ui->fechaEntradaCombo->setCurrentIndex(0);
    ui->fechaLimiteCombo->setCurrentIndex(ui->fechaLimiteCombo->count()-1);

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(vigilarButtonBox(QAbstractButton*)));
}

NuevaCategoriaDialog::~NuevaCategoriaDialog()
{
    delete ui;
}

QVariantMap NuevaCategoriaDialog::getDatosCategoria()
{
    return mapaDatos;
}

void NuevaCategoriaDialog::vigilarButtonBox(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::AcceptRole) {
        //Comprobar que hay un nombre correcto
        if(ui->nombreLine->text().isNull() || ui->nombreLine->text().isEmpty()) {
            QMessageBox::warning(this,tr("MyAcolito - Warning"),tr("El nombre de categoria no es valido."));
            return;
        }
        mapaDatos.insert(tr("nombre"),ui->nombreLine->text());
        mapaDatos.insert(tr("precio"),0);
        mapaDatos.insert(tr("maximo"),0);
        mapaDatos.insert(tr("fechaInicio"),ui->fechaEntradaCombo->itemText(0));
        mapaDatos.insert(tr("fechaLimite"),ui->fechaLimiteCombo->itemText(ui->fechaLimiteCombo->count()-1));
    }
}
