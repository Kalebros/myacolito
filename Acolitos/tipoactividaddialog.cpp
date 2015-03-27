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

#include "tipoactividaddialog.h"
#include "ui_tipoactividaddialog.h"

TipoActividadDialog::TipoActividadDialog(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::TipoActividadDialog),
    database(db)
{
    ui->setupUi(this);

    if(database) {
        modeloTipos=database->getModeloTiposActividad();

        ui->vistaTipos->setModel(modeloTipos);

        mapeador=new QDataWidgetMapper(this);
        mapeador->setModel(modeloTipos);
        mapeador->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

        mapeador->addMapping(ui->nombreTipo,0,"text");

        mapeador->toFirst();

        connect(ui->vistaTipos->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));
        connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addTipoActividad()));
        connect(ui->delButton,SIGNAL(clicked()),this,SLOT(delTipoActividad()));
    }
}

TipoActividadDialog::~TipoActividadDialog()
{
    delete ui;
}

void TipoActividadDialog::addTipoActividad()
{
    if(!ui->nombreTipo->text().isNull() && !ui->nombreTipo->text().isEmpty()) {
        bool ok;
        QString nuevoTipo=QInputDialog::getText(this,tr("MyAcolito - Nuevo tipo de actividad"),tr("Escribe el nombre del nuevo tipo de actividad")
                                                ,QLineEdit::Normal,QString(),&ok);

        if(ok) {
            database->addTipoActividad(nuevoTipo);
            ui->vistaTipos->setCurrentIndex(modeloTipos->index(0,0));
        }
    }
}

void TipoActividadDialog::delTipoActividad()
{
    QString tipo=ui->nombreTipo->text();

    if(!tipo.isNull() && !tipo.isEmpty()) {
        QStringList listaSustitutos;
        int nElementos=modeloTipos->rowCount();
        for(int i=0;i<nElementos;i++)
            listaSustitutos << modeloTipos->index(i,0).data().toString();

        listaSustitutos.removeAll(tipo);

        bool ok;
        QString sustituto=QInputDialog::getItem(this,tr("MyAcolito - Borrar tipo de actividad"),tr("Antes de eliminar el tipo ")+tipo+tr(", debe seleccionar un sustituto.")
                                                ,listaSustitutos,0,false,&ok);

        if(ok) {
            database->delTipoActividad(tipo,sustituto);
            ui->vistaTipos->setCurrentIndex(modeloTipos->index(0,0));
        }
    }
}
