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

#include "gestorpaginas.h"

GestorPaginas::GestorPaginas(QObject *parent, SlidingStackedWidget *sWidget) :
    QObject(parent),
    _stackedWidget(sWidget)
{
    _mapeador=new QSignalMapper(this);
    _totalPaginas=0;
}

int GestorPaginas::getNumPaginas() const
{
    return _totalPaginas;
}

void GestorPaginas::setSlidingStackedWidget(SlidingStackedWidget *sWidget)
{
    if(_stackedWidget) {
        _mapeador->disconnect();
    }
    _stackedWidget=sWidget;
    _totalPaginas=0;

    connect(_mapeador,SIGNAL(mapped(int)),_stackedWidget,SLOT(slideInIndice(int)));
}

void GestorPaginas::addPageWidget(QAction *accion, QWidget *widget)
{
    _stackedWidget->insertWidget(_totalPaginas,widget);

    _mapeador->setMapping(accion,_totalPaginas++);
    connect(accion,SIGNAL(toggled(bool)),_mapeador,SLOT(map()));
}
