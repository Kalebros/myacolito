#ifndef SMSCRIPTLITE_H
#define SMSCRIPTLITE_H

#include "smscript.h"

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QVariant>

class SMStoLite;

/*!
  Script basado en almacenes SQLite
 */

class SMANAGERSHARED_EXPORT SMScriptLite : public SMScript
{
    Q_OBJECT
public:
    explicit SMScriptLite(QObject *parent = 0,int idInterno=-1,SMStoLite *db=0,QSqlTableModel *modelo=0);

    int getID() const;
    QString getNombre() const;
    QString getPath() const;
    QString getArgumentos() const;

    
signals:
    
public slots:

    void setNombre(QString nombre);
    void setPath(QString path);
    void setArgumentos(QString argumentos);

private:

    int _identificador; ///< Identificador dentro de la base de datos
    SMStoLite *_almacen;    ///< Almacen al que pertenece el script

    QSqlTableModel *_modeloDatos;    ///< Modelo que contiene los datos
    int pNombre,pPath,pArgumentos;  ///< Posiciones de los campos dentro del modelo

    /*!
      Busca la posicion del script dentro del modelo
      @return int, posicion del script dentro del modelo
      @note No modifica el objeto
     */

    int searchPosition() const;
    
    /*!
      Calcula las posiciones de los campos dentro del modelo
      @return void
      @pre El modelo suministrado es distinto de cero y el almacen esta abierto
      @note Modifica el objeto
      */

    void getFieldsIndex();
};

#endif // SMSCRIPTLITE_H
