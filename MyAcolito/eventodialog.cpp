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

#include "eventodialog.h"
#include "ui_eventodialog.h"

EventoDialog::EventoDialog(QWidget *parent, QSortFilterProxyModel *mod) :
    QDialog(parent),
    ui(new Ui::EventoDialog),
    _modelo(mod)
{
    ui->setupUi(this);
    datosEvento=0;
    setupDialog();
}

EventoDialog::~EventoDialog()
{
    delete ui;
}

void EventoDialog::setupDialog()
{
    ui->buttonBox->addButton(tr("Abrir"),QDialogButtonBox::AcceptRole);
    ui->buttonBox->addButton(tr("Crear"),QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(tr("Cancelar"),QDialogButtonBox::RejectRole);

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(botonPulsado(QAbstractButton*)));
    ui->listView->setItemDelegate(new DelegadoEvento(this));
    ui->listView->setModel(_modelo);
}

void EventoDialog::botonPulsado(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::ActionRole) //Boton de crear
        emit lanzarAsistenteEventos();
    else if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::AcceptRole) //Boton abrir
        guardarEvento();
}

void EventoDialog::guardarEvento()
{
    QModelIndex idx=ui->listView->currentIndex();

    datosEvento=MyEventoDB::getInstance()->getEvento(_modelo->data(idx,MyModeloEventos::Nombre).toString());
}

Evento *EventoDialog::getDatosEvento()
{
    return datosEvento;
}
