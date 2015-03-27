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

#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent,QString pathStorage) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
    ui->userLine->setEmptyMessage(tr("Usuario"));
    ui->passwordLine->setEmptyMessage(QString::fromLatin1("Contraseña")); //Este archivo esta codificado en latin 1

    conexionesStorage=new ConexionesSQL(this,pathStorage);

    if(conexionesStorage->isEmpty())
        addConexionPredeterminada();

    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(addConexion()));

    ui->comboBox->setModel(conexionesStorage->modeloConexiones());
    ui->comboBox->setModelColumn(1);

    seleccionarConexionPredeterminada();
}

void LogDialog::seleccionarConexionPredeterminada()
{
    QString predeterminada=conexionesStorage->conexionPredeterminada();

    if(!predeterminada.isEmpty() || !predeterminada.isNull()) {
        int pos=ui->comboBox->findText(predeterminada);
        ui->comboBox->setCurrentIndex(pos);
    }
    else ui->comboBox->setCurrentIndex(0);
}

void LogDialog::addConexion()
{
    ConexionDialog *dialog=new ConexionDialog(this);

    int res=dialog->exec();
    if(res==QDialog::Accepted) {

        DatosConexion data=dialog->getDatosConexion();
        bool predeter=dialog->getDefault();

        if((!data.getConnectionName().isEmpty() || !data.getConnectionName().isNull()) &&
           (!data.getHostName().isEmpty() || !data.getHostName().isNull())) {
            conexionesStorage->addConexion(data,tr("QMYSQL"),predeter);
            seleccionarConexionPredeterminada();
        }
    }

    delete dialog;
}

void LogDialog::addConexionPredeterminada()
{
    DatosConexion conexion;
    conexion.setConnectionName(tr("localhost"));
    conexion.setDatabaseName(tr("Eventos"));
    conexion.setHostName(tr("http://127.0.0.1"));
    conexion.setPort(3306);

    conexionesStorage->addConexion(conexion,tr("QMYSQL"),true);
}

DatosConexion LogDialog::getConexion() const
{
    return _conexionSeleccionada;
}

void LogDialog::accept()
{
    QString conexion=ui->comboBox->currentText();
    _conexionSeleccionada=conexionesStorage->getConexion(conexion);

    QPair<QString,QString> login;
    login.first=ui->userLine->text();
    login.second=ui->passwordLine->text();

    _conexionSeleccionada.setLogin(login);

    QDialog::accept();
}

LogDialog::~LogDialog()
{
    delete ui;
}
