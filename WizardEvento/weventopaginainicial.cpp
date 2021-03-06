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

#include "weventopaginainicial.h"
#include "ui_weventopaginainicial.h"

WEventoPaginaInicial::WEventoPaginaInicial(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WEventoPaginaInicial)
{
    ui->setupUi(this);

    //Registramos los campos de nombre y fechas para que sean accesibles por otras paginas
    this->registerField("nombreEvento*",ui->nombreLineEdit);
    this->registerField("fechasEvento",ui->calendarWidget,"fechas");
}

WEventoPaginaInicial::~WEventoPaginaInicial()
{
    delete ui;
}

void WEventoPaginaInicial::initializePage()
{
    ui->calendarWidget->reset();

    QWizardPage::initializePage();
}
