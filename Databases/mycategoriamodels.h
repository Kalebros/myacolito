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

#ifndef MYCATEGORIAMODELS_H
#define MYCATEGORIAMODELS_H

#include "myacolitomodels.h"

#include "mymodelocategorias.h"

class MyCategoriaModels : public MyAcolitoModels
{
    Q_OBJECT
public:
    explicit MyCategoriaModels(QObject *parent = 0, MyAcolitoDB *db=0);
    virtual bool isActive() const;

    /*!
      Devuelve el modelo base de categorias
      */

    QSortFilterProxyModel *getModeloCategorias();
    
signals:

    void cambiosCategoria();
    
public slots:

    virtual void deleteModelos();
    virtual void loadModelos();

    void crearNuevaCategoria(QVariantMap datosCategoria);
    void borrarCategoria(QString cat,QString sustituta);
    void updateCategoria(const QString cat);

private:

    MyModeloCategorias *_modeloBase;    ///< Modelo base de categorias
    
};

#endif // MYCATEGORIAMODELS_H
