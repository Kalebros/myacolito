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

#include "addlistahabitacionesdialog.h"
#include "ui_addlistahabitacionesdialog.h"

AddListaHabitacionesDialog::AddListaHabitacionesDialog(QWidget *parent,AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::AddListaHabitacionesDialog),
    _db(db)
{
    ui->setupUi(this);
    if(db) {
        modeloTiposHabitaciones=_db->getModeloTiposHabitaciones();
        ui->tiposComboBox->setModel(modeloTiposHabitaciones);
        ui->tiposComboBox->setModelColumn(0);
    }

    ui->habLabels->setVisible(false);
    ui->progressBar->setVisible(false);

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(vigilaBoton(QAbstractButton*)));
}

AddListaHabitacionesDialog::~AddListaHabitacionesDialog()
{
    delete ui;
}

void AddListaHabitacionesDialog::vigilaBoton(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::ApplyRole) {
        generaHabitaciones();
        close();
    }
}

void AddListaHabitacionesDialog::generaHabitaciones()
{
    setProgressBarOn(true);
    int valorInicial=ui->origenSpinBox->value();
    int valorFinal=ui->limiteSpinBox->value();

    ui->progressBar->setRange(0,(valorFinal-valorInicial)+1);

    Habitacion *hab;
    for(int i=valorInicial;i<=valorFinal;i++) {
        ui->progressBar->setValue(i);
        hab=new Habitacion(this);
        hab->tipo=ui->tiposComboBox->currentText();
        hab->codigo=ui->baseLineEdit->text().arg(i);
        hab->comentario=QString(" ");
        hab->estaReservada=false;
        hab->conceptoReserva=QString("Escribe aquí el concepto de reserva");
        _db->addNuevaHabitacion(hab);
    }
    ui->progressBar->setValue(valorFinal+1);
    setProgressBarOn(false);
}

void AddListaHabitacionesDialog::setProgressBarOn(bool modo)
{
    switch(modo) {
    case true:
        ui->progressBar->setRange(0,0);
        ui->habLabels->setVisible(true);
        ui->progressBar->setVisible(true);
        ui->gridLayout->setEnabled(false);
        ui->buttonBox->setEnabled(false);
        break;
    case false:
        ui->habLabels->setVisible(false);
        ui->progressBar->setVisible(false);
        ui->gridLayout->setEnabled(true);
        ui->buttonBox->setEnabled(true);
        break;
    default:
        break;
    }
}
