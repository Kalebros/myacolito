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

#ifndef ACOLITODATABASE_H
#define ACOLITODATABASE_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariantMap>

#include "acolitodb.h"

class Evento;
class Asistente;
class Habitacion;

/*!
  Conexion de bases de datos para un evento, clase abstracta
  @brief
  Los eventos se almacenan en una unica base de datos
  (al menos en esta version). Desde esta unica conexion
  puedo solicitar todos los modelos que necesito para trabajar
  */

class AcolitoDatabase : public QObject
{
    Q_OBJECT
public:
    explicit AcolitoDatabase(QObject *parent = 0);

    /*!
      Establece una conexion AcolitoDB para la database
      @param db Conexion de base de datos a utilizar
      @note Modifica el objeto
      */
    virtual void setAcolitoDatabase(AcolitoDB *db)=0;

    /*!
      Devuelve la conexion AcolitoDB de la database
      @return Objeto conexion
      @note No modifica el objeto
      */
    virtual AcolitoDB *getAcolitoDatabase()=0;

    ///////////////////////////////////////////////////////////
    // Modelos

    /*!
      Devuelve el modelo de categorias
      */

    virtual QSortFilterProxyModel *getModeloCategorias()=0;

    /*!
      Devuelve el modelo de Asistentes
      */

    virtual QSortFilterProxyModel *getModeloAsistentes()=0;

    /*!
      Devuelve el modelo de Habitaciones
      */

    virtual QSortFilterProxyModel *getModeloHabitaciones()=0;

    /*!
      Devuelve el modelo de tipos de habitaciones
      */

    virtual QSortFilterProxyModel *getModeloTiposHabitaciones()=0;

    /*!
      Devuelve el modelo de alojamientos
      */

    virtual QSortFilterProxyModel *getModeloAlojamientos()=0;

    /*!
      Devuelve el modelo de fechas
      */

    virtual QSortFilterProxyModel *getModeloFechas()=0;

    /*!
      Devuelve el modelo de actividades
      */

    virtual QSortFilterProxyModel *getModeloActividades()=0;

    /*!
      Devuelve el modelo de localizaciones de actividades
      */

    virtual QSortFilterProxyModel *getModeloLocalizaciones()=0;

    /*!
      Devuelve el modelo de tipos de actividades
      */

    virtual QSortFilterProxyModel *getModeloTiposActividad()=0;

    /*!
      Devuelve el modelo de mensajes
      @note Si no existe la tabla de mensajes, devuelve un
      modelo nulo (=0)
      */

    virtual QSortFilterProxyModel *getModeloMensajes()=0;

    /*!
      Devuelve el icono de Ok de asistentes
      */

    virtual QIcon getAsistentesOkIcon() const=0;

    /*!
      Devuelve el icono de False de asistentes
      */

    virtual QIcon getAsistentesFalseIcon() const=0;

    /*!
      Devuelve el organizador de una actividad
      */

    virtual QString getOrganizadorActividad(int idActividad)=0;

    /*!
      Devuelve el pago de un asistente (si ha pagado o no)
      */

    virtual bool haPagado(QString dni)=0;

    /*!
      Devuelve el grupo alojado en la habitacion
      @param habitacion Codigo de la habitacion que queremos comprobar
      @return Identificador del grupo alojado, o QVariant nulo si la habitacion esta libre
      @pre El codigo de habitacion debe de existir dentro del sistema
      */

    virtual QVariant grupoAlojadoEn(QString codHabitacion)=0;

    /*!
      Devuelve la habitacion en la cual esta alojado un grupo
      @param grupo  Identificador del grupo que queremos comprobar
      @return Codigo de la habitacion en la cual esta alojado, o QVariant nulo si el grupo
      no esta alojado
      @pre El grupo debe de existir dentro del sistema
      */

    virtual QVariant habitacionDelGrupo(QString idGrupo)=0;

    /*!
      Devuelve si existe la tabla de mensajes dentro de la base de datos
      @return bool, @b TRUE si existe, @b FALSE en caso contrario
      @pre La base de datos debe de estar abierta
      */

    virtual bool existeTablaMensajes() const=0;

    /*!
      Crea la tabla de mensajes
      @return bool, @b TRUE se ha creado la tabla de mensajes, @b FALSE en caso contrario
      @pre La base de datos debe de estar abierta
      */

    virtual bool crearTablaMensajes()=0;

    /*!
      Añade un nuevo mensaje a la tabla de mensajes
      @param mensaje    Mensaje a añadir
      @return bool, @b TRUE se ha añadido el mensaje, @b FALSE en caso contrario
      @pre La base de datos debe de estar abierta
      @pre La tabla de mensajes debe de existir
      */

    virtual bool nuevoMensaje(QString mensaje)=0;

    /*!
      Borra un mensaje de la tabla de mensajes
      @param idMensaje  Identificador del mensaje
      @return bool, @b TRUE se ha borrado el mensaje, @b FALSE en caso contrario
      @pre La base de datos debe de estar abierta
      @pre La tabla de mensajes debe de existir
      */

    virtual bool borrarMensaje(int idMensaje)=0;

    /*!
      Devuelve los identificadores asignados al mensaje indicado

      No se obliga a que los mensajes tengan un texto único, asi que puede
      darse el caso de que existan diversos mensajes con el mismo texto y distinto
      identificador
      @param texto  Texto del mensaje
      @return Lista de identificadores, una lista vacia en caso de no encontrar el mensaje
      @pre La base de datos debe de estar abierta
      @pre La tabla de mensajes debe de existir
      */

    virtual QList<int> getIdMensaje(QString texto) const=0;

    /*!
      Devuelve el mensaje asignado al identificador
      @param id Identificador del mensaje
      @return Texto del mensaje
      @pre La base de datos debe de estar abierta
      @pre La tabla de mensajes debe de existir
      */

    virtual QString getMensajeById(int id) const=0;

signals:

    /*!
      Indica que se ha cambiado la conexion y hay
      que recargar los modelos empleados
      */
    void nuevaDatabase();

    /*!
      Indica que se han producido cambios en la asignacion
      de habitaciones a grupos
      */

    void reloadAlojamiento();
    
public slots:

    /*!
      Vuelve a crear los modelos del database
      @note Modifica el objeto
      */
    virtual void conectar()=0;

    /*!
      Crea una nueva base de datos Acolite en un
      servidor
      @param servidor   Conexion con el servidor
      @param evento     Datos del evento
      @param categorias Datos de las categorias
      */
    virtual void crearDatabase(Evento *ev=0,QStandardItemModel *categorias=0)=0;

    /*!
      Crea una nueva categoria de asistentes
      */

    virtual void crearCategoriaAsistentes(QVariantMap datosCategoria)=0;

    /*!
      Borra una categoría de asistentes
      */

    virtual void borrarCategoriaAsistentes(QString categoria,QString sustituta)=0;

    /*!
      Añadir un nuevo asistente a la base de datos
      @param asis   Datos del nuevo asistente
      @note Los datos del nuevo asistente deben de ser coherentes
      */

    virtual void addNuevoAsistente(Asistente *asis)=0;

    /*!
      Añade un nuevo grupo de asistentes a la base de datos
      @param listaAsis  Lista de asistentes que conforman el grupo
      @note Los datos de la lista de asistentes deben de ser coherentes
      @note Los asistentes no deben de existir dentro de la base de datos
      */

    virtual void addGrupoAsistentes(QList<Asistente*> listaAsis)=0;

    /*!
      Agrupa juntos a los asistentes indicados
      @param listaAsis  Lista de asistentes que conforman el grupo
      @note Los datos de la lista de asistentes deben de ser suficientes
      (como minimo incluir el DNI)
      @note Los asistentes deben de existir dentro de la base de datos
      @note Se diferencia de @e nuevoGrupoAsistentes en que este metodo agrupa
      a todos los asistentes en el grupo del priemer asistente de la lista
      */

    virtual void agruparAsistentes(QList<Asistente*> listaAsis)=0;

    /*!
      Crea un nuevo grupo con los asistentes indicados
      @param listaAsis  Lista de asistentes que conforman el grupo
      @note Los datos de la lista de asistentes deben de ser suficientes
      (como minimo incluir el DNI)
      @note Los asistentes deben de existir dentro de la base de datos
      @note Se diferencia de @e agruparAsistentes en que este metodo separa a
      los asistentes de sus grupos y crea un nuevo grupo para ellos.
      @note Si alguno de los asistentes estaba en un grupo unico asignado a una habitacion,
      y al cambiar al asistente desaparece el grupo, la habitacion queda libre.
      */

    virtual void nuevoGrupoAsistentes(QList<Asistente*> listaAsis)=0;

    /*!
      Borra los asistentes indicados
      @param listaAsis  Lista de asistentes que queremos eliminar
      @note Los datos de la lista de asistentes deben de ser suficientes
      (como minimo incluir el DNI y el tipo)
      @note Los asistentes deben de existir dentro de la base de datos
      */

    virtual void eliminarAsistentes(QList<Asistente*> listaAsis)=0;

    /*!
      Añade una nueva habitacion a la base de datos
      @param hab    Datos de la nueva habitacion
      @note Los datos de la habitacion deben de ser suficientes
      @note El codigo de la habitacion debe de ser unico y no existir dentro de la base de datos
      */

    virtual void addNuevaHabitacion(Habitacion *habitacion)=0;

    /*!
      Añade un nuevo tipo de habitacion a la base de datos
      @param datosTipo  Datos del nuevo tipo de habitacion
      @note Los datos del tipo de habitacion deben de ser suficientes
      */

    virtual void addNuevoTipoHabitacion(QVariantMap datosTipo)=0;

    /*!
      Borra un tipo de habitacion de la base de datos
      @param tipo   Nombre del tipo a eliminar
      @param tipoSubst  Nombre del tipo que va a sustituir a ese en el sistema
      @note Los datos del tipo de habitacion han de ser consistentes (ambos tienen que existir
      en la base de datos)
      */

    virtual void borrarTipoHabitacion(QString tipo,QString tipoSubst)=0;

    /*!
      Devuelve si una habitacion esta abierta o no
      @param codHab Codigo de la habitacion
      @return Estado de la habitacion, @e true abierta, @e false cerrada.
      @note El codigo de la habitacion debe de existir dentro de la base de datos
      */

    virtual bool estaAbierta(QString codHab)=0;

    /*!
      Abre o cierra una habitacion
      @param codHab Codigo de la habitacion
      @param estado Estado al que queremos colocar la habitacion
      @note El codigo de la habitacion debe de existir dentro de la base de datos
      */

    virtual void abrirHabitacion(QString codHab,bool estado)=0;

    /*!
      Asigna una habitacion a un grupo de asistentes
      @param asis   Asistente perteneciente al grupo
      @param hab    Habitacion que queremos asignar
      @note El asistente debe incluir al menos el identificador de grupo
      @pre La habitacion existe
      @post Si la habitacion estaba asignada a otro grupo, el grupo pasa a estar
      sin alojar.
      @post Si el grupo que queremos realojar estaba en otra habitacion, la habitacion
      anterior queda libre.
      */

    virtual void alojarGrupo(Asistente *asis,Habitacion *hab)=0;

    /*!
      Realoja a un asistente en una habitacion con otro grupo
      @param asis   Asistente que queremos realojar
      @param hab    Habitacion en la cual queremos alojar al asistente
      @pre El asistente debe incluir al menos el codigo del asistente
      @pre La habitacion debe incluir al menos el codigo de la habitacion
      @pre El identificador del asistente debe de existir en la base de datos
      @pre El codigo de la habitacion deben de existir en la base de datos
      @post El asistente cambia de grupo; si esta solo en su grupo, este se elimina.
      Si el grupo estaba alojado en otra habitacion, esta queda libre.
      */

    virtual void realojarAsistente(Asistente *asis,Habitacion *hab)=0;

    /*!
      Separa al asistente de su grupo en uno nuevo
      @param asis   Asistente que queremos separar
      @note El asistente debe de incluir al menos el codigo del asistente
      @pre El asistente debe existir en la base de datos
      @post El asistente esta en un nuevo grupo en el que se encuentra solo
      */

    virtual void separarAsistente(Asistente *asis)=0;


    /*!
      Intercambia de habitaciones dos grupos
      @param asistenteA Asistente perteneciente al primer grupo
      @param asistenteB Asistente perteneciente al segundo grupo
      @pre Los dos asistentes deben incluir al menos el identificador de sus grupos
      @pre Los dos grupos estan alojados en habitaciones
      @post Los dos grupos intercambian habitaciones en el mismo estado en el que se encontraban
      @post Si uno de los grupos estaba no alojado, el otro pasará a estar en esa situacion
      */

    virtual void intercambiarHabitaciones(Asistente *asistenteA,Asistente *asistenteB)=0;

    /*!
      Desaloja una habitacion
      @param habitacion Habitacion que queremos desalojar
      @note La habitacion debe de incluir al menos el codigo de habitacion
      @pre La habitacion debe de existir en la base de datos
      */

    virtual void desalojarHabitacion(Habitacion *hab)=0;

    /*!
      Activa el modo de todas las jornadas para una actividad
      */

    virtual void activarTodasLasJornadas(int idActividad,bool modo)=0;

    /*!
      Devuelve el modo de todas las jornadas para una actividad
      */

    virtual bool getModoTodasLasJornadas(int idActividad)=0;
    /*!
      Añade un nuevo tipo de actividad
      */

    virtual void addTipoActividad(QString tipo)=0;

    /*!
      Borra un tipo de actividad
      */

    virtual void delTipoActividad(QString tipo,QString sustituto)=0;

    /*!
      Añade una nueva localizacion
      */

    virtual void addLocalizacion(QString lugar)=0;

    /*!
      Borra una localizacion
      */

    virtual void delLocalizacion(QString lugar,QString sustituto)=0;

    /*!
      Añade una actividad
      */

    virtual void addActividad(QVariantMap actividad)=0;

    /*!
      Añade una lista de actividades
      */

    virtual void addListaActividades(QList<QVariantMap> listaActividades)=0;

    /*!
      Elimina una actividad
      */

    virtual void delActividad(int idActividad)=0;

};

#endif // ACOLITODATABASE_H
