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

#include "weventopaginaplazas.h"
#include "ui_weventopaginaplazas.h"

WEventoPaginaPlazas::WEventoPaginaPlazas(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WEventoPaginaPlazas)
{
    ui->setupUi(this);
    modeloPlazas=0;
    modeloFechas=0;
    setupPage();
}

WEventoPaginaPlazas::~WEventoPaginaPlazas()
{
    delete ui;
}

void WEventoPaginaPlazas::setupPage()
{
    modeloPlazas=new QStandardItemModel(this);
    modeloFechas=new QStandardItemModel(this);

    ui->entradaComboBox->setModel(modeloFechas);
    ui->salidaComboBox->setModel(modeloFechas);

    ui->listView->setModel(modeloPlazas);
    ui->listView->setModelColumn(0);    // Columna de nombre de tipo

    mapeador=new QDataWidgetMapper(this);

    mapeador->setModel(modeloPlazas);
    mapeador->toFirst();

    mapeador->addMapping(ui->categoriaEdit,0);  //Nombre de categoria
    mapeador->addMapping(ui->entradaComboBox,1,"fechaSeleccionada");  //Fecha de entrada
    mapeador->addMapping(ui->salidaComboBox,2,"fechaSeleccionada");   //Fecha de salida
    mapeador->addMapping(ui->precioSpinBox,3);  // Precio de una plaza
    mapeador->addMapping(ui->plazasSpinBox,4);  // Numero de plazas a crear

    mapeador->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    connect(ui->listView->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),mapeador,SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(nuevaCategoria()));
    connect(ui->delButton,SIGNAL(clicked()),this,SLOT(borrarCategorias()));

    registerField("listaCategorias",ui->listView);
}

void WEventoPaginaPlazas::initializePage()
{
    modeloPlazas->clear();
    cleanupPage();
}

void WEventoPaginaPlazas::cleanupPage()
{
    //Nuevo modelo de fechas
    modeloFechas->clear();

    QList<QVariant> listaFechas=this->field("fechasEvento").toList();

    modeloFechas->appendRow(new QStandardItem("Sin asignar"));
    foreach(QVariant variant,listaFechas) {
        QDate fecha=variant.toDate();
        QStandardItem* item=new QStandardItem();
        item->setText(fecha.toString("dd 'de' MMMM,yyyy"));
        modeloFechas->appendRow(item);
    }
}

void WEventoPaginaPlazas::corrigeFechas(QList<QVariant> fechas)
{

    /*
    int numCategorias=modeloPlazas->rowCount();

    foreach(QVariant variant,fechas)
        listaFechas.append(variant.toDate().toString("dd 'de' MMMM"));

    for(int i=0;i<numCategorias;i++) {
        if(!listaFechas.contains(modeloPlazas->item(i,1)->text()))
            modeloPlazas->item(i,1)->setText("Sin asignar");
        if(!listaFechas.contains(modeloPlazas->item(i,2)->text()))
            modeloPlazas->item(i,2)->setText("Sin asignar");
    }
    */
}

void WEventoPaginaPlazas::nuevaCategoria()
{
    QList<QStandardItem *> datosCategoria;

    datosCategoria << new QStandardItem("Sin nombre") << new QStandardItem("Sin asignar");
    datosCategoria << new QStandardItem("Sin asignar");

    QStandardItem *precioPlaza,*numPlazas;

    precioPlaza=new QStandardItem();
    numPlazas=new QStandardItem();
    precioPlaza->setData(QVariant(0),Qt::EditRole);
    numPlazas->setData(QVariant(0),Qt::EditRole);

    datosCategoria << precioPlaza << numPlazas;

    modeloPlazas->appendRow(datosCategoria);
}

void WEventoPaginaPlazas::borrarCategorias()
{
    QModelIndexList listaSeleccion=ui->listView->selectionModel()->selectedIndexes();

    qSort(listaSeleccion);

    foreach(QModelIndex index,listaSeleccion) {
        modeloPlazas->removeRow(index.row());
    }
}

bool WEventoPaginaPlazas::validatePage()
{
    int numFilas=modeloPlazas->rowCount();
    bool valido=true;
    for(int i=0;i<numFilas && valido;i++) {
        if(modeloPlazas->data(modeloPlazas->index(i,1),Qt::EditRole).toString() == QString("Sin asignar"))
            valido=false;
        if(modeloPlazas->data(modeloPlazas->index(i,2),Qt::EditRole).toString() == QString("Sin asignar"))
            valido=false;
    }

    if(!valido)
        QMessageBox::warning(this,"Advertencia de asistente","Hay categorias sin fechas asignadas. Por favor, revisa las categorias");

    return valido;
}
