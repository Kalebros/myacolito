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

#include "binarytoolbutton.h"

BinaryToolButton::BinaryToolButton(QWidget *parent) :
    QToolButton(parent)
{
}

void BinaryToolButton::setParValores(QVariantList nuevosValores)
{
    _parValores.first=nuevosValores.at(0);
    _parValores.second=nuevosValores.at(1);
}

void BinaryToolButton::setIconoA(QIcon *icono)
{
    _parIconos.first=icono;
}

void BinaryToolButton::setIconoB(QIcon *icono)
{
    _parIconos.second=icono;
}

void BinaryToolButton::setValorActual(QVariant valor)
{
    _valorActual=valor;

    if(_valorActual==_parValores.first)
        this->setIcon(*_parIconos.first);
    else if(_valorActual==_parValores.second)
        this->setIcon(*_parIconos.second);

    emit changedValorActual(_valorActual);
}

QVariantList BinaryToolButton::getParValores() const
{
    QVariantList resultado;
    resultado.append(_parValores.first);
    resultado.append(_parValores.second);

    return resultado;
}

QIcon *BinaryToolButton::getIconoA() const
{
    return _parIconos.first;
}

QIcon *BinaryToolButton::getIconoB() const
{
    return _parIconos.second;
}

QVariant BinaryToolButton::getValorActual() const
{
    return _valorActual;
}
