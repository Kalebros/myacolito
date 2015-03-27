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

#ifndef TIPOACTIVIDADDIALOG_H
#define TIPOACTIVIDADDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QDataWidgetMapper>
#include <QInputDialog>

#include <Databases/mydatabase.h>

namespace Ui {
class TipoActividadDialog;
}

class TipoActividadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TipoActividadDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~TipoActividadDialog();
    
private:
    Ui::TipoActividadDialog *ui;

    AcolitoDatabase *database;  ///< Conexion con la base de datos

    QSortFilterProxyModel *modeloTipos; ///< Modelo de tipos de actividades
    QDataWidgetMapper *mapeador;        ///< Mapeador basico de datos

private slots:

    /*!
      Añade un nuevo tipo de actividad
      */

    void addTipoActividad();

    /*!
      Borra un tipo de actividad
      */

    void delTipoActividad();

};

#endif // TIPOACTIVIDADDIALOG_H
