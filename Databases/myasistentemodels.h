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

#ifndef MYASISTENTEMODELS_H
#define MYASISTENTEMODELS_H

#include "myacolitomodels.h"
#include "mymodeloasistente.h"

#include <QDebug>

#include <QSortFilterProxyModel>
#include <QSqlTableModel>

/*!
  Generador de modelos de asistentes
  */

class MyAsistenteModels : public MyAcolitoModels
{
    Q_OBJECT
public:
    explicit MyAsistenteModels(QObject *parent = 0,MyAcolitoDB *db=0);

    virtual bool isActive() const;

    /*!
      Devuelve el modelo base de asistentes
      */

    QSortFilterProxyModel *getModeloAsistentes();

    /*!
      Devuelve el icono de ok del modelo
      */

    QIcon getIconoOk() const;

    /*!
      Devuelve el icono de false del modelo
      */

    QIcon getIconoFalse() const;

    /*!
      Devuelve el pago de un asistente
      */

    bool getPagoAsistente(QString dni) const;


signals:

    void nuevaPlaza(const QString cat);
    void borradaPlaza(const QString cat);
    void cambiosGrupo();

public slots:

    virtual void deleteModelos();
    virtual void loadModelos();

    void addAsistente(Asistente *asis);
    void eliminarAsistente(Asistente *asis);
    void agruparAsistentes(QList<Asistente*> lista);
    void separarAsistente(Asistente *asis);
    void nuevoGrupoAsistentes(QList<Asistente*> lista);
    void cambiarAsistenteDeGrupo(Asistente *asis,int idGrupo);

    void recargarDatosCategorias();

private:

    MyModeloAsistente *_modeloAsistentes;  ///< Modelo base de asistentes

private slots:

    void depurar(QString mensaje);
};

#endif // MYASISTENTEMODELS_H
