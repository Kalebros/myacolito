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

#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QList>
#include <QSqlTableModel>
#include <QSqlQueryModel>

#include "acolitodatabase.h"
#include "myeventodb.h"

#include "myasistentemodels.h"
#include "mycategoriamodels.h"
#include "myhabitacionmodels.h"
#include "myactividadmodels.h"
#include "mymensajesmodels.h"

#include <QDebug>

/*!
  Conexion de bases de datos MySQL para un evento
  */

class MyDatabase : public AcolitoDatabase
{
    Q_OBJECT
public:

    /*!
      Devuelve la instancia unica de MyDatabase
      @param parent Padre en caso de que no exista la instancia y
      sea necesario crearla
      @note Modifica el objeto
      */
    static MyDatabase *getInstance(QObject *parent=0);

    void setAcolitoDatabase(AcolitoDB *db);
    AcolitoDB *getAcolitoDatabase();

    ///////////////////////////////////////////////////////////
    // Modelos

    QSortFilterProxyModel *getModeloCategorias();
    QSortFilterProxyModel *getModeloAsistentes();
    QSortFilterProxyModel *getModeloHabitaciones();
    QSortFilterProxyModel *getModeloAlojamientos();
    QSortFilterProxyModel *getModeloTiposHabitaciones();
    QSortFilterProxyModel *getModeloFechas();
    QSortFilterProxyModel *getModeloActividades();
    QSortFilterProxyModel *getModeloLocalizaciones();
    QSortFilterProxyModel *getModeloTiposActividad();
    QSortFilterProxyModel *getModeloMensajes();


    QIcon getAsistentesOkIcon() const;
    QIcon getAsistentesFalseIcon() const;
    QString getOrganizadorActividad(int idActividad);
    QVariant grupoAlojadoEn(QString codHabitacion);
    QVariant habitacionDelGrupo(QString idGrupo);
    bool haPagado(QString dni);

    bool existeTablaMensajes() const;
    bool crearTablaMensajes();
    bool nuevoMensaje(QString mensaje);
    bool borrarMensaje(int idMensaje);
    QList<int> getIdMensaje(QString texto) const;
    QString getMensajeById(int id) const;


signals:

public slots:

    void conectar();
    void crearDatabase(Evento *ev=0,QStandardItemModel *categorias=0);
    void crearCategoriaAsistentes(QVariantMap datosCategoria);
    void borrarCategoriaAsistentes(QString categoria, QString sustituta);
    void addNuevoAsistente(Asistente *asis);
    void addGrupoAsistentes(QList<Asistente*> listaAsis);
    void agruparAsistentes(QList<Asistente *> listaAsis);
    void nuevoGrupoAsistentes(QList<Asistente*> listaAsis);
    void eliminarAsistentes(QList<Asistente*> listaAsis);
    void addNuevaHabitacion(Habitacion *habitacion);
    void addNuevoTipoHabitacion(QVariantMap datosTipo);
    void borrarTipoHabitacion(QString tipo, QString tipoSubst);
    void alojarGrupo(Asistente *asis, Habitacion *hab);
    void realojarAsistente(Asistente *asis,Habitacion *hab);
    void separarAsistente(Asistente *asis);
    void intercambiarHabitaciones(Asistente *asistenteA, Asistente *asistenteB);
    void desalojarHabitacion(Habitacion *hab);
    bool estaAbierta(QString codHab);
    void abrirHabitacion(QString codHab, bool estado);
    void activarTodasLasJornadas(int idActividad, bool modo);
    bool getModoTodasLasJornadas(int idActividad);
    void addTipoActividad(QString tipo);
    void delTipoActividad(QString tipo, QString sustituto);
    void addLocalizacion(QString lugar);
    void delLocalizacion(QString lugar, QString sustituto);
    void addActividad(QVariantMap actividad);
    void addListaActividades(QList<QVariantMap> listaActividades);
    void delActividad(int idActividad);

private:

    /*!
      Constructor privado
      */
    explicit MyDatabase(QObject *parent = 0);

    static MyDatabase* _instancia; ///< Instancia unica de la conexion
    static QString _insertCategorias;   ///< Query de insercion inicial de categorias para nuevos eventos
    static QString _insertFechas;   ///< Query de insercion inicial de fechas para nuevos eventos

    MyAcolitoDB *_db;   ///< Conexion de base de datos

    MyAsistenteModels *_gModelosAsistentes;   ///< Generador de modelos de asistentes
    MyCategoriaModels *_gModelosCategorias;     ///< Generador de modelos de categorias
    MyHabitacionModels *_gModelosHabitaciones;  ///< Generador de modelos de habitaciones
    MyActividadModels *_gModelosActividades;    ///< Generador de modelos de actividades
    MyMensajesModels *_gModelosMensajes;        ///< Generador de modelos de mensajes

    QSqlQueryModel *_modeloFechas;  ///< Modelo de fechas del evento

private slots:

    /*!
      Desconecta todos los modelos del database
      @note Modifica el objeto
      */
    void desconectar();
    
};

#endif // MYDATABASE_H
