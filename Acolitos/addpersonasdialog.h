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

#ifndef ADDPERSONASDIALOG_H
#define ADDPERSONASDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QStandardItem>
#include <QString>
#include <QModelIndex>
#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>

#include <Databases/acolitodatabase.h>
#include <Databases/mymodeloasistente.h>

#include <Componentes/boolcheckbox.h>
#include <Componentes/textcombobox.h>

namespace Ui {
    class addPersonasDialog;
}

class addPersonasDialog : public QDialog
{
    Q_OBJECT
    Q_ENUMS (DatosPersonas)
public:

    enum DatosPersonas
    { Nombre,DNI,Telefono,Correo,Categoria,PlazaPagada,Comentario };

    explicit addPersonasDialog(QWidget *parent = 0,AcolitoDatabase *database=0);
    ~addPersonasDialog();

public slots:

    void mostrarDialogo();

signals:

    /*!
      Esta señal se emite para solicitar la creación de un nuevo
      grupo de asistentes
      */
    void nuevoGrupoPersonas(QList<Asistente*> grupo);

    /*!
      Señal de error
      */

    void error(QString mensaje);

    /*!
      Señal de error critico
      */

    void critical(QString mensaje);

private:
    Ui::addPersonasDialog *ui;
    AcolitoDatabase *db;                ///< Database empleada
    QStandardItemModel *modeloPersonas; ///< Modelo de personas
    QDataWidgetMapper *mapeador;        ///< Mapeador de datos

    /*!
      Prepara el dialogo durante la creacion del mismo
      */

    void setup();

    /*!
      Recarga los datos básicos del dialogo
      */

    void reloadDialog();

    void limpiaCampos();

private slots:

    /*!
      Añade una nueva persona al grupo
      */

    void addPersona();

    /*!
      Borra una persona del grupo
      */

    void borraPersona();

    /*!
      Envia los datos
      */

    void sendGrupo();

    /*!
      Comprueba el boton pulsado en el buttonBox
      */

    void compruebaBoton(QAbstractButton* boton);
};

#endif // ADDPERSONASDIALOG_H
