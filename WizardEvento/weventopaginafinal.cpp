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

#include "weventopaginafinal.h"
#include "ui_weventopaginafinal.h"

#include <QDebug>

WEventoPaginaFinal::WEventoPaginaFinal(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WEventoPaginaFinal)
{
    ui->setupUi(this);

    //Registrar campo de comentario
    registerField("comentario",ui->comentBox,"plainText");

}

WEventoPaginaFinal::~WEventoPaginaFinal()
{
    delete ui;
}

void WEventoPaginaFinal::initializePage()
{
    ui->eventoLine->setText(field("nombreEvento").toString());
    QList<QVariant> fechasSeleccionadas=field("fechasEvento").toList();

    if(!fechasSeleccionadas.isEmpty()) {    //Evitamos un error si la lista esta vacia
        QList<QDate> listaFechas;
        QVariant variant;
        foreach(variant,fechasSeleccionadas) {
            listaFechas.append(variant.toDate());
        }

        //Preparar dias y meses de evento
        QString diasEvento;
        QList<QString> mesesEvento,yearEvento;

        QDate fecha;
        foreach(fecha,listaFechas) {
            diasEvento.append(fecha.toString("d")).append(", ");
            if(!mesesEvento.contains(fecha.toString("MMMM")))   //Añadir si es nuevo
                mesesEvento.append(fecha.toString("MMMM"));
            if(!yearEvento.contains(fecha.toString("yyyy")))    //Idem
                yearEvento.append(fecha.toString("yyyy"));
        }

        diasEvento.remove(diasEvento.length()-2,2); //Eliminamos la ultima coma
        ui->diasLine->setText(diasEvento);

        if(mesesEvento.size()>1) {  //En caso de existir mas de un mes entre las fechas
            QString mesDef,mes;
            foreach(mes,mesesEvento)
                mesDef.append(mes).append(", ");
            mesDef.remove(mesDef.length()-2,2);
            ui->mesesLine->setText(mesDef);
        }
        else ui->mesesLine->setText(mesesEvento.first());

        if(yearEvento.size()>1) {       //En caso de existir mas de un año, por ejemplo diciembre y enero
            QString yearDef,year;
            foreach(year,yearEvento)
                yearDef.append(year).append(", ");
            yearDef.remove(yearDef.length()-2,2);
            ui->yearLine->setText(yearDef);
        }
        else ui->yearLine->setText(yearEvento.first());
    }
    QWizardPage::initializePage();
}

void WEventoPaginaFinal::cleanupPage()
{
    ui->eventoLine->clear();
    ui->diasLine->clear();
    ui->mesesLine->clear();
    ui->yearLine->clear();
}
