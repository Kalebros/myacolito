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

#ifndef MYMODELOASISTENTE_H
#define MYMODELOASISTENTE_H

#include <QAbstractTableModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QString>
#include <QSqlDatabase>
#include <QIcon>
#include <QList>

#include <QDebug>
#include <QSqlError>

/*!
  Clase base de asistentes
  */

class Asistente : public QObject
{
    Q_OBJECT
public:

    /*!
      Constructor por defecto
      */
    explicit Asistente(QObject *parent=0) :
        QObject(parent),
        idAsis(-1),
        idGrupo(-1),
        pago(false),
        activo(false),
        expirado(false)
    {}

public:

    int idAsis;    ///< Identificador del asistente, -1 si no esta definido
    int idGrupo;       ///< Identificador del grupo, -1 si no esta definido
    QString nombre;    ///< Nombre del asistente
    QString DNI;       ///< DNI del asistente
    QString telefono;  ///< Telefono del asistente
    QString comentarios;    ///< Comentarios del asistente
    QString mail;          ///< Correo del asistente
    QString tipo;           ///< Categoria de la plaza del asistente
    bool pago;              ///< Pago del asistente, por defecto falso
    bool activo;            ///< Plaza activa o no, por defecto falso
    bool expirado;          ///< Plaza expirada, por defecto falso
};

/*!
  Modelo basico de asistentes para MyAcolite
  */

class MyModeloAsistente : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QIcon iconoOk READ getIconoOk)
    Q_PROPERTY(QIcon iconoFalse READ getIconoFalse)
public:
    explicit MyModeloAsistente(QObject *parent = 0,const QSqlDatabase db=QSqlDatabase());

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /*!
      Devuelve el icono OK de MyModeloAsistente
      */

    QIcon getIconoOk() const;

    /*!
      Devuelve el icono FALSE de MyModeloAsistente
      */

    QIcon getIconoFalse() const;

    /*!
      Devuelve el pago de un asistente
      */

    bool pagoAsistente(QString dni) const;

signals:

    void nuevaPlaza(const QString categoria);
    void borradaPlaza(const QString categoria);
    void cambiosGrupo();

public slots:

    /*!
      Recarga el modelo
      @note Modifica el objeto
      @note Esta version produce un reseteo del modelo,
      lo cual no es necesariamente malo, pero implica perder
      el foco en las vistas que lo emplean
      */
    void reload();

    /*!
      Añade un asistente a la base de datos
      @param asis   Asistente
      @note Modifica el objeto
      */
    void addAsistente(Asistente *asis);

    /*!
      Elimina un asistente de la base de datos
      @param asis   Asistente
      @note Modifica el objeto
      */

    void deleteAsistente(Asistente *asis);

    /*!
      Agrupa una serie de asistentes juntos
      @param lista  Lista de asistentes que conforman el grupo
      @note Modifica el objeto
      @note Se diferencia del metodo @e nuevoGrupoAsistentes en que
      se asignan a los asistentes al grupo del primer asistente de la lista
      */

    void agruparAsistentes(QList<Asistente*> lista);

    /*!
      Separa a un asistente en un nuevo grupo
      @param identificador  Identificador del asistente
      @note Modifica el objeto
      */

    void separarAsistente(int identificador);

    /*!
      Cambia a un asistente de grupo
      @param asistente  Asistente que queremos cambiar de grupo
      @param idGrupo    Identificador del grupo
      @note El asistente debe incluir al menos el identificador
      @pre El asistente existe en la base de datos
      @pre El grupo debe existir dentro de la base de datos
      */

    void cambiarAsistenteDeGrupo(Asistente *asistente,int idGrupo);

    /*!
      Crea un nuevo grupo con una serie de asistentes
      @param lista  Lista de asistentes que conforman el grupo
      @note Modifica el objeto
      @note Se diferencia del metodo @e agruparAsistentes en que
      se crea un nuevo grupo para todos los asistentes.
      @post Si un asistente estaba solo en su grupo y dentro de una habitacion,
      se eliminara su grupo y su habitacion quedara libre. En caso de que no se
      elimine el grupo, seguirá ocupando la habitacion
      */

    void nuevoGrupoAsistentes(QList<Asistente*> lista);

    /*!
      Recargar los datos de categorias
      */

    void updateCategorias();

private:

    QIcon iconoOk;
    QIcon iconoError;

    QSqlTableModel *_tablaAsistentes;   ///< Tabla de datos de los asistentes
    QSqlTableModel *_tablaReservas;  ///< Tabla de datos de plazas de asistentes
    QSqlTableModel *_tablaPlazas;   ///< Tabla de datos de la plaza reservada

    static QString _selectDNI;  ///< Sentencia de seleccion de asistente por dni para filtro de tablas

    /*!
      Coloca el modelo de reservas en el indice del modelo de asistentes indicado
      */
    void setReservaOn(const QModelIndex &idx) const;

    /*!
      Coloca el modelo de plazas en el indice del modelo de asistentes indicado
      */
    void setPlazaOn(const QModelIndex &idx) const;

    /*!
      Coloca el modelo de reservas en el DNI indicado del modelo de asistentes
      */
    void setReservaOnDNI(const QString dni) const;

    /*!
      Devuelve el QModelIndex correspondiente a un asistente
      */
    QModelIndex getModelIndexOf(QString dni);
};

#endif // MYMODELOASISTENTE_H
