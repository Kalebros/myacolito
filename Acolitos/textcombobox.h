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

#ifndef TEXTCOMBOBOX_H
#define TEXTCOMBOBOX_H

#include <QDate>
#include <QComboBox>
#include <QAbstractItemModel>
#include <QStandardItemModel>

class TextComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString fechaSeleccionada READ getFechaSeleccionada WRITE setFechaSeleccionada)
public:
    explicit TextComboBox(QWidget *parent = 0);

   QString getFechaSeleccionada() const;

signals:

public slots:

    void setFechaSeleccionada(QString fecha);
};

#endif // TextComboBox_H
