#include "addactividaddialog.h"
#include "ui_addactividaddialog.h"

AddActividadDialog::AddActividadDialog(QWidget *parent, AcolitoDatabase *db) :
    QDialog(parent),
    database(db),
    ui(new Ui::AddActividadDialog)
{
    ui->setupUi(this);

    if(database) {
        ui->tipoComboBox->setModel(database->getModeloTiposActividad());
        ui->tipoComboBox->setModelColumn(0);

        ui->diaComboBox->setModel(database->getModeloFechas());
        ui->diaComboBox->setModelColumn(0);

        ui->lugarComboBox->setModel(database->getModeloLocalizaciones());
        ui->lugarComboBox->setModelColumn(0);
    }

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(vigilarButtonBox(QAbstractButton*)));
}

AddActividadDialog::~AddActividadDialog()
{
    delete ui;
}

void AddActividadDialog::vigilarButtonBox(QAbstractButton *botonPulsado)
{
    if(ui->buttonBox->buttonRole(botonPulsado)==QDialogButtonBox::AcceptRole) {

        if(ui->actividadLine->text().isEmpty() || ui->actividadLine->text().isNull()) {
            QMessageBox::warning(this,tr("Nueva actividad - Error en formulario"),tr("No se ha introducido un nombre para la actividad."));
            return;
        }

        QVariantMap datosActividad;

        datosActividad["nombre"]=ui->actividadLine->text();

        if(ui->asociacionLine->text().isEmpty())
            datosActividad["asociacion"]=QString(" ");
        else datosActividad["asociacion"]=ui->asociacionLine->text();

        if(ui->responsableLine->text().isEmpty())
            datosActividad["responsable"]=QString(" ");
        else datosActividad["responsable"]=ui->responsableLine->text();

        if(ui->correoLine->text().isEmpty())
            datosActividad["correo"]=QString(" ");
        else datosActividad["correo"]=ui->correoLine->text();

        if(ui->coorganizadorLine->text().isEmpty())
            datosActividad["coorganizadores"]=QString(" ");
        else datosActividad["coorganizadores"]=ui->coorganizadorLine->text();

        if(ui->mPropioText->toPlainText().isEmpty())
            datosActividad["mPropio"]=QString(" ");
        else datosActividad["mPropio"]=ui->mPropioText->toPlainText();

        if(ui->mOrgText->toPlainText().isEmpty())
            datosActividad["mOrganizacion"]=QString(" ");
        else datosActividad["mOrganizacion"]=ui->mOrgText->toPlainText();

        if(ui->espacioText->toPlainText().isEmpty())
            datosActividad["espacio"]=QString(" ");
        else
            datosActividad["espacio"]=ui->espacioText->toPlainText();

        datosActividad["dia"]=ui->diaComboBox->currentText();

        datosActividad["todaJornada"]=ui->allJornadasCheckBox->isChecked();

        datosActividad["horaInicio"]=ui->horaInicioTime->time();
        datosActividad["horaFin"]=ui->horaFinTime->time();
        datosActividad["minimoPart"]=ui->minPartSpinBox->value();
        datosActividad["maximoPart"]=ui->maxPartSpinBox->value();
        datosActividad["tipo"]=ui->tipoComboBox->currentText();

        if(ui->descText->toPlainText().isEmpty())
            datosActividad["descripcion"]=QString(" ");
        else datosActividad["descripcion"]=ui->descText->toPlainText();

        if(ui->reqText->toPlainText().isEmpty())
            datosActividad["requisitos"]=QString(" ");
        else datosActividad["requisitos"]=ui->reqText->toPlainText();

        if(ui->observacionesText->toPlainText().isEmpty())
            datosActividad["observaciones"]=QString(" ");
        else datosActividad["observaciones"]=ui->observacionesText->toPlainText();

        datosActividad["lugar"]=ui->lugarComboBox->currentText();

        database->addActividad(datosActividad);

    }
}
