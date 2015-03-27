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

#ifndef BOOLCHECKBOX_H
#define BOOLCHECKBOX_H

#include <QCheckBox>

/*!
  CheckBox que devuelve el valor booleano seleccionado
  */

class BoolCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(int estado READ getEstado WRITE setEstado)
public:
    explicit BoolCheckBox(QWidget *parent = 0);

    int getEstado() const;

signals:

public slots:

    void setEstado(int nuevoEstado);

};

#endif // BOOLCHECKBOX_H
