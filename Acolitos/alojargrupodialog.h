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

#ifndef ALOJARGRUPODIALOG_H
#define ALOJARGRUPODIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

#include <Databases/acolitodatabase.h>
#include <Databases/mymodeloasistente.h>
#include <Databases/mymodelohabitaciones.h>

#include "booleanicondelegate.h"
#include "tiposhabdelegate.h"
#include "modeloseleccion.h"

namespace Ui {
class AlojarGrupoDialog;
}

class AlojarGrupoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AlojarGrupoDialog(QWidget *parent = 0,QVariant grupo=QVariant(),AcolitoDatabase *db=0);
    ~AlojarGrupoDialog();
    
private:
    Ui::AlojarGrupoDialog *ui;
    AcolitoDatabase *database;
    QVariant IDgrupo;

    QSortFilterProxyModel *modeloHabitaciones;
    ModeloSeleccion *modelo;
    QSortFilterProxyModel *filtroLibres;

private slots:

    void vigilaBoton(QAbstractButton *boton);

    void filtrarLibres();
};

#endif // ALOJARGRUPODIALOG_H
