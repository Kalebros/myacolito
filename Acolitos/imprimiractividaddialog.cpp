#include "imprimiractividaddialog.h"
#include "ui_imprimiractividaddialog.h"

ImprimirActividadDialog::ImprimirActividadDialog(QWidget *parent,AcolitoDatabase *db) :
    QDialog(parent),
    database(db),
    ui(new Ui::ImprimirActividadDialog)
{
    ui->setupUi(this);
    if(database) {
        ui->comboBox->setModel(database->getModeloFechas());

        modelo=database->getModeloActividades();
        modelo->setFilterKeyColumn(9);

        ui->vistaActividades->setModel(modelo);
        ui->vistaActividades->setModelColumn(1);

        ui->comboBox->setCurrentIndex(0);

        modelo->setFilterFixedString(ui->comboBox->currentText());

        connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),modelo,SLOT(setFilterFixedString(QString)));
        connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(solicitarImpresion()));
        connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(reject()));
    }
}

ImprimirActividadDialog::~ImprimirActividadDialog()
{
    delete ui;
}

void ImprimirActividadDialog::solicitarImpresion()
{
    emit imprimirDia(modelo);
    this->accept();
}
