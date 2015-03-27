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

#ifndef ACTIVIDADREPORT_H
#define ACTIVIDADREPORT_H

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


/*!
 * \brief Clase ActividadReport
 *  Reporte para emplear con las actividades del programa Acolito.
 *
 * Necesitamos los siguientes datos de las actividades para poder generar el
 * reporte:
 *  -# <b>Nombre de la actividad:</b> Emplea el identificador @e nombre
 *  -# <b>Tipo de la actividad:</b> Emplea el identificador @e categoria
 *  -# <b>ID de la actividad:</b> Emplea el identificador @e id
 *  -# <b>Dia de la actividad:</b> Emplea el identificador @e fecha
 *  -# <b>Hora de inicio de la actividad:</b> Emplea el identificador @e inicio
 *  -# <b>Hora de fin de la actividad:</b> Emplea el identificador @e fin
 *  -# <b>Maximo de Participantes:</b> Emplea el identificador @e maxParticipantes
 *  -# <b>Minimo de Participantes:</b> Emplea el identificador @e minParticipantes
 *  -# <b>Descripcion de la actividad:</b> Emplea el identificador @e descripcion
 *  -# <b>Requisitos de la actividad:</b> Emplea el identificador @e requisitos
 *  -# <b>Responsable de la actividad:</b> Emplea el identificador @e responsable
 *  -# <b>Lugar de la actividad: </b> Emplea el identificador @e lugar
 *
 *  Hay un limite en cuanto al numero de caracteres de la descripcion y de los requisitos...
 *  por defecto están puestos en 900 y 800, respectivamente, y con un límite de caracteres
 *  de 100 para el titulo de la actividad y de 13 para la categoría; el tipo de fuente es
 *  Verdana 12 para el texto normal, 18 para las cabeceras y 20 para el identificador de la actividad
 */


class REPORTESACOLITOSHARED_EXPORT ActividadReport : public BReport
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ActividadReport(QObject *parent = 0);

    QTextDocument *generateReport();
    QString getNombreReport() const;
    
signals:
    
public slots:

private:

    int _limiteCarDescripcion;
    int _limiteCarRequisitos;
    int _limiteCarTitulo;
    int _limiteCarCategoria;
    QFont _fTexto;
    QFont _fCabecera;
    QFont _fId;

    /*!
     * Crea la tabla base para imprimir la actividad
     */

    QTextTable *crearTablaBase();

    /*!
     * Crea la tabla de participantes
     */

    QTextTable *crearTablaParticipantes();
    
};

#endif // ACTIVIDADREPORT_H
