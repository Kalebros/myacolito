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

#include "alojarasistentedialog.h"
#include "ui_alojarasistentedialog.h"

AlojarAsistenteDialog::AlojarAsistenteDialog(QWidget *parent, QVariant asis, AcolitoDatabase *db) :
    QDialog(parent),
    idAsistente(asis),
    database(db),
    mapeador(0),
    ui(new Ui::AlojarAsistenteDialog)
{
    ui->setupUi(this);

    if(database) {
        modeloHabitaciones=database->getModeloHabitaciones();
        modeloAsistentes=database->getModeloAsistentes();
        modeloBase=new ModeloSeleccion(this);

        modeloHabitaciones->setFilterKeyColumn(0);
        modeloBase->setFilterKeyColumn(1);
        modeloAsistentes->setFilterKeyColumn(6);
        modeloAsistentes->setFilterFixedString(tr("-1"));

        modeloBase->setSourceModel(modeloHabitaciones);
        modeloBase->setSeleccionExclusiva(true);
        modeloBase->setColumnaIdentificador(0);
        modeloBase->setColumnaSeleccionable(0);

        ui->comboBox->clear();

        QSortFilterProxyModel *tiposHab=database->getModeloTiposHabitaciones();

        ui->comboBox->insertItem(0,tr("Todas las habitaciones"));

        for(int i=0;i<tiposHab->rowCount();i++)
            ui->comboBox->insertItem(i+1,tiposHab->index(i,0).data(Qt::EditRole).toString());

        ui->vistaHabitaciones->setModel(modeloBase);

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

        ui->vistaAlojados->setModel(modeloAsistentes);

        ui->vistaAlojados->setColumnHidden(0,true);
        ui->vistaAlojados->setColumnHidden(4,true);
        ui->vistaAlojados->setColumnHidden(5,true);
        ui->vistaAlojados->setColumnHidden(6,true);
        ui->vistaAlojados->setColumnHidden(8,true);
        ui->vistaAlojados->setColumnHidden(11,true);

        ui->vistaAlojados->setItemDelegateForColumn(7,new CategoriasDelegate(this,database));

        ui->vistaAlojados->setItemDelegateForColumn(9,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
        ui->vistaAlojados->setItemDelegateForColumn(10,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
        ui->vistaAlojados->setItemDelegateForColumn(11,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));

        mapeador=new QDataWidgetMapper(this);

        mapeador->setModel(modeloBase);
        mapeador->toFirst();

        mapeador->addMapping(ui->codHabLabel,0,"text");

        ui->searchLine->setEmptyMessage(tr("Buscar codigo de habitacion"));

        connect(ui->searchLine,SIGNAL(textChanged(QString)),modeloHabitaciones,SLOT(setFilterFixedString(QString)));
        connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(filtrarTiposHabitaciones(QString)));
        connect(ui->vistaHabitaciones->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(filtrarAlojados(QModelIndex,QModelIndex)));
        connect(ui->vistaHabitaciones->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));

        ui->comboBox->setCurrentIndex(0);

        connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(vigilarBoton(QAbstractButton*)));
    }
}

AlojarAsistenteDialog::~AlojarAsistenteDialog()
{
    delete ui;
}

void AlojarAsistenteDialog::filtrarAlojados(QModelIndex indice, QModelIndex crap)
{
    QString habitacion=modeloBase->index(indice.row(),0).data(Qt::EditRole).toString();

    QVariant grupo=database->grupoAlojadoEn(habitacion);

    if(grupo.isNull() || !grupo.isValid())
        modeloAsistentes->setFilterFixedString(tr("-1"));

    else {
        QString filtro;
        QString id=grupo.toString();
        for(int i=0;i<id.count();i++) {
            filtro.append(id.at(i));
            filtro.append("{1,1}");
        }
        filtro.prepend('^');
        filtro.append("$");

        modeloAsistentes->setFilterRegExp(filtro);
    }
}

void AlojarAsistenteDialog::filtrarTiposHabitaciones(QString tipoHab)
{
    if(tipoHab==tr("Todas las habitaciones")) {
        modeloBase->setFilterFixedString(QString());
    }
    else {
        modeloBase->setFilterFixedString(tipoHab);
    }
}

void AlojarAsistenteDialog::vigilarBoton(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::ApplyRole) {
        QVariantList habitacion=modeloBase->elementosMarcados();

        if(!habitacion.isEmpty()) {
            Asistente *asis=new Asistente(this);
            Habitacion *hab=new Habitacion(this);

            asis->idAsis=idAsistente.toInt();
            hab->codigo=habitacion.first().toString();

            database->realojarAsistente(asis,hab);
            close();
        }
    }
}
