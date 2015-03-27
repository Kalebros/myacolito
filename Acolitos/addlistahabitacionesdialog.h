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

#ifndef ADDLISTAHABITACIONESDIALOG_H
#define ADDLISTAHABITACIONESDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

#include <Databases/acolitodatabase.h>
#include <Databases/mymodelohabitaciones.h>

namespace Ui {
class AddListaHabitacionesDialog;
}

class AddListaHabitacionesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddListaHabitacionesDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~AddListaHabitacionesDialog();
    
private:
    Ui::AddListaHabitacionesDialog *ui;
    AcolitoDatabase *_db;
    QSortFilterProxyModel *modeloTiposHabitaciones; ///< Modelo de tipos de habitaciones

    /*!
      Genera el listado de habitaciones especificado
      */

    void generaHabitaciones();

    /*!
      Bloquea la entrada de datos y hace visible el progress bar
      */

    void setProgressBarOn(bool modo);

private slots:

    void vigilaBoton(QAbstractButton *boton);

};

#endif // ADDLISTAHABITACIONESDIALOG_H
