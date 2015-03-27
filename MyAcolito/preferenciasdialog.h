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

#ifndef PREFERENCIASDIALOG_H
#define PREFERENCIASDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QApplication>

#include <Databases/datosconexion.h>

#include "conexionessql.h"
#include "conexiondialog.h"

//Libreria de gestion de scripts
#include <SManager/smanager.h>
#include <SManager/smstorage.h>
#include <SManager/smcontainerwidget.h>

namespace Ui {
class PreferenciasDialog;
}

/*!
  Dialogo de preferencias de MyAcolito
  @brief
  Por ahora incluye las preferencias de:
  - @e Conexiones
  - @e Gestion de scripts
  */

class PreferenciasDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferenciasDialog(QWidget *parent = 0,QString archivoCon=QString(),QString archivoSto=QString());
    ~PreferenciasDialog();
    
private:
    Ui::PreferenciasDialog *ui;
    ConexionesSQL *sto;     ///< Almacen de conexiones
    SMStorage *stoScripts;  ///< Almacen de scripts

    QSortFilterProxyModel *modelo;  ///< Modelo de conexiones
    QDataWidgetMapper *mapeador;    ///< Mapeador para mostrar los datos de las conexiones

    /*!
      Prepara el dialogo
      */
    void setup();

    /*!
      Crea una conexion por defecto en localhost
      */

    void crearConexionPorDefecto();

    /*!
      Establece la conexion predeterminada como la actual en el comboBox
      */

    void buscarConexionPredeterminada();

signals:

    /*!
      Se solicita ejecutar un script
      */

    void ejecutarScript(SMScript *s);

private slots:

    /*!
      Borra la conexion seleccionada
      */

    void delConexion();

    /*!
      Crea una nueva conexion
      */

    void crearConexion();

    /*!
      Cambia la pantalla de preferencias dependiendo
      de la seleccion del usuario
      */

    void cambiarPantalla(int indice);
};

#endif // PREFERENCIASDIALOG_H
