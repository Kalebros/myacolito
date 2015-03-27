#ifndef MYMENSAJESMODELS_H
#define MYMENSAJESMODELS_H

#include "myacolitomodels.h"
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlRecord>

/*!
  Creador de modelos de mensajes
 */

class MyMensajesModels : public MyAcolitoModels
{
    Q_OBJECT
public:
    explicit MyMensajesModels(QObject *parent = 0,MyAcolitoDB *db=0);

    bool isActive() const;

    /*!
      Devuelve modelo de mensajes
      */

    QSortFilterProxyModel *getModeloMensajes();

    /*!
      Devuelve si existe la tabla de mensajes
      */

    bool existeTablaMensajes() const;

    /*!
      Crea la tabla de mensajes
      */

    bool creaTablaMensajes();

    /*!
      Añade un nuevo mensaje
      */

    bool addMensaje(QString mensaje);

    /*!
      Borra un mensaje
      */

    bool removeMensaje(int id);
    
signals:
    
public slots:

    void deleteModelos();
    void loadModelos();
    
private:

    QSqlTableModel *_modeloBase;
};

#endif // MYMENSAJESMODELS_H
