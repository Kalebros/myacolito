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

#include "wizardevento.h"

WizardEvento::WizardEvento(QWidget *parent) :
    QWizard(parent)
{
    setup();
    crearPaginaInicio();
    crearPaginaPlazas();
    crearPaginaFinal();
}

void WizardEvento::setIcono(QIcon icon)
{
    iconoApp=icon;
    setWindowIcon(iconoApp);
    this->setPixmap(QWizard::LogoPixmap,iconoApp.pixmap(iconoApp.actualSize(QSize(25,25))).scaledToHeight(60,Qt::SmoothTransformation));
}

void WizardEvento::crearPaginaInicio()
{
    WEventoPaginaInicial *paginaInicial=new WEventoPaginaInicial(this);

    addPage(paginaInicial);
}

void WizardEvento::crearPaginaPlazas()
{
    WEventoPaginaPlazas *paginaPlazas=new WEventoPaginaPlazas(this);
    addPage(paginaPlazas);
}

void WizardEvento::crearPaginaFinal()
{
    WEventoPaginaFinal *paginaFinal=new WEventoPaginaFinal(this);

    addPage(paginaFinal);
}

void WizardEvento::setup()
{
    connect(this,SIGNAL(accepted()),this,SLOT(enviarNuevoEvento()));
    setWindowTitle(tr("MyAcolito - Asistente de nuevos eventos"));
}

void WizardEvento::enviarNuevoEvento()
{
    QStandardItemModel *modelo;
    WEventoPaginaPlazas *paginaPlazas=qobject_cast<WEventoPaginaPlazas*>(page(1));

    modelo=paginaPlazas->getModeloPlazas();

    //Convertir datos a evento
    Evento *nuevoEvento=new Evento();
    nuevoEvento->nombre(field("nombreEvento").toString());
    nuevoEvento->comentario(field("comentario").toString());
    nuevoEvento->addFecha(field("fechasEvento").toList());
    QString database=field("nombreEvento").toString();
    database=database.replace(QChar(' '),QChar('_'));
    database=database.toLower();
    nuevoEvento->database(database);

    emit this->datosEvento(nuevoEvento,modelo);
}

void WizardEvento::lanzarAsistente()
{
    restart();
    show();
}
