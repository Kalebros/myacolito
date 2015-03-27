#ifndef IMPRIMIRACTIVIDADDIALOG_H
#define IMPRIMIRACTIVIDADDIALOG_H

#include <QDialog>

#include <Databases/acolitodatabase.h>

namespace Ui {
class ImprimirActividadDialog;
}

class ImprimirActividadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImprimirActividadDialog(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~ImprimirActividadDialog();

signals:

    void imprimirDia(QSortFilterProxyModel *m);
    
private:
    Ui::ImprimirActividadDialog *ui;

    AcolitoDatabase *database;
    QSortFilterProxyModel *modelo;

private slots:

    void solicitarImpresion();
};

#endif // IMPRIMIRACTIVIDADDIALOG_H
