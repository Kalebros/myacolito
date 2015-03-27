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

#ifndef INTCHECKBOX_H
#define INTCHECKBOX_H

#include <QCheckBox>

/*!
  CheckBox de valores enteros (0,1)
  @brief
  La clase QCheckBox tiene señales que solo devuelven el estado
  en modo bool; para trabajar correctamente con bases de datos
  como SQLite, donde el valor booleano es almacenado como valor entero
  0/1, utilizo esta clase.
  */

class IntCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(int estado READ getEstado WRITE setEstado)
public:
    explicit IntCheckBox(QWidget *parent = 0);

    int getEstado() const;

signals:
    
public slots:

    void setEstado(int estado);

private slots:

private:

};

#endif // INTCHECKBOX_H
