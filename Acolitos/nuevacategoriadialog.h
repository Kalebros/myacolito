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

#ifndef NUEVACATEGORIADIALOG_H
#define NUEVACATEGORIADIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QVariant>
#include <QMap>
#include <QVariantMap>
#include <QMessageBox>

#include <Databases/acolitodatabase.h>

namespace Ui {
class NuevaCategoriaDialog;
}

class NuevaCategoriaDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NuevaCategoriaDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~NuevaCategoriaDialog();

    QVariantMap getDatosCategoria();
    
private:
    Ui::NuevaCategoriaDialog *ui;

    AcolitoDatabase *database;  ///< Conexion con la base de datos

    QVariantMap mapaDatos;  ///< Datos del dialogo

private slots:

    void vigilarButtonBox(QAbstractButton *boton);
};

#endif // NUEVACATEGORIADIALOG_H
