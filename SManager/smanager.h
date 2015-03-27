#ifndef SMANAGER_H
#define SMANAGER_H

#include "SManager_global.h"
#include "smscript.h"

#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QProcess>
#include <QFileInfo>

#include <QDebug>

/*!
  Gestor de scripts.

  Lanza un script, recopila la salida y devuelve los datos en formato JSON

  Para utilizar con Qt 5.x
 */

class SMANAGERSHARED_EXPORT SManager : public QObject
{
    Q_OBJECT

public:
    explicit SManager(QObject *parent = 0);

    /*!
      Devuelve el resultado de la ejecucion de un script
      @pre El script ha sido ejecutado
      @return Documento devuelto por el script
      @note No modifica el objeto
     */
    QJsonDocument *getScriptResults()
    { return documento; }

    /*!
      Devuelve si el manager esta funcionando o no
      (si hay algun script en ejecucion)
      @return bool, true: esta activo, false: esta libre
      @note No modifica el objeto
      */

    bool isActive() const;

signals:

    /*!
      Se ha iniciado la ejecucion de un script
     */
    void initScript(SMScript* script);

    /*!
      Ha terminado la ejecucion de un script
     */
    void finishedScript(SMScript *script);

    /*!
      Error en el script
      */

    void error(QString error);
    
public slots:

    /*!
      Ejecuta un script
      @param script Datos del script a ejecutar
      */

    void execScript(SMScript *script);

    /*!
      Cancelar ejecucion
      */

    void cancelScript();

private:

    bool _isActive;             ///< Flag de control de ejecucion
    bool _isCanceled;           ///< Flag de control de cancelacion
    QJsonDocument *documento;   ///< Documento devuelto
    QProcess *procesoActivo;    ///< Proceso activo
    SMScript *scriptActivo;     ///< Script que se esta ejecutando

private slots:

    /*!
      Controla el inicio del script
      */

    void procesoIniciado();

    /*!
      Controla el final del script
      */

    void procesoTerminado(int code,QProcess::ExitStatus status);

    /*!
      Cambio de estado
      */

    void cambioEstado(QProcess::ProcessState estado);

    /*!
      Error
      */

    void errorProceso(QProcess::ProcessError error);
    
};

#endif // SMANAGER_H
