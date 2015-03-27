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

#include "acolitobinarytoolbutton.h"

AcolitoBinaryToolButton::AcolitoBinaryToolButton(QWidget *parent) :
    QToolButton(parent)
{
    _estado=false;
    connect(this,SIGNAL(clicked()),this,SLOT(cambiarEstado()));
}

void AcolitoBinaryToolButton::setIconoTrue(QIcon *icono)
{
    _parIconos.first=icono;
}

void AcolitoBinaryToolButton::setIconoFalse(QIcon *icono)
{
    _parIconos.second=icono;
}

void AcolitoBinaryToolButton::setEstado(int estado)
{
    switch(estado) {
    case 0:
        setIcon(*_parIconos.second);
        _estado=false;
        break;
    case 1:
        setIcon(*_parIconos.first);
        _estado=true;
        break;
    default:
        break;
    }

    emit changedValorActual(_estado);
}

QIcon *AcolitoBinaryToolButton::getIconoTrue() const
{
    return _parIconos.first;
}

QIcon *AcolitoBinaryToolButton::getIconoFalse() const
{
    return _parIconos.second;
}

int AcolitoBinaryToolButton::getEstado() const
{
    return (int)_estado;
}

void AcolitoBinaryToolButton::cambiarEstado()
{
    setEstado((int)(!_estado));
}
