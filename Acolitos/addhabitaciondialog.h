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

#ifndef ADDHABITACIONDIALOG_H
#define ADDHABITACIONDIALOG_H

#include <QDialog>

#include <QDialogButtonBox>

#include <Databases/acolitodatabase.h>
#include <Databases/mymodelohabitaciones.h>

namespace Ui {
class AddHabitacionDialog;
}

/*!
  Añade una nueva habitacion al sistema
  */

class AddHabitacionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddHabitacionDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~AddHabitacionDialog();

public slots:

    /*!
      Cambia la base de datos empleada
      @param db Nueva conexion AcolitoDatabase
      @note Modifica el objeto
      */

    void setAcolitoDatabase(AcolitoDatabase *db);

    /*!
      Resetea y lanza el dialogo para añadir nuevas habitaciones
      */

    void lanzaDialogo();
    
private:
    Ui::AddHabitacionDialog *ui;

    AcolitoDatabase *_db;   ///< Database empleada
    QSortFilterProxyModel *modeloTipos; ///< Modelo de tipos de habitaciones

    /*!
      Limpia los datos de los campos
      */

    void limpiarCampos();

private slots:

    /*!
      Activar la reserva, para abrir el campo de concepto de reserva
      */

    void activarReserva(int estado);

    /*!
      Guardar la habitacion
      */

    void guardarHabitacion();

    /*!
      Procesa la señal del boton
      */

    void procesarBoton(QAbstractButton *boton);
};

#endif // ADDHABITACIONDIALOG_H
