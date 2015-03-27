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

#ifndef VARIANTTOOLBUTTON_H
#define VARIANTTOOLBUTTON_H

#include <QToolButton>
#include <QVariant>

/*!
  Tool Button que almacena un dato de tipo QVariant.

  Este toolButton se puede usar en combinación con un
  QDataWidgetMapper para solventar las carencias de conexion
  que este último posee (si conectas una propiedad de un widget,
  no puedes conectar ninguna otra)
  */

class VariantToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QVariant valor READ getValor WRITE setValor NOTIFY valueChanged)
public:
    explicit VariantToolButton(QWidget *parent = 0);

    /*!
      Devuelve el QVariant almacenado en el QToolButton
      @return QVariant, valor almacenado
      @note No modifica el objeto
      */

    QVariant getValor() const;
    
signals:

    /*!
      Nueva señal clicked
      */

    void clicked(QVariant valor);

    /*!
      Notifica que se ha producido un cambuo de valor,
      con este nuevo valor
      */

    void valueChanged(QVariant valor);

public slots:

    /*!
      Establece el valor QVariant almacenado en el ToolButton
      */

    void setValor(QVariant valor);

private:

    QVariant _valor;    ///< Valor almacenado

private slots:

    void sendNotifySignal();
    
};

#endif // VARIANTTOOLBUTTON_H
