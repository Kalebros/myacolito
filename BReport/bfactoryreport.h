/***********************************************************************
** BReport, copyright @2012,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of BReport.
**
** BReport is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** BReport is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with BReport.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#ifndef BFACTORYREPORT_H
#define BFACTORYREPORT_H

#include "breport.h"

#include <QObject>
#include <QMap>
#include <QMetaObject>

#include <QDebug>

/*!
  Factoria de reportes e informes

  @brief
  Cada factoria de reportes esta identificada por una cadena.
  Podemos emplear distintas factorias identificadas por una
  misma cadena para diferentes situaciones (por ejemplo,
  reportes sobre diferentes bases de datos).

  La factoria se encarga de procesar la segunda parte de los mensajes
  y seleccionar el reporte adecuado, además de proporcionar el documento
  de base.

  Dentro de la factoria, existe un mapa con los distintos meta-objetos
  de los reportes conocidos por la factoria. Cuando se recibe un mensaje,
  debidamente procesado por el ReportManager, la factoria comprueba
  el nombre de las clases conocidas, si coincide alguna devuelve un nuevo
  reporte y podemos empezar a generar.
  */

class BREPORTSHARED_EXPORT BFactoryReport : public QObject
{
    Q_OBJECT
public:
    explicit BFactoryReport(QObject *parent = 0);

    /*!
      Devuelve un BReport conocido por la factoria
      */
    BReport *getReport(QString name);

signals:

public slots:

private:

    QMap<QString,const QMetaObject*> listaReportes;   ///< Lista de reportes

protected:

    /*!
      Añade un nuevo reporte a la factoria
      */
    void addReport(const QMetaObject &metaReport);

    /*!
      Elimina un reporte de la factoria
      */
    void removeReport(QMetaObject *metaReport);
};

#endif // BFACTORYREPORT_H
