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

#ifndef ACOLITOVARIANTBUTTON_H
#define ACOLITOVARIANTBUTTON_H

#include <QToolButton>
#include <QRegExp>
#include <QVariant>

class AcolitoVariantButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QVariant valorBoton READ getValor WRITE setValor NOTIFY valueChanged)
public:
    explicit AcolitoVariantButton(QWidget *parent = 0);
    QVariant getValor() const;
    
signals:

    void valueChanged(QVariant valor);
    void valueChangedRegExp(QString expresion);
    void valueChangedString(QString cadena);
    void botonPulsado(QVariant valor);
    void botonPulsadoString(QString valor);
    void botonPulsadoInt(int valor);
    
public slots:

    void setValor(QVariant valor);

private:

    QVariant _valorAlmacenado;

    QString generarExpresionRegular();
private slots:

    void clickedValue();    
};

#endif // ACOLITOVARIANTBUTTON_H
