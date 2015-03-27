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

#ifndef BINARYTOOLBUTTON_H
#define BINARYTOOLBUTTON_H

#include <QToolButton>
#include <QVariant>
#include <QVariantList>
#include <QPair>
#include <QIcon>
#include <QString>

class BinaryToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QVariantList parValores READ getParValores WRITE setParValores)
    Q_PROPERTY(QIcon *iconoA READ getIconoA WRITE setIconoA)
    Q_PROPERTY(QIcon *iconoB READ getIconoB WRITE setIconoB)
    Q_PROPERTY(QVariant valorActual READ getValorActual WRITE setValorActual NOTIFY changedValorActual)

public:
    explicit BinaryToolButton(QWidget *parent = 0);
    QVariantList getParValores() const;
    QIcon *getIconoA() const;
    QIcon *getIconoB() const;
    QVariant getValorActual() const;

    void setParValores(QVariantList nuevosValores);
    void setIconoA(QIcon *icono);
    void setIconoB(QIcon *icono);
    void setValorActual(QVariant valor);

signals:

    void changedValorActual(QVariant valor);

public slots:

private:

    QPair<QVariant,QVariant> _parValores;
    QPair<QIcon*,QIcon*> _parIconos;
    QVariant _valorActual;
    
};

#endif // BINARYTOOLBUTTON_H
