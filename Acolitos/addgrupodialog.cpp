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

#include "addgrupodialog.h"
#include "ui_addgrupodialog.h"

AddGrupoDialog::AddGrupoDialog(QWidget *parent,AcolitoDatabase *db) :
    QDialog(parent),
    ui(new Ui::AddGrupoDialog),
    database(db),
    modeloBase(0)
{
    ui->setupUi(this);
    setup();
}

AddGrupoDialog::~AddGrupoDialog()
{
    delete ui;
}

void AddGrupoDialog::setup()
{
    modeloBase=new ModeloSeleccion(this);
    modeloBase->setSourceModel(database->getModeloAsistentes());

    modeloBase->setColumnaIdentificador(2);
    modeloBase->setColumnaSeleccionable(1);

    filtroBusqueda=new QSortFilterProxyModel(this);
    filtroGrupo=new QSortFilterProxyModel(this);

    filtroBusqueda->setSourceModel(modeloBase);
    filtroGrupo->setSourceModel(filtroBusqueda);

    setupFiltros();
    setupVista();

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(vigilarBotones(QAbstractButton*)));
}

void AddGrupoDialog::setupVista()
{
    ui->tableView->setModel(filtroGrupo);

    ui->tableView->setItemDelegateForColumn(9,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));
    ui->tableView->setItemDelegateForColumn(10,new BooleanIconDelegate(this,QIcon(":/acolitos/iconos/okIcon"),QIcon(":/acolitos/iconos/falseIcon")));

    ui->tableView->setItemDelegateForColumn(7,new CategoriasDelegate(this,database));
    ui->tableView->setColumnHidden(0,true);

    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(8,true);
    ui->tableView->setColumnHidden(11,true);
}

void AddGrupoDialog::setupFiltros()
{
    filtroBusqueda->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filtroGrupo->setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(ui->searchLine,SIGNAL(textChanged(QString)),filtroBusqueda,SLOT(setFilterFixedString(QString)));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(cambiarModoBusqueda(int)));

    cambiarModoBusqueda(ui->comboBox->currentIndex());

    filtroGrupo->setFilterKeyColumn(2); //Columna de valores seleccionados
    filtroGrupo->setFilterRegExp(QString());

    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(cambiarModoFiltrado(int)));
}

void AddGrupoDialog::cambiarModoBusqueda(int modo)
{
    switch(modo) {
    case 0: //Busqueda por nombre
        filtroBusqueda->setFilterKeyColumn(1);
        break;
    case 1: //Busqueda por dni
        filtroBusqueda->setFilterKeyColumn(2);
        break;
    case 2:
        filtroBusqueda->setFilterKeyColumn(3);
        break;
    default:
        break;
    }
    ui->searchLine->clear();
}

void AddGrupoDialog::cambiarModoFiltrado(int modo)
{
    if(modo==Qt::Unchecked) {
        filtroGrupo->setFilterRegExp(QString());
    }
    else {
        QVariantList listaSeleccion=modeloBase->elementosMarcados();
        QString expresionFiltro;
        foreach(QVariant elemento,listaSeleccion) {
            QString identificador=elemento.toString();

            //Genera una expresion regular que lo admita
            expresionFiltro+=identificador.append("|");
        }
        expresionFiltro.chop(1);

        expresionFiltro.prepend("(").append(")");
        filtroGrupo->setFilterRegExp(QRegExp(expresionFiltro));
    }
}

void AddGrupoDialog::vigilarBotones(QAbstractButton *boton)
{
    if(ui->buttonBox->buttonRole(boton)==QDialogButtonBox::ApplyRole) {
            QVariantList listadoDNI=modeloBase->elementosMarcados();
            QList<Asistente*> datosAsistentes;
            Asistente *asis;
            foreach(QVariant dni,listadoDNI) {
                asis=new Asistente(this);
                asis->DNI=dni.toString();
                datosAsistentes << asis;
            }
            database->nuevoGrupoAsistentes(datosAsistentes);
            close();
    }
}
