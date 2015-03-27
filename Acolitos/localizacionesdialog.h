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

#ifndef LOCALIZACIONESDIALOG_H
#define LOCALIZACIONESDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QDataWidgetMapper>
#include <QInputDialog>

#include <Databases/mydatabase.h>

namespace Ui {
class LocalizacionesDialog;
}

class LocalizacionesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LocalizacionesDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~LocalizacionesDialog();
    
private:
    Ui::LocalizacionesDialog *ui;

    AcolitoDatabase *database;      ///< Base de datos
    QSortFilterProxyModel *modeloLocalizaciones;    ///< Modelo de localizaciones
    QDataWidgetMapper *mapeador;        ///< Mapeador

private slots:

    /*!
      Añade una nueva localizacion
      */

    void nuevaLocalizacion();

    /*!
      Borra una localizacion
      */

    void borraLocalizacion();
};

#endif // LOCALIZACIONESDIALOG_H
