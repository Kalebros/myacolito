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

#ifndef CONEXIONDIALOG_H
#define CONEXIONDIALOG_H

#include <QDialog>

#include <Databases/datosconexion.h>

namespace Ui {
class ConexionDialog;
}

class ConexionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConexionDialog(QWidget *parent = 0);
    ~ConexionDialog();

    DatosConexion getDatosConexion() const;

    bool getDefault() const
    { return isDefault; }
    
public slots:

    void setNombreConexion(QString nombre);
    void setHostName(QString name);
    void setPort(int port);
    void setPredeterminado(int estado);

private:
    Ui::ConexionDialog *ui;

    DatosConexion nConexion;
    bool isDefault;         ///< Flag de conexion
};

#endif // CONEXIONDIALOG_H
