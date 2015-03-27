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

#ifndef MYACOLITOMODELS_H
#define MYACOLITOMODELS_H

#include <QObject>

#include "myacolitodb.h"

/*!
  Clase base para todos los generadores de modelos de MyAcolito
  @brief
    Los generadores de modelos son las clases intermediarias de MyDatabase.
    A partir de ellos, obtengo todos los modelos necesarios para poder trabajar
    con los Acolitos.
  */

class MyAcolitoModels : public QObject
{
    Q_OBJECT
public:
    explicit MyAcolitoModels(QObject *parent = 0,MyAcolitoDB *db=0);
    
    /*!
      Devuelve si existen modelos activos dentro del objeto
      */

    virtual bool isActive() const=0;

    /*!
      Devuelve el objeto MyAcolitDB utilizado como conexion
      */

    MyAcolitoDB *database();

signals:

    void changeModelos();
    
public slots:

    /*!
      Establece la base de datos de acolitos. No se permite
      establecer una nueva base de datos mientras existan modelos
      activos
      */
    void setAcolitoDB(MyAcolitoDB *db);

    /*!
      Borra los modelos activo del objeto
      */

    virtual void deleteModelos()=0;

    /*!
      Recarga los modelos del objeto
      */

    virtual void loadModelos()=0;

protected:

    MyAcolitoDB *_db;   ///< Conexion con la base de datos de MyAcolito
};

#endif // MYACOLITOMODELS_H
