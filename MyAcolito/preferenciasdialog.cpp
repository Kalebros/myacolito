/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ram�rez Marti
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

#include "preferenciasdialog.h"
#include "ui_preferenciasdialog.h"

PreferenciasDialog::PreferenciasDialog(QWidget *parent, QString archivoCon, QString archivoSto) :
    QDialog(parent),
    ui(new Ui::PreferenciasDialog)
{
    ui->setupUi(this);
    sto=new ConexionesSQL(this,archivoCon);

    if(sto->isEmpty())
        crearConexionPorDefecto();


    stoScripts=SMStorage::getStorage("QSQLITE",archivoSto);
    if(!stoScripts->isOpen())
        stoScripts->openAlmacen();

    setup();
}

PreferenciasDialog::~PreferenciasDialog()
{
    delete ui;
}

void PreferenciasDialog::setup()
{
    modelo=sto->modeloConexiones();

    ui->conexionesComboBox->setModel(modelo);
    ui->conexionesComboBox->setModelColumn(1);

    mapeador=new QDataWidgetMapper(this);
    mapeador->setModel(modelo);

    mapeador->addMapping(ui->conexionNameLine,1);   //Nombre= Columna 1
    mapeador->addMapping(ui->conexionHostLine,3);   //Host= Columna3
    mapeador->addMapping(ui->conexionPortSpinBox,5);    //Puerto= Columna 5
    mapeador->addMapping(ui->conexionDefaultCheckBox,6,"estado");    //Predeterminada= 6

    mapeador->toFirst();

    connect(ui->conexionesComboBox,SIGNAL(currentIndexChanged(int)),mapeador,SLOT(setCurrentIndex(int)));

    buscarConexionPredeterminada();

    connect(ui->removeConexionButton,SIGNAL(clicked()),this,SLOT(delConexion()));
    connect(ui->addConexionButton,SIGNAL(clicked()),this,SLOT(crearConexion()));

    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(cambiarPantalla(int)));

    ui->listWidget->setCurrentRow(0);

    //SMStorage
    ui->scriptContainer->setSMStorage(stoScripts);

    connect(ui->scriptContainer,SIGNAL(execScript(SMScript*)),this,SIGNAL(ejecutarScript(SMScript*)));
}

void PreferenciasDialog::crearConexionPorDefecto()
{
    DatosConexion datos;
    datos.setConnectionName(tr("localhost"));
    datos.setDatabaseName(tr("Eventos"));
    datos.setHostName(tr("http://127.0.0.1"));
    datos.setPort(3306);
    sto->addConexion(datos,tr("QMYSQL"),true);
}

void PreferenciasDialog::buscarConexionPredeterminada()
{
    QString nombre=sto->conexionPredeterminada();

    if(!nombre.isEmpty() || !nombre.isNull()) {
        int idx=ui->conexionesComboBox->findText(nombre);
        ui->conexionesComboBox->setCurrentIndex(idx);
    }
    else ui->conexionesComboBox->setCurrentIndex(0);
}

void PreferenciasDialog::delConexion()
{
    QString conexion=ui->conexionesComboBox->currentText();

    sto->removeConexion(conexion);
}

void PreferenciasDialog::crearConexion()
{
    ConexionDialog *dialogo=new ConexionDialog(this);

    if(dialogo->exec()==QDialog::Accepted) {
        DatosConexion data=dialogo->getDatosConexion();
        bool predeter=dialogo->getDefault();

        if((!data.getConnectionName().isEmpty() || !data.getConnectionName().isNull()) &&
           (!data.getHostName().isEmpty() || !data.getHostName().isNull())) {
            sto->addConexion(data,tr("QMYSQL"),predeter);
            buscarConexionPredeterminada();
        }
    }
    delete dialogo;
}

void PreferenciasDialog::cambiarPantalla(int indice)
{
    ui->stackedWidget->setCurrentIndex(indice);
}
