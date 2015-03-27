#ifndef ACTCOMPLETADIALOG_H
#define ACTCOMPLETADIALOG_H

#include <QDialog>

#include <Databases/acolitodatabase.h>
#include <QDataWidgetMapper>

namespace Ui {
class ActCompletaDialog;
}

class ActCompletaDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ActCompletaDialog(QWidget *parent = 0,int idActividad=0,AcolitoDatabase *db=0);
    ~ActCompletaDialog();
    
private:
    Ui::ActCompletaDialog *ui;

    int actividad;
    AcolitoDatabase *database;  ///< Conexion con base de datos

    QSortFilterProxyModel *modeloFechas;
    QSortFilterProxyModel *modeloLocalizacion;
    QSortFilterProxyModel *modeloTipos;
    QSortFilterProxyModel *modeloActividades;

    QDataWidgetMapper *mapeador;    ///< Mapeador de datos

    /*!
      Prepara el mapeador
      */

    void setupMapeador();
};

#endif // ACTCOMPLETADIALOG_H
