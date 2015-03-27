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

#include "smscriptform.h"
#include "ui_smscriptform.h"

SMScriptForm::SMScriptForm(QWidget *parent, SMStorage *sto, int idScript) :
    QWidget(parent),
    ui(new Ui::SMScriptForm)
{
    ui->setupUi(this);
    almacen=sto;
    _scriptEnAlmacen=idScript;

    if(almacen->isOpen() && _scriptEnAlmacen!=-1) {
        script=almacen->getScript(_scriptEnAlmacen);
    }

    ui->nombreEdit->setText(script->getNombre());
    ui->pathEdit->setText(script->getPath());
    ui->argumentosEdit->setText(script->getArgumentos());

    //Conectar
    connect(ui->nombreEdit,SIGNAL(textChanged(QString)),script,SLOT(setNombre(QString)));
    connect(ui->pathEdit,SIGNAL(textChanged(QString)),script,SLOT(setPath(QString)));
    connect(ui->argumentosEdit,SIGNAL(textChanged(QString)),script,SLOT(setArgumentos(QString)));

    connect(ui->ejecutarButton,SIGNAL(clicked()),this,SLOT(execClicked()));
    connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(deleteClicked()));
    connect(ui->pathButton,SIGNAL(clicked()),this,SLOT(selectPath()));
}

SMScriptForm::~SMScriptForm()
{
    delete ui;
}

void SMScriptForm::execClicked()
{
    emit execScript(script);
}

void SMScriptForm::deleteClicked()
{
    emit deleteScript(_scriptEnAlmacen,almacen,this);
}

void SMScriptForm::selectPath()
{
    QString archivo=QFileDialog::getOpenFileName(this,tr("Seleccionar archivo ejecutable"));

    if(!archivo.isEmpty() || !archivo.isNull())
        ui->pathEdit->setText(archivo);
}
