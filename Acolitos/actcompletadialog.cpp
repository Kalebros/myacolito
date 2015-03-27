#include "actcompletadialog.h"
#include "ui_actcompletadialog.h"

ActCompletaDialog::ActCompletaDialog(QWidget *parent, int idActividad, AcolitoDatabase *db) :
    QDialog(parent),
    actividad(idActividad),
    database(db),
    ui(new Ui::ActCompletaDialog)
{
    ui->setupUi(this);

    if(database) {
        modeloFechas=database->getModeloFechas();
        modeloLocalizacion=database->getModeloLocalizaciones();
        modeloActividades=database->getModeloActividades();
        modeloTipos=database->getModeloTiposActividad();

        ui->lugarComboBox->setModel(modeloLocalizacion);
        ui->diaComboBox->setModel(modeloFechas);
        ui->tipoComboBox->setModel(modeloTipos);

        modeloActividades->setFilterKeyColumn(0);

        //Preparar filtro de actividades
        QString numero=QString::number(actividad);

        QString filtro;
        for(int i=0;i<numero.count();i++) {
            filtro.append(numero.at(i));
            filtro.append("{1,1}");
        }
        filtro.prepend('^');
        filtro.append("$");

        modeloActividades->setFilterRegExp(filtro);

        setupMapeador();

    }
}

ActCompletaDialog::~ActCompletaDialog()
{
    delete ui;
}

void ActCompletaDialog::setupMapeador()
{
    mapeador=new QDataWidgetMapper(this);

    mapeador->setModel(modeloActividades);

    mapeador->addMapping(ui->actividadLine,1,"text");
    mapeador->addMapping(ui->asociacionLine,2,"text");
    mapeador->addMapping(ui->responsableLine,3,"text");
    mapeador->addMapping(ui->correoLine,4,"text");
    mapeador->addMapping(ui->coorganizadorLine,5,"text");
    mapeador->addMapping(ui->mPropioText,6,"plainText");
    mapeador->addMapping(ui->mOrgText,7,"plainText");
    mapeador->addMapping(ui->espacioText,8,"plainText");
    mapeador->addMapping(ui->diaComboBox,9,"texto");
    mapeador->addMapping(ui->allJornadasCheckBox,10,"estado");
    mapeador->addMapping(ui->horaInicioTime,11,"time");
    mapeador->addMapping(ui->horaFinTime,12,"time");
    mapeador->addMapping(ui->minPartSpinBox,13,"value");
    mapeador->addMapping(ui->maxPartSpinBox,14,"value");
    mapeador->addMapping(ui->tipoComboBox,15,"texto");
    mapeador->addMapping(ui->descText,16,"plainText");
    mapeador->addMapping(ui->reqText,17,"plainText");
    mapeador->addMapping(ui->observacionesText,18,"plainText");
    mapeador->addMapping(ui->lugarComboBox,19,"texto");

    mapeador->toFirst();
}
