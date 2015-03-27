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

#include "delegadoevento.h"

DelegadoEvento::DelegadoEvento(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QSize DelegadoEvento::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QIcon icon=qvariant_cast<QIcon>(index.data(Qt::DisplayRole));
    QSize iconSize=icon.actualSize(option.decorationSize);
    QFont fuente=QApplication::font();
    QFontMetrics fmt(fuente);

    return QSize(iconSize.width(),iconSize.height()+fmt.height()+40);
}

void DelegadoEvento::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();

    //Datos del evento
    QString evento=index.data(MyModeloEventos::Nombre).toString();
    QString comentario=index.data(MyModeloEventos::Comentario).toString();
    QString fechas=index.data(MyModeloEventos::Fechas).toString();
    QIcon icono=qvariant_cast<QIcon>(index.data(MyModeloEventos::Icono));

    //Datos adicionales
    QFont fuente=QApplication::font();
    QFont subfuente=QApplication::font();
    fuente.setBold(true);
    subfuente.setWeight(subfuente.weight()-2);
    subfuente.setItalic(true);
    QFontMetrics fmt(fuente);

    //Medidas
    QSize iconSize=icono.actualSize(option.decorationSize);
    QRect rectCabecera=option.rect;
    QRect rectSubcabecera=option.rect;
    QRect rectIcono=rectSubcabecera;

    rectIcono.setRight(iconSize.width()+20);
    rectIcono.setTop(rectIcono.top()+5);
    rectCabecera.setLeft(rectIcono.right());
    rectSubcabecera.setLeft(rectIcono.right());
    rectCabecera.setTop(rectCabecera.top()+5);
    rectCabecera.setBottom(rectCabecera.top()+fmt.height());

    rectSubcabecera.setTop(rectCabecera.bottom()+2);

    //Dibujar
    painter->drawPixmap(QPoint(rectIcono.left()+iconSize.width()/2+2,rectIcono.top()+iconSize.height()/2+3),icono.pixmap(iconSize.width(),iconSize.height()));

    painter->setFont(fuente);
    painter->drawText(rectCabecera,evento);
    painter->setFont(subfuente);
    painter->drawText(rectSubcabecera.left(),rectSubcabecera.top()+17,fechas);

    painter->restore();
}
