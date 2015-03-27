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

#include "alojargrupodialog.h"
#include "ui_alojargrupodialog.h"

AlojarGrupoDialog::AlojarGrupoDialog(QWidget *parent, QVariant grupo, AcolitoDatabase *db) :
    QDialog(parent),
    database(db),
    IDgrupo(grupo),
    modeloHabitaciones(0),
    filtroLibres(0),
    modelo(0),
    ui(new Ui::AlojarGrupoDialog)
{
    ui->setupUi(this);

    if(database) {
        modeloHabitaciones=database->getModeloHabitaciones();
        modeloHabitaciones->setFilterKeyColumn(0);
        modeloHabitaciones->setFilterCaseSensitivity(Qt::CaseInsensitive);

        filtroLibres=new QSortFilterProxyModel(this);
        filtroLibres->setSourceModel(modeloHabitaciones);
        filtroLibres->setFilterKeyColumn(6);

        modelo=new ModeloSeleccion(this);
        modelo->setSourceModel(filtroLibres);
        modelo->setColumnaIdentificador(0);
        modelo->setColumnaSeleccionable(0);
        modelo->setSeleccionExclusiva(true);

        ui->vistaHabitaciones->setModel(modelo);

        ui->searchLine->setEmptyMessage(tr("Buscar codigo de habitacion"));

        ui->vistaHabitaciones->setColumnHidden(2,true);
        ui->vistaHabitaciones->setColumnHidden(3,true);
        ui->vistaHabitaciones->setColumnHidden(5,true);

        ui->vistaHabitaciones->setItemDelegateForColumn(1,new TiposHabDelegate(this,database));
        ui->vistaHabitaciones->setItemDelegateForColumn(4,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
        ui->vistaHabitaciones->setItemDelegateForColumn(6,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));

        //Ajuste de version
#if QT_VERSION >=0x050000
        ui->vistaHabitaciones->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
#else
        ui->vistaHabitaciones->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);
#endif
        connect(ui->searchLine,SIGNAL(textChanged(QString)),modeloHabitaciones,SLOT(setFilterFixedString(QString)));
        connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(vigilaBoton(QAbstractButton*)));
        connect(ui->siLibreBox,SIGNAL(clicked()),this,SLOT(filtrarLibres()));
        connect(ui->noLibreBox,SIGNAL(clicked()),this,SLOT(filtrarLibres()));
    }
}

AlojarGrupoDialog::~AlojarGrupoDialog()
{
    delete ui;
}

void AlojarGrupoDialog::vigilaBoton(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::ApplyRole) {
        QVariantList habitacion=modelo->elementosMarcados();

        if(!habitacion.isEmpty()) {
            QVariant grupo=database->grupoAlojadoEn(habitacion.first().toString());

            //Hay un grupo alojado
            if(grupo.isValid()) {
                Asistente *a=new Asistente(this);
                Asistente *b=new Asistente(this);

                a->idGrupo=IDgrupo.toInt();
                b->idGrupo=grupo.toInt();

                database->intercambiarHabitaciones(a,b);
            }
            else {
                Asistente *a=new Asistente(this);
                Habitacion *h=new Habitacion(this);

                a->idGrupo=IDgrupo.toInt();
                h->codigo=habitacion.first().toString();
                database->alojarGrupo(a,h);
            }
            close();
        }
    }
}

void AlojarGrupoDialog::filtrarLibres()
{
    if((ui->siLibreBox->isChecked() && ui->noLibreBox->isChecked()) ||
            (!ui->siLibreBox->isChecked() && !ui->noLibreBox->isChecked())) {
        filtroLibres->setFilterRegExp(tr("0|1"));
    }
    else if(ui->siLibreBox->isChecked() && !ui->noLibreBox->isChecked())
        filtroLibres->setFilterRegExp(tr("1"));
    else if(!ui->siLibreBox->isChecked() && ui->noLibreBox->isChecked())
        filtroLibres->setFilterRegExp(tr("0"));
}
