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

#ifndef GESTORPAGINAS_H
#define GESTORPAGINAS_H

#include <QObject>
#include <QSignalMapper>
#include <QAction>

#include <Componentes/slidingstackedwidget.h>

/*!
  Gestiona los cambios de pagina en un SlidingStackedWidget
  */

class GestorPaginas : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int numPaginas READ getNumPaginas)
public:
    explicit GestorPaginas(QObject *parent = 0,SlidingStackedWidget *sWidget=0);
    
    /*!
      Devuelve el numero de paginas del slidingstackedWidget
      */
    int getNumPaginas() const;

signals:
    
public slots:

    /*!
      Añade un nuevo widget al stackedWidget, asociado con una accion para cambiar de pagina
      al pulsarse en ella
      */
    void addPageWidget(QAction *accion,QWidget *widget);

    /*!
      Establece un nuevo slidingStackedWidget
      */
    void setSlidingStackedWidget(SlidingStackedWidget *sWidget);

private:

    int _totalPaginas;  ///< Paginas totales del slidingStackedWidget
    QSignalMapper *_mapeador;   ///< Mapeador de señales

    SlidingStackedWidget *_stackedWidget;   ///< SlidingStackedWidget
};

#endif // GESTORPAGINAS_H
