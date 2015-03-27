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

#include "bfactoryreport.h"

BFactoryReport::BFactoryReport(QObject *parent) :
    QObject(parent)
{
}

void BFactoryReport::addReport(const QMetaObject &metaReport)
{
    listaReportes.insert(metaReport.className(),&metaReport);
    //qDebug() << "CLASE EN FACTORIA: " << metaReport.className();
}

void BFactoryReport::removeReport(QMetaObject *metaReport)
{
    listaReportes.remove(metaReport->className());
}

BReport *BFactoryReport::getReport(QString name)
{
    BReport *res=0;

    if(listaReportes.contains(name)) {
        const QMetaObject *metaReport=listaReportes.value(name);

        res=qobject_cast<BReport*>(metaReport->newInstance(Q_ARG(QObject*,this)));
    }
    else
    {
        qDebug() << "ERROR EN FACTORY REPORT: No existe el reporte indicado";
    }

    return res;
}
