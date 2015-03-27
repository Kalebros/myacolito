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

#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include <Componentes/customlineedit.h>

#include <Databases/datosconexion.h>

#include "conexionessql.h"
#include "conexiondialog.h"

namespace Ui {
class LogDialog;
}

/*!
  Dialogo de conexion con una base de datos
  */
class LogDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LogDialog(QWidget *parent = 0, QString pathStorage=QString());
    ~LogDialog();

    /*!
      Devuelve los datos de la conexion seleccionada
      */
    DatosConexion getConexion() const;

public slots:

    /*!
      Lanza un dialogo para añadir una nueva conexion
      */
    void addConexion();

    void accept();
    
private:
    Ui::LogDialog *ui;
    ConexionesSQL *conexionesStorage;
    DatosConexion _conexionSeleccionada;    ///< Datos de la conexion seleccionada

    /*!
      Establece la conexion predeterminada como la
      conexion actual del comboBox
      */
    void seleccionarConexionPredeterminada();

    /*!
      Introduce una conexion por defecto en el storage
      */

    void addConexionPredeterminada();
};

#endif // LOGDIALOG_H
