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

#ifndef MYACOLITO_H
#define MYACOLITO_H

#include <QWidget>

#include <Databases/mydatabase.h>

/*!
  Clase base de Acolitos
  */

class MyAcolito : public QWidget
{
    Q_OBJECT
public:
    explicit MyAcolito(QWidget *parent = 0,AcolitoDatabase *db=0);
    
signals:
    
    /*!
      Envia un mensaje de error
      */
    void error(QString mensaje);

public slots:

    /*!
      Prepara el acolito
      */
    void loadAcolito(AcolitoDatabase *db=0);

    /*!
      Recarga los modelos
      */
    virtual void reloadModelos()=0;

    /*!
      Limpia el acolito
      */

    virtual void clear()=0;

protected:

    AcolitoDatabase *database;
    
};

#endif // MYACOLITO_H
