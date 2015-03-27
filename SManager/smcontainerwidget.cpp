/***********************************************************************
** SManager, copyright @2013,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of SManager.
**
** SManager is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SManager is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with SManager.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "smcontainerwidget.h"
#include "ui_smcontainerwidget.h"

SMContainerWidget::SMContainerWidget(QWidget *parent,SMStorage *sto) :
    QWidget(parent),
    ui(new Ui::SMContainerWidget),
    almacen(sto),
    layout(0)
{
    ui->setupUi(this);
    listaWidgets=QList<SMScriptForm*>();

    if(almacen) {
        if(!almacen->isOpen())
            almacen->openAlmacen();

        if(almacen->size()!=0)
            loadScriptForms();
    }

    connect(ui->addScriptButton,SIGNAL(clicked()),this,SLOT(addScript()));
}

void SMContainerWidget::setSMStorage(SMStorage *sto)
{
    if(almacen) {
        resetContainer();
        almacen->disconnect(this);
    }

    almacen=sto;

    if(!almacen->isOpen())
        almacen->openAlmacen();

    if(almacen->size()!=0)
        loadScriptForms();

    connect(ui->addScriptButton,SIGNAL(clicked()),this,SLOT(addScript()));
}

void SMContainerWidget::loadScriptForms()
{
    if(!listaWidgets.isEmpty())
        resetContainer();

    prepararLayout();

    int numScripts=almacen->size();
    SMScriptForm *sForm=0;
    for(int i=0;i<numScripts;i++) {
        sForm=new SMScriptForm(this,almacen,i);
        layout->addWidget(sForm);
        listaWidgets << sForm;

        connect(sForm,SIGNAL(execScript(SMScript*)),this,SIGNAL(execScript(SMScript*)));
        connect(sForm,SIGNAL(deleteScript(int,SMStorage*,SMScriptForm*)),this,SLOT(removeScript(int,SMStorage*,SMScriptForm*)));
    }

    layout->insertStretch(-1);

}

void SMContainerWidget::prepararLayout()
{
    if(layout) {
        delete layout;
        layout=0;
    }

    layout=new QVBoxLayout(ui->widget);
    ui->widget->setLayout(layout);
}

void SMContainerWidget::addScript()
{
    int id=almacen->addScript(tr("Sin nombre"),tr(""),tr(""));

    SMScriptForm *scriptWidget=new SMScriptForm(this,almacen,id);

    layout->insertWidget(listaWidgets.count(),scriptWidget);
    listaWidgets << scriptWidget;
}

void SMContainerWidget::removeScript(int id, SMStorage *sto, SMScriptForm *form)
{
    layout->removeWidget(form);
    delete form;

    almacen->removeScript(id);
    listaWidgets.removeAll(form);
}

void SMContainerWidget::resetContainer()
{
    foreach(SMScriptForm *sForm,listaWidgets) {
        layout->removeWidget(sForm);
        delete sForm;
    }

    listaWidgets.clear();
    prepararLayout();
}

SMContainerWidget::~SMContainerWidget()
{
    delete ui;
}
