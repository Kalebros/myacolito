#ifndef SMCONTAINERWIDGET_H
#define SMCONTAINERWIDGET_H

#include "SManager_global.h"

#include "smscriptform.h"
#include "smstorage.h"
#include "smscript.h"

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class SMContainerWidget;
}

/*!
  Widget contenedor de ScriptForms
 */

class SMANAGERSHARED_EXPORT SMContainerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SMContainerWidget(QWidget *parent = 0,SMStorage *sto=0);
    ~SMContainerWidget();

    /*!
      Cambia el almacen de scripts
      */

    void setSMStorage(SMStorage *sto);
    
signals:

    void execScript(SMScript *script);

private:
    Ui::SMContainerWidget *ui;
    SMStorage *almacen;         ///< Almacen de scripts
    QVBoxLayout *layout;        ///< Layout para los scriptForms
    QList<SMScriptForm*> listaWidgets;  ///< Lista de widgetsºº

    /*!
      Carga los scripts del almacen
      */

    void loadScriptForms();

    /*!
      Resetea el contenedor
      */

    void resetContainer();

    /*!
      Prepara el layout del contenedor
      */

    void prepararLayout();

private slots:

    /*!
      Eliminar script
      */

    void removeScript(int id,SMStorage *sto,SMScriptForm *form);

    /*!
      Nuevo script
      */

    void addScript();

};

#endif // SMCONTAINERWIDGET_H
