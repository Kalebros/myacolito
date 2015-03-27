#ifndef MENSAJESDIALOG_H
#define MENSAJESDIALOG_H

#include <QDialog>

#include <Databases/acolitodatabase.h>
#include <QInputDialog>

namespace Ui {
class MensajesDialog;
}

class MensajesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MensajesDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~MensajesDialog();
    
private:
    Ui::MensajesDialog *ui;
    AcolitoDatabase *database;  ///< Base de datos de acolito
    QSortFilterProxyModel *modeloMensajes;  ///< Modelo de mensajes


    /*!
      Prepara el dialogo
      */

    void setup();

    /*!
      Carga el modelo de mensajes
      */

    void loadModeloMensajes();

private slots:

    /*!
      Crea la tabla de mensajes
      */

    void crearTablaMensajes();

    /*!
      Nuevo mensaje
      */

    void addMensaje();

    /*!
      Borrar mensajes
      */

    void borrarMensajes();
};

#endif // MENSAJESDIALOG_H
