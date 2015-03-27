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

#ifndef WEVENTOPAGINAPLAZAS_H
#define WEVENTOPAGINAPLAZAS_H

#include <QWizardPage>

#include <QDataWidgetMapper>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QStandardItem>
#include <QMessageBox>
#include <QModelIndex>
#include <QDate>

#include <QDebug>

namespace Ui {
    class WEventoPaginaPlazas;
}

class WEventoPaginaPlazas : public QWizardPage
{
    Q_OBJECT

public:
    explicit WEventoPaginaPlazas(QWidget *parent = 0);
    ~WEventoPaginaPlazas();

    void initializePage();

    void cleanupPage();

    QStandardItemModel *getModeloPlazas() const
    {
        return modeloPlazas;
    }

private:
    Ui::WEventoPaginaPlazas *ui;

    QStandardItemModel *modeloPlazas;   ///< Modelo de tipos de plazas
    QStandardItemModel *modeloFechas;   ///< Modelo de fechas para los comboBox
    QDataWidgetMapper *mapeador;        ///< Mapeador del modelo

    /*!
      Prepara la página
      */
    void setupPage();

    /*!
      Corrige las fechas asignadas en el modelo de plazas.
      Si alguna fecha no existe o se corresponde con las fechas del modelo,
      colocarla como 'Sin asignar'.
      */

    void corrigeFechas(QList<QVariant> fechas);

    bool validatePage();

private slots:

    /*!
      Crea una nueva categoria
      */

    void nuevaCategoria();

    /*!
      Borra las categorias seleccionadas
      */

    void borrarCategorias();
};

#endif // WEVENTOPAGINAPLAZAS_H
