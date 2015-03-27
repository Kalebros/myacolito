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

#ifndef ASISTENTEREPORT_H
#define ASISTENTEREPORT_H

#include "ReportesAcolito_global.h"

#include <BReport/breport.h>

#include <QBrush>
#include <QPen>
#include <QColor>
#include <QTextFrame>
#include <QTextFrameFormat>
#include <QFontMetrics>
#include <QDebug>
#include <QDate>

class REPORTESACOLITOSHARED_EXPORT AsistenteReport : public BReport
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit AsistenteReport(QObject *parent = 0);

    QTextDocument *generateReport();
    QString getNombreReport() const;
    
signals:
    
public slots:
    
private:

    QFont _fTexto;      //Fuente de texto
    QFont _fCabecera;   //Fuente de cabecera

    /*!
      Crea una tabla para introducir los datos del asistente
      */

    QTextTable *crearTablaAsistente();

    /*!
      Crea una tabla para introducir los datos de la habitacion
      y compañeros de cuarto
      */

    QTextTable *crearTablaHabitacion(int numCompis);
};

#endif // ASISTENTEREPORT_H
