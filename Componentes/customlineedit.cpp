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

#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent)
                : QLineEdit( parent )
{

}

CustomLineEdit::~CustomLineEdit()
{

}

void CustomLineEdit::setEmptyMessage( const QString &msg ) {
    emptyMessage = msg;
    drawEmptyMsg = text().isEmpty();
    update();
}

void CustomLineEdit::paintEvent( QPaintEvent *ev ) {
    QLineEdit::paintEvent( ev );

    if ( text().isEmpty() ) {
        QPainter p(this);
        QFont f = font();
        f.setItalic(true);
        p.setFont(f);

        QColor color(palette().color(foregroundRole()));
        color.setAlphaF(0.5);
        p.setPen(color);

        QStyleOptionFrame opt;
        initStyleOption(&opt);
        QRect cr = style()->subElementRect(QStyle::SE_LineEditContents, &opt, this);
        cr.setLeft(cr.left() + 2);
        cr.setRight(cr.right() - 2);

        p.drawText(cr, Qt::AlignLeft|Qt::AlignVCenter, emptyMessage);
    }
}

void CustomLineEdit::focusInEvent( QFocusEvent *ev ) {
    if ( drawEmptyMsg ) {
        drawEmptyMsg = false;
        update();
    }
    QLineEdit::focusInEvent( ev );
}

void CustomLineEdit::focusOutEvent( QFocusEvent *ev ) {
    if ( text().isEmpty() ) {
        drawEmptyMsg = true;
        update();
    }
    QLineEdit::focusOutEvent( ev );
}
