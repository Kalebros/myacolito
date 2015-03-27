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

#ifndef EVENTODIALOG_H
#define EVENTODIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QSortFilterProxyModel>

#include "delegadoevento.h"

#include <Databases/myeventodb.h>

namespace Ui {
class EventoDialog;
}

/*!
  Muestra los eventos de un modelo
  */

class EventoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EventoDialog(QWidget *parent = 0,QSortFilterProxyModel *mod=0);
    ~EventoDialog();

    Evento *getDatosEvento();

signals:

    void lanzarAsistenteEventos();
    void abrirEvento(Evento *ev);

private:
    Ui::EventoDialog *ui;

    QSortFilterProxyModel *_modelo; ///< Modelo de eventos
    Evento *datosEvento;        ///< Datos del evento seleccionado

    void setupDialog();

private slots:

    void botonPulsado(QAbstractButton *boton);
    void guardarEvento();
};

#endif // EVENTODIALOG_H
