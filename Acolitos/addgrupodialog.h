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

#ifndef ADDGRUPODIALOG_H
#define ADDGRUPODIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>

#include <Databases/acolitodatabase.h>
#include <Databases/mymodeloasistente.h>

#include "booleanicondelegate.h"
#include "categoriasdelegate.h"
#include "modeloseleccion.h"

namespace Ui {
class AddGrupoDialog;
}

class AddGrupoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddGrupoDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~AddGrupoDialog();
    
private:
    Ui::AddGrupoDialog *ui;
    AcolitoDatabase *database;

    ModeloSeleccion *modeloBase;
    QSortFilterProxyModel *filtroBusqueda;
    QSortFilterProxyModel *filtroGrupo;

    void setup();
    void setupVista();
    void setupFiltros();

private slots:

    void cambiarModoBusqueda(int modo);
    void cambiarModoFiltrado(int modo);
    void vigilarBotones(QAbstractButton *boton);
};

#endif // ADDGRUPODIALOG_H
