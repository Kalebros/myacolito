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

#ifndef WIZARDEVENTO_H
#define WIZARDEVENTO_H

#include <QWizard>
#include <QWizardPage>
#include <QBoxLayout>
#include <QIcon>

#include <Databases/myeventodb.h>

#include "weventopaginainicial.h"
#include "weventopaginaplazas.h"
#include "weventopaginafinal.h"

/*!
  Asistente para la creación de eventos, programa AcoLITE
  */

class WizardEvento : public QWizard
{
    Q_OBJECT
public:
    explicit WizardEvento(QWidget *parent = 0);

signals:

    /*!
      Envia los datos del evento al programa para su procesamiento
      @param evento Evento creado
      @param plazas Listado de categorias de plazas a crear
      */
    void datosEvento(Evento *evento,QStandardItemModel *plazas);

public slots:

    /*!
      Muestra el asistente tras reiniciarlo
      */

    void lanzarAsistente();

    /*!
      Establece el icono del wizard
      */

    void setIcono(QIcon icon);

private:

    void setup();

    /*!
      Creamos la primera pagina del asistente
      */
    void crearPaginaInicio();

    /*!
      Creamos la pagina de plazas
      */

    void crearPaginaPlazas();

    /*!
      Creamos la ultima pagina del asistente
      */
    void crearPaginaFinal();

private slots:

    /*!
      Envia la señal de datosEvento cuando pulsamos en Finish
      */
    void enviarNuevoEvento();

private:

    QIcon iconoApp; ///< Icono de la aplicacion

};

#endif // WIZARDEVENTO_H
