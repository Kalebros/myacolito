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

#include "localizacionesdialog.h"
#include "ui_localizacionesdialog.h"

LocalizacionesDialog::LocalizacionesDialog(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::LocalizacionesDialog),
    database(db)
{
    ui->setupUi(this);

    if(database) {
        modeloLocalizaciones=database->getModeloLocalizaciones();

        ui->vistaLocalizaciones->setModel(modeloLocalizaciones);
        mapeador=new QDataWidgetMapper(this);
        mapeador->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

        mapeador->setModel(modeloLocalizaciones);

        mapeador->addMapping(ui->lugarLine,0,"text");

        mapeador->toFirst();

        connect(ui->vistaLocalizaciones->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                mapeador,SLOT(setCurrentModelIndex(QModelIndex)));

        ui->vistaLocalizaciones->setCurrentIndex(modeloLocalizaciones->index(0,0));

        connect(ui->addLocalizacion,SIGNAL(clicked()),this,SLOT(nuevaLocalizacion()));
        connect(ui->delLocalizacion,SIGNAL(clicked()),this,SLOT(borraLocalizacion()));
    }
}

void LocalizacionesDialog::nuevaLocalizacion()
{
    if(database) {
        bool ok;

        QString nLugar=QInputDialog::getText(this,tr("MyAcolito - Nuevo lugar"),tr("Escribe el nombre de la nueva localizacion."),QLineEdit::Normal,QString(),&ok);

        if(ok) {
            database->addLocalizacion(nLugar);
            ui->vistaLocalizaciones->setCurrentIndex(modeloLocalizaciones->index(0,0));
        }
    }
}

void LocalizacionesDialog::borraLocalizacion()
{
    if(database) {
        QStringList listaSustitutos;

        int nLugar=modeloLocalizaciones->rowCount();
        for(int i=0;i<nLugar;i++)
            listaSustitutos << modeloLocalizaciones->index(i,0).data().toString();

        listaSustitutos.removeAll(ui->lugarLine->text());

        bool ok;

        QString sustituto=QInputDialog::getItem(this,tr("MyAcolito - Borrar lugar"),tr("Antes de eliminar el lugar ")+ui->lugarLine->text()+", debe seleccionar un sustituto.",
                                                listaSustitutos,0,false,&ok);

        if(ok) {
            database->delLocalizacion(ui->lugarLine->text(),sustituto);
            ui->vistaLocalizaciones->setCurrentIndex(modeloLocalizaciones->index(0,0));
        }
    }
}

LocalizacionesDialog::~LocalizacionesDialog()
{
    delete ui;
}
