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

#ifndef ACOLITOBINARYTOOLBUTTON_H
#define ACOLITOBINARYTOOLBUTTON_H

#include <QToolButton>
#include <QVariant>
#include <QVariantList>
#include <QPair>
#include <QIcon>
#include <QString>

class AcolitoBinaryToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QIcon *iconoTrue READ getIconoTrue WRITE setIconoTrue)
    Q_PROPERTY(QIcon *iconoFalse READ getIconoFalse WRITE setIconoFalse)
    Q_PROPERTY(int estadoBinario READ getEstado WRITE setEstado)
public:
    explicit AcolitoBinaryToolButton(QWidget *parent = 0);
    QIcon *getIconoTrue() const;
    QIcon *getIconoFalse() const;
    int getEstado() const;

    void setIconoTrue(QIcon *icono);
    void setIconoFalse(QIcon *icono);

signals:

    void changedValorActual(int valor);

public slots:

    void setEstado(int estado);

private slots:

    void cambiarEstado();

private:

    QPair<QIcon*,QIcon*> _parIconos;
    bool _estado;
    
};

#endif // AcolitoBinaryToolButton_H
