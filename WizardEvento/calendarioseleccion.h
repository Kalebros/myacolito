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

#ifndef CALENDARIOSELECCION_H
#define CALENDARIOSELECCION_H

#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QList>
#include <QDate>
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QRect>

#include <QtAlgorithms>

/*!
  Widget de selecciÃ³n de fechas para calendarios, proyecto GEFE
  */

class CalendarioSeleccion : public QCalendarWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ getColor WRITE setColor)
    Q_PROPERTY(QList<QVariant> fechas READ getFechas)

public:
    explicit CalendarioSeleccion(QWidget *parent = 0,QColor c=QColor(Qt::green));

    /*!
      Devuelve el color asignado para el fondo de las fechas seleccionadas
      @return Color seleccionado de fondo
      @note No modifica el objeto
      */
    QColor getColor() const;

    /*!
      Devuelve las fechas seleccionadas, en forma de lista de tipo QVariant
      @return Lista de fechas en formato QVariant, sin ordenar.
      @note El orden de las fechas es el orden en el que se seleccionaron los dias
      @note No modifica el objeto
      */
    QList<QVariant> getFechas() const;

protected:

    /*!
      Metodo reescrito, dibuja las fechas seleccionadas con el fondo indicado
      */
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

signals:

public slots:

    /*!
      Establece el color de fondo de las fechas seleccionadas.
      @param c  Color de fondo para las fechas seleccionadas
      @note Modifica el objeto
      */
    void setColor(QColor c);

    /*!
      Limpia la lista de fechas seleccionadas
      @note Modifica el objeto
      */
    void reset();

private:

    QList<QDate> listaFechas;   ///< Lista de fechas seleccionadas
    QColor colorBase;           ///< Color de fondo para fechas seleccionadas, por defecto Qt::green

    /*!
      Prepara el cuadro
      */

    void setup();

private slots:

    /*!
      Seleccionar origen
      */

    void setNuevaFecha(const QDate &fecha);
};

#endif // CALENDARIOSELECCION_H
