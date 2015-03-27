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

#ifndef HABITACIONTIPOSDIALOG_H
#define HABITACIONTIPOSDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QVariantMap>
#include <QInputDialog>

#include <Databases/acolitodatabase.h>

#include "nuevotipohabdialog.h"

namespace Ui {
class HabitacionTiposDialog;
}

class HabitacionTiposDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HabitacionTiposDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~HabitacionTiposDialog();
    
private:
    Ui::HabitacionTiposDialog *ui;
    AcolitoDatabase *database;
    QDataWidgetMapper *mapeador;

    QSortFilterProxyModel *modeloTiposHabitacion;

private slots:

    void addTipoHabitacion();

    void borrarTipoHabitacion();

};

#endif // HABITACIONTIPOSDIALOG_H
