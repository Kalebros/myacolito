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

#ifndef NUEVOTIPOHABDIALOG_H
#define NUEVOTIPOHABDIALOG_H

#include <QDialog>
#include <QVariantMap>
#include <QMessageBox>
#include <QDialogButtonBox>

#include <Databases/acolitodatabase.h>

namespace Ui {
class NuevoTipoHabDialog;
}

class NuevoTipoHabDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NuevoTipoHabDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~NuevoTipoHabDialog();

    QVariantMap getDatosTipoHabitacion();
    
private:
    Ui::NuevoTipoHabDialog *ui;
    AcolitoDatabase *database;

    QVariantMap datosTipoHab;

private slots:

    void vigilaBotones(QAbstractButton *boton);

};

#endif // NUEVOTIPOHABDIALOG_H
