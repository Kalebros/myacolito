#ifndef SMSCRIPT_H
#define SMSCRIPT_H

#include "SManager_global.h"
#include <QObject>

/*!
  Interfaz de scripts que puede manejar el SManager
 */

class SMANAGERSHARED_EXPORT SMScript : public QObject
{
    Q_OBJECT
public:
    explicit SMScript(QObject *parent = 0);

    /*!
      Devuelve el identificador del script; dependiendo del
      almacen al que pertenece, dicho identificador puede tener distintos
      sentidos.
      @return int, identificador del script
      @note No modifica el objeto
      */

    virtual int getID() const=0;

    /*!
      Devuelve el nombre del script
      @return QString, nombre del script
      @note No modifica el objeto
      */

    virtual QString getNombre() const=0;
    
    /*!
      Devuelve el path del script
      @return QString, path del script
      @note No modifica el objeto
      */

    virtual QString getPath() const=0;

    /*!
      Devuelve los argumentos del script
      @return QString, argumentos del script
      @note No modifica el objeto
      */

    virtual QString getArgumentos() const=0;

signals:

public slots:

    /*!
      Cambia el nombre del script
      @param nombre Nuevo nombre del script
      @note Modifica el objeto
      */

    virtual void setNombre(QString nombre)=0;

    /*!
      Cambia el path del script
      @param path  Nuevo path del script
      @note Modifica el objeto
      */

    virtual void setPath(QString path)=0;

    /*!
      Cambia los argumentos del script
      @param argumentos Nuevos argumentos del script
      @note Modifica el objeto
      */

    virtual void setArgumentos(QString argumentos)=0;

        
};

#endif // SMSCRIPT_H
