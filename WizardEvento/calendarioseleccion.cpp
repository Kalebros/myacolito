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

#include "calendarioseleccion.h"

#include <QDebug>

CalendarioSeleccion::CalendarioSeleccion(QWidget *parent,QColor c) :
    QCalendarWidget(parent),
    colorBase(c)
{
    setup();
}

void CalendarioSeleccion::setup()
{

    this->setCurrentPage(QDate::currentDate().year(),QDate::currentDate().month());
    this->setMinimumDate(QDate::currentDate());

    connect(this,SIGNAL(activated(QDate)),this,SLOT(setNuevaFecha(QDate)));
}

QColor CalendarioSeleccion::getColor() const
{
    return this->colorBase;
}

QList<QVariant> CalendarioSeleccion::getFechas() const
{
    QList<QVariant> lista;
    QList<QDate> listado=listaFechas;

    qSort(listado);

    foreach(QDate fecha,listado) {
        lista.append(QVariant(fecha));
    }

    return lista;
}

void CalendarioSeleccion::setColor(QColor c)
{
    colorBase=c;
    this->repaint();
}

void CalendarioSeleccion::setNuevaFecha(const QDate &fecha)
{
    if(!listaFechas.contains(fecha))
    {
        listaFechas.append(fecha);
        updateCell(fecha);
    }
    else
    {
        listaFechas.removeAll(fecha);
        updateCell(fecha);
    }

}

void CalendarioSeleccion::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    if(!listaFechas.contains(date))
        QCalendarWidget::paintCell(painter,rect,date);
    else
    {
        switch((date==this->selectedDate()))
        {
        case true:
            painter->setBrush(this->palette().highlight());
            painter->setPen(QPen(painter->brush().color(),Qt::SolidLine));
            painter->drawRect(rect);
            painter->setPen(QPen(this->palette().highlightedText().color(),Qt::SolidLine));
            painter->drawText(rect,tr(", Evento").prepend(date.toString("dd")),QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
            break;
        case false:
            painter->setBrush(QBrush(colorBase,Qt::SolidPattern));
            painter->setPen(QPen(painter->brush().color(),Qt::SolidLine));
            painter->drawRect(rect);
            painter->setPen(QPen(this->palette().text().color(),Qt::SolidLine));
            painter->drawText(rect,tr(", Evento").prepend(date.toString("dd")),QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
            break;
        }
    }
}

void CalendarioSeleccion::reset()
{
    listaFechas.clear();

    this->setCurrentPage(QDate::currentDate().year(),QDate::currentDate().month());
    this->setMinimumDate(QDate::currentDate());
}
