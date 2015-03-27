/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ram�rez Marti
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

#ifndef ASISTENTECATEGORIASDIALOG_H
#define ASISTENTECATEGORIASDIALOG_H

#include <QDialog>

#include <QInputDialog>

#include <Databases/acolitodatabase.h>
#include <Databases/mydatabase.h>
#include <QVariantMap>
#include <QDataWidgetMapper>

#include "nuevacategoriadialog.h"

namespace Ui {
class AsistenteCategoriasDialog;
}

class AsistenteCategoriasDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AsistenteCategoriasDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~AsistenteCategoriasDialog();
    
private:
    Ui::AsistenteCategoriasDialog *ui;
    QSortFilterProxyModel *modeloCategorias;
    QDataWidgetMapper *mapeador;

    AcolitoDatabase *database;  ///< Conexion de base de datos

private slots:

    void nuevaCategoria();

    void borrarCategoria();

    void addCategoria();
};

#endif // ASISTENTECATEGORIASDIALOG_H
