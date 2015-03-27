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

#include "conexiondialog.h"
#include "ui_conexiondialog.h"

ConexionDialog::ConexionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConexionDialog)
{
    ui->setupUi(this);
    nConexion.setPort(3306);
    nConexion.setDatabaseName(tr("Eventos"));
    isDefault=false;

    connect(ui->conexionLine,SIGNAL(textChanged(QString)),this,SLOT(setNombreConexion(QString)));
    connect(ui->hostLine,SIGNAL(textChanged(QString)),this,SLOT(setHostName(QString)));
    connect(ui->portSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setPort(int)));
    connect(ui->predeterCheckBox,SIGNAL(stateChanged(int)),this,SLOT(setPredeterminado(int)));
}

ConexionDialog::~ConexionDialog()
{
    delete ui;
}

void ConexionDialog::setNombreConexion(QString nombre)
{
    nConexion.setConnectionName(nombre);
}

void ConexionDialog::setHostName(QString name)
{
    nConexion.setHostName(name);
}

void ConexionDialog::setPort(int port)
{
    nConexion.setPort(port);
}

void ConexionDialog::setPredeterminado(int estado)
{
    isDefault=(bool)estado;
}

DatosConexion ConexionDialog::getDatosConexion() const
{
    return nConexion;
}
