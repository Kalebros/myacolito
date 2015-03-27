#ifndef SMSCRIPTFORM_H
#define SMSCRIPTFORM_H

#include "SManager_global.h"

#include <QWidget>
#include <QFileDialog>

#include "smscript.h"
#include "smstorage.h"

namespace Ui {
class SMScriptForm;
}

class SMANAGERSHARED_EXPORT SMScriptForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit SMScriptForm(QWidget *parent = 0,SMStorage *sto=0,int idScript=-1);
    ~SMScriptForm();

signals:

    /*!
      Borrar el script asignado
      */

    void deleteScript(int idScript,SMStorage *almacen,SMScriptForm *form);

    /*!
      Ejecutar un script
      */

    void execScript(SMScript *script);
    
private:
    Ui::SMScriptForm *ui;

    SMStorage *almacen;     ///< Almacen sobre el que existe el script
    SMScript *script;       ///< Script sobre el que trabajamos
    int _scriptEnAlmacen;   ///< Identificador del script dentro del almacen

private slots:

    void deleteClicked();
    void execClicked();
    void selectPath();
};

#endif // SMSCRIPTFORM_H
