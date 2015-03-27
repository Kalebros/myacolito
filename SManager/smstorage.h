#ifndef SMSTORAGE_H
#define SMSTORAGE_H

#include "SManager_global.h"
#include <QObject>

class SMScript;

/*!
  Interfaz de almacenes

  Existen dos tipos básicos de almacenes:
  - Los que tienen permanencia (creados sobre un archivo o base de datos)
  - Almacenes temporales (p.e, para crear nuevos scripts)

  Esta interfaz es común a ambos
 */

class SMANAGERSHARED_EXPORT SMStorage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool open READ isOpen)
public:
    explicit SMStorage(QObject *parent = 0);
    
    /*!
      Indica si el almacen esta abierto y listo para
      trabajar con el
      @return bool, true: esta abierto, false: esta cerrado
      @note No modifica el objeto
     */
    virtual bool isOpen() const=0;

    /*!
      Abre el almacen, cargando los scripts para que sean
      accesibles
      @note Modifica el objeto
      @return bool, true: se ha podido abrir el almacen, false: se ha producido un error
      */

    virtual bool openAlmacen()=0;

    /*!
      Cierra el almacen; al cerrar el almacen se borran de
      memoraia los datos de scripts
      @note modifica el objeto
      */

    virtual void closeAlmacen()=0;

    /*!
      Devuelve el numero de scripts del almacen
      @return int, numero de scripts en el almacen
      @note No modifica el objeto
      @pre El almacen esta abierto
      */

    virtual int size() const=0;

    /*!
      Devuelve el ultimo error producido en el almacen
      @return QString, texto indicativo del error
      @note No modifica el objeto
      */

    virtual QString lastError() const=0;

    /*!
      Devuelve un script especifico del almacen
      @param num    Numero de script dentro del almacen
      @return Script, script almacenado en el almacen, 0 si no se encontró ninguno
      @note No modifica el objeto
      @pre El almacen esta abierto
      @pre El numero de script identifica a un script dentro del almacen
      */

    virtual SMScript *getScript(int num)=0;

    /*!
      Almacena un nuevo script dentro del almacen
      @param script Nuevo script a almacenar
      @return int, numero del script dentro del almacen
      @note Modifica el objeto
      @pre El almacen esta abierto
      @post El script es accesible utilizando el numero devuelto
      */

    virtual int addScript(SMScript *script)=0;

    /*!
      @overload
      */

    virtual int addScript(QString nombre,QString path,QString argumentos)=0;

    /*!
      Borra un script del almacen
      @param id Numero identificador del script dentro del almacen
      @note Modifica el objeto
      @return bool, true: se ha podido eliminar el script, false: se produjo un error
      @pre El almacen esta abierto
      @pre El identificador es valido
      */

    virtual bool removeScript(int id)=0;

    /*!
      Copia todos los scripts de un SMStorage
      @param almacen    SMStorage del cual copiar los scripts
      @note Modifica el objeto
      @pre Los dos almacenes estan abiertos
      @post Pueden existir scripts repetidos
      */

    virtual void addAlmacen(SMStorage *almacen)=0;

    /*!
      Genera un almacen de un tipo determinado
      */

    static SMStorage *getStorage(QString tipo,QVariant datosConfiguracion);

signals:
    
public slots:
    
};

#endif // SMSTORAGE_H
