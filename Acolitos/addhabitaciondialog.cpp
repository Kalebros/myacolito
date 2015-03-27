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

#include "addhabitaciondialog.h"
#include "ui_addhabitaciondialog.h"

AddHabitacionDialog::AddHabitacionDialog(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::AddHabitacionDialog),
    _db(db),
    modeloTipos(0)
{
    ui->setupUi(this);
    if(_db) {
        modeloTipos=_db->getModeloTiposHabitaciones();
        ui->tipoComboBox->setModel(modeloTipos);
        ui->tipoComboBox->setModelColumn(0);
    }

    connect(ui->reservadaCheckBox,SIGNAL(stateChanged(int)),this,SLOT(activarReserva(int)));
    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(procesarBoton(QAbstractButton*)));
}

AddHabitacionDialog::~AddHabitacionDialog()
{
    delete ui;
}

void AddHabitacionDialog::setAcolitoDatabase(AcolitoDatabase *db)
{
    _db=db;
    if(modeloTipos) {
        delete modeloTipos;
        modeloTipos=0;
    }
    modeloTipos=_db->getModeloTiposHabitaciones();
    ui->tipoComboBox->setModel(modeloTipos);
    ui->tipoComboBox->setModelColumn(0);
}

void AddHabitacionDialog::activarReserva(int estado)
{
    if(estado==0)
        ui->reservaTextEdit->setPlainText(tr("Escribe el concepto de reserva"));
    ui->reservaTextEdit->setEnabled((bool)estado);
}

void AddHabitacionDialog::lanzaDialogo()
{
    if(_db) {
        if(modeloTipos) {
            delete modeloTipos;
            modeloTipos=0;
        }
        modeloTipos=_db->getModeloTiposHabitaciones();
        ui->tipoComboBox->setModel(modeloTipos);
        ui->tipoComboBox->setModelColumn(0);
    }
    limpiarCampos();
}

void AddHabitacionDialog::limpiarCampos()
{
    ui->codigoLine->setText(QString());
    ui->tipoComboBox->setCurrentIndex(0);
    ui->comentarioTextEdit->setPlainText(QString());
    ui->reservaTextEdit->setPlainText(QString());
    ui->reservadaCheckBox->setChecked(true);
}

void AddHabitacionDialog::procesarBoton(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::AcceptRole)
        guardarHabitacion();
}

void AddHabitacionDialog::guardarHabitacion()
{
    Habitacion *hab=new Habitacion(this);

    hab->codigo=ui->codigoLine->text();
    hab->tipo=ui->tipoComboBox->currentText();

    if(ui->reservadaCheckBox->isChecked())
        hab->estaReservada=true;
    else hab->estaReservada=false;

    hab->conceptoReserva=ui->reservaTextEdit->toPlainText();
    hab->comentario=ui->comentarioTextEdit->toPlainText();

    _db->addNuevaHabitacion(hab);

    this->close();
}
