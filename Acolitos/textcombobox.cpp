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

#include "textcombobox.h"

TextComboBox::TextComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

QString TextComboBox::getFechaSeleccionada() const
{
    return this->currentText();
}

void TextComboBox::setFechaSeleccionada(QString fecha)
{
    bool found=false;
    for(int i=0;i<this->count() && !found;i++) {
        if(fecha==this->itemText(i)) {
            found=true;
            setCurrentIndex(i);
        }
    }

    if(!found)
        setCurrentIndex(0);
}
