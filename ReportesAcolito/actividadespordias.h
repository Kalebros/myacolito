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

#ifndef ACTIVIDADESPORDIAS_H
#define ACTIVIDADESPORDIAS_H

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


class REPORTESACOLITOSHARED_EXPORT ActividadesPorDias : public BReport
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ActividadesPorDias(QObject *parent = 0);
    
    QTextDocument *generateReport();
    QString getNombreReport() const;

signals:
    
public slots:

private:

    QFont _fTexto;
    QFont _fCabecera;

    /*!
      Crea una nueva tabla para insertar datos de actividades
      */

    QTextTable *crearTablaActividades(int numActividades);
};

#endif // ACTIVIDADESPORDIAS_H
