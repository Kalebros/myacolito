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

#include "varianttoolbutton.h"

VariantToolButton::VariantToolButton(QWidget *parent) :
    QToolButton(parent),
    _valor(QVariant())
{
    connect(this,SIGNAL(clicked()),this,SLOT(sendNotifySignal()));
}

QVariant VariantToolButton::getValor() const
{
    return _valor;
}

void VariantToolButton::setValor(QVariant valor)
{
    _valor=valor;

    emit valueChanged(_valor);
}

void VariantToolButton::sendNotifySignal()
{
    emit clicked(_valor);
}
