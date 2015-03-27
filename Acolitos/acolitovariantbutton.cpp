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

#include "acolitovariantbutton.h"

AcolitoVariantButton::AcolitoVariantButton(QWidget *parent) :
    QToolButton(parent)
{
    _valorAlmacenado=QVariant();
    connect(this,SIGNAL(clicked()),this,SLOT(clickedValue()));
}

QVariant AcolitoVariantButton::getValor() const
{
    return _valorAlmacenado;
}

void AcolitoVariantButton::setValor(QVariant valor)
{
    _valorAlmacenado=valor;
    if(_valorAlmacenado.canConvert(QVariant::String)||_valorAlmacenado.canConvert(QVariant::Int)) {
        emit valueChangedRegExp(generarExpresionRegular());
        emit valueChangedString(valor.toString());
    }
    emit valueChanged(valor);
}


void AcolitoVariantButton::clickedValue()
{
    emit botonPulsado(_valorAlmacenado);
    if (_valorAlmacenado.canConvert(QVariant::String))
            emit botonPulsadoString(_valorAlmacenado.toString());
    if(_valorAlmacenado.canConvert(QVariant::Int))
        emit botonPulsadoInt(_valorAlmacenado.toInt());
}

QString AcolitoVariantButton::generarExpresionRegular()
{
    QString numero=_valorAlmacenado.toString();

    QString res;
    for(int i=0;i<numero.count();i++) {
        res.append(numero.at(i));
        res.append("{1,1}");
    }
    res.prepend('^');
    res.append("$");

    return res;
}
