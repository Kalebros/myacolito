/***********************************************************************
** SManager, copyright @2013,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of SManager.
**
** SManager is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SManager is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with SManager.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "smstorage.h"

#include "smstolite.h"

SMStorage::SMStorage(QObject *parent) :
    QObject(parent)
{
}

SMStorage *SMStorage::getStorage(QString tipo, QVariant datosConfiguracion)
{
    SMStorage *res=0;
    if(tipo=="QSQLITE") {
        res=new SMStoLite(0,datosConfiguracion.toString());
    }

    return res;
}
