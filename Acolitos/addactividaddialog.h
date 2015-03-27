#ifndef ADDACTIVIDADDIALOG_H
#define ADDACTIVIDADDIALOG_H

#include <QDialog>

#include <QVariantMap>
#include <QDialogButtonBox>
#include <QMessageBox>

#include <Databases/acolitodatabase.h>

namespace Ui {
class AddActividadDialog;
}

class AddActividadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddActividadDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~AddActividadDialog();
    
private:
    Ui::AddActividadDialog *ui;
    AcolitoDatabase *database;  ///< COnexion con base de datos

private slots:

    void vigilarButtonBox(QAbstractButton *botonPulsado);
};

#endif // ADDACTIVIDADDIALOG_H
