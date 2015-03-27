/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ram�rez Marti
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

#ifndef ACOLITOTEXTCOMBOBOX_H
#define ACOLITOTEXTCOMBOBOX_H

#include <QComboBox>
#include <QString>

class AcolitoTextComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString texto READ getTextoActual WRITE setTextoActual)
public:
    explicit AcolitoTextComboBox(QWidget *parent = 0);

    QString getTextoActual() const;

signals:

public slots:

    void setTextoActual(QString texto);
};

#endif // ACOLITOTEXTCOMBOBOX_H