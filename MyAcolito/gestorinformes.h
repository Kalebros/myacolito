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

#ifndef GESTORINFORMES_H
#define GESTORINFORMES_H

#include <QObject>

#include <BReport/breportmanager.h>
#include <Databases/acolitodatabase.h>

#include <QDate>
#include <QTime>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QVariantMap>

/*!
  Esta clase recupera informacion de la base de datos para
  poder generar informes.

  Los informes se generan tambien como reportes, pero se invocan desde
  fuera de los acolitos para evitar saturarlos de funciones y metodos
  innecesarios. Los objetos de la clase GestorInformes trabajan sobre
  objetos AcolitoDatabase y luego envian señales a un ReportManager
  para generar los informes como reportes, pero siempre mostrandolos
  en dialogos.
 */

class GestorInformes : public QObject
{
    Q_OBJECT
public:
    explicit GestorInformes(QObject *parent = 0);

    /*!
      Establece la database sobre la cual debe de trabajar el gestor
      de informes.
      @param database   Base de datos
      @note Modifica el objeto
      @note No modifica la base de datos
      */

    void setDatabase(AcolitoDatabase *database);
    
signals:

    /*!
      Señal de informes
      */

    void solicitarInforme(QString mensaje,QVariantMap datosInforme,ReportOptions opciones);
    
public slots:

    /*!
      Solicitar informe de reporte de actividades por dias
      */

    void reporteActividadesPorDias();

    /*!
      Solicitar informe de reporte de actividades por tipos
      */

    void reporteActividadesPorTipos();

    /*!
      Solicitar informe de reporte de actividades por localizaciones
      */

    void reporteActividadesPorLocalizacion();

    /*!
      Solicitar informe de reporte de asistentes por categorias
      */

    void reporteAsistentesPorCategorias();

private:

    AcolitoDatabase *_database;  ///< Base de datos
    
};

#endif // GESTORINFORMES_H
