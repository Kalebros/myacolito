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

#include "asistentecategoriasdialog.h"
#include "ui_asistentecategoriasdialog.h"

AsistenteCategoriasDialog::AsistenteCategoriasDialog(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::AsistenteCategoriasDialog),
    database(db)
{
    ui->setupUi(this);
    if(database) {
        modeloCategorias=database->getModeloCategorias();

        ui->vistaCategorias->setModel(modeloCategorias);
        ui->vistaCategorias->setModelColumn(0);

        ui->fechaEntradaCombo->setModel(database->getModeloFechas());
        ui->fechaLimiteCombo->setModel(database->getModeloFechas());

        ui->fechaEntradaCombo->setModelColumn(0);
        ui->fechaLimiteCombo->setModelColumn(0);

        mapeador=new QDataWidgetMapper(this);

        mapeador->setModel(modeloCategorias);

        mapeador->addMapping(ui->nombreLine,0,"text");
        mapeador->addMapping(ui->precioSpinBox,1,"value");
        mapeador->addMapping(ui->plazaSpinBox,2,"value");
        mapeador->addMapping(ui->fechaEntradaCombo,3,"fechaSeleccionada");
        mapeador->addMapping(ui->fechaLimiteCombo,4,"fechaSeleccionada");

        connect(ui->vistaCategorias->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));
        connect(ui->nuevaCatButton,SIGNAL(clicked()),this,SLOT(addCategoria()));
        connect(ui->borrarCatButton,SIGNAL(clicked()),this,SLOT(borrarCategoria()));
    }
}

AsistenteCategoriasDialog::~AsistenteCategoriasDialog()
{
    delete ui;
}

void AsistenteCategoriasDialog::nuevaCategoria()
{
    QVariantMap datosCat;

    datosCat.insert(tr("nombre"),tr("Nueva categoria"));
    datosCat.insert(tr("precio"),0);
    datosCat.insert(tr("maximo"),0);
    datosCat.insert(tr("fechaInicio"),ui->fechaEntradaCombo->itemText(0));
    datosCat.insert(tr("fechaLimite"),ui->fechaLimiteCombo->itemText(ui->fechaLimiteCombo->count()-1));

    database->crearCategoriaAsistentes(datosCat);
}

void AsistenteCategoriasDialog::addCategoria()
{
    NuevaCategoriaDialog dialogo(this,database);

    dialogo.exec();

    database->crearCategoriaAsistentes(dialogo.getDatosCategoria());
}

void AsistenteCategoriasDialog::borrarCategoria()
{
    QString categoriaABorrar=modeloCategorias->data(ui->vistaCategorias->currentIndex()).toString();

    if(categoriaABorrar.isNull() || categoriaABorrar.isEmpty())
        return;

    QString mensaje=tr("Antes de borrar la categoria %1, debe de seleccionar otra para sustituir las plazas ya asignadas.").arg(categoriaABorrar);

    QStringList listaCategorias;
    int numCategorias=modeloCategorias->rowCount();

    for(int i=0;i<numCategorias;i++)
        listaCategorias << modeloCategorias->index(i,0).data(Qt::EditRole).toString();

    listaCategorias.removeAll(categoriaABorrar);

    bool ok;
    QString categoriaSustituta=QInputDialog::getItem(this,tr("MyAcolito - Borrar categoria"),mensaje,listaCategorias,0,false,&ok);

    if(ok && !categoriaSustituta.isEmpty()) {
        database->borrarCategoriaAsistentes(categoriaABorrar,categoriaSustituta);
    }
}
