#ifndef SMSTOLITE_H
#define SMSTOLITE_H

#include "smstorage.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QStringList>

/*!
  Almacen basado en archivos SQLite
 */

class SMANAGERSHARED_EXPORT SMStoLite : public SMStorage
{
    Q_OBJECT
public:

    /*!
      Constructor del almacen
       @param parent    Padre del objeto en la jerarquia Qt
       @param pathArchivo   Path del archivo SQLite
     */

    explicit SMStoLite(QObject *parent = 0,QString pathArchivo=QString());

    ~SMStoLite();
    bool isOpen() const;
    bool openAlmacen();

    QString lastError() const {
        return _lastError;
    }

    void closeAlmacen();
    int size() const;
    SMScript *getScript(int num);
    int addScript(SMScript *script);
    int addScript(QString nombre,QString path,QString argumentos);
    bool removeScript(int id);
    void addAlmacen(SMStorage *almacen);

    
signals:
    
public slots:

private:

    static int _idConexion; ///< Identificador de la conexion
    static QString _nombreConexionBase; ///< Nombre base de las conexiones empleadas

    QSqlDatabase db;    ///< Conexion con la base de datos sqlite
    QString _path;      ///< Path del archivo de base de datos
    QString _lastError; ///< Ultimo error producido

    QSqlTableModel *_modeloScripts; ///< Modelo de scripts

    /*!
      Crea un almacen determinado
      @param db Conexion de la base de datos
      @pre La conexion debe de estar abierta
      @note No modifica el objeto
      @post Se ha creado la estructura de un almacen de scripts dentro de la base de datos
      */

    static bool crearAlmacenLite(QSqlDatabase db);
};

#endif // SMSTOLITE_H
