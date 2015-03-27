/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of MyAcolito.
**
** MyAcolito is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** MyAcolito is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with MyAcolito.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#ifndef MYACOLITOACTIVIDADES_H
#define MYACOLITOACTIVIDADES_H

#include <QWidget>

#include <QDataWidgetMapper>

#include <BReport/breportmanager.h>

#include <QTextDocument>
#include <QTextCursor>
#include <QTextFormat>
#include <QPrinter>
#include <QPrintDialog>
#include <QHeaderView>


#include "myacolito.h"
#include "tipoactividaddialog.h"
#include "localizacionesdialog.h"
#include "addactividaddialog.h"
#include "actcompletadialog.h"
#include "imprimiractividaddialog.h"

#include "booleanicondelegate.h"
#include "fechasdelegate.h"
#include "tipodelegate.h"

//BReport
#include <BReport/libBReport.h>

namespace Ui {
class MyAcolitoActividades;
}

class MyAcolitoActividades : public MyAcolito
{
    Q_OBJECT
    
public:
    explicit MyAcolitoActividades(QWidget *parent = 0,MyDatabase *db=0);
    ~MyAcolitoActividades();

signals:

    void solicitarReporteActividades(QString reporte,QVariantMap datos,ReportOptions opciones);
    void solicitarReportePorDia(QString reporte,QList<QVariantMap> datos,ReportOptions opciones);

public slots:

    virtual void reloadModelos();
    virtual void clear();

    /*!
      Muestra el dialogo de tipos de actividades
      */

    void dialogoTiposActividad();

    /*!
      Muestra el dialogo de localizaciones
      */

    void dialogoLocalizaciones();

    /*!
      Muestra el dialogo de nuevas actividades
      */

    void dialogoNuevaActividad();

    /*!
      Muestra el dialogo para imprimir actividades por dias
      */

    void dialogoImpresionPorDias();


private slots:

    /*!
      Activa o desactiva el modo 'todas las jornadas' para una actividad
      */

    void setModoTodoJornadas();

    /*!
      Actualiza el widget de horarios
      */

    void actualizarWidgetHorarios(int modo);

    /*!
      Vigila widget horarios
      */

    void vigilaCambiosModoJornadas(QModelIndex indexA,QModelIndex indexB);

    /*!
      Filtra por tipos
      */

    void filtrarPorTipos(QString tipo);

    /*!
      Filtra por fechas
      */

    void filtrarPorFechas(QString fecha);

    /*!
      Carga el comboBox de tipos
      */

    void loadTipoComboBox();

    /*!
      Carga el comboBox de fechas
      */

    void loadFechasComboBox();

    /*!
      Elimina una actividad seleccionada
      */

    void deleteActividad();

    /*!
      Muestra la vista completa de una actividad
      */

    void dialogoVistaCompleta();

    /*!
      Envia un mensaje de impresion de actividad
      */

    void solicitarImpresion();

    /*!
      Envia un mensaje para guardar la actividad como un archivo (PDF u ODT)
      */

    void solicitarImportarDialogo();

    /*!
      Imprime una serie de actividades de un modelo seleccionado
      */

    void solicitarImpresion(QSortFilterProxyModel *actividadesDelDia);


private:
    Ui::MyAcolitoActividades *ui;

    QSortFilterProxyModel *modeloActividades;   ///< Modelo de actividades
    QSortFilterProxyModel *filtroTipos;
    QSortFilterProxyModel *filtroFechas;
    QDataWidgetMapper *mapeador;

    /*!
      Prepara el acolito
      */

    void setup();

    /*!
      Prepara el mapeador
      */

    void setupMapeador();

    /*!
      Prepara la vista
      */

    void setupVista();

    /*!
      Prepara los filtros
      */

    void setupFiltros();

    /*!
      Devuelve los datos de la actividad en un QVariantMap
      */

    QVariantMap datosActividadMostrada();

    /*!
     * Genera la lista de datos de actividades
     * a partir de un QSortFilterProxyModel
     */

    QList<QVariantMap> datosListaActividades(QSortFilterProxyModel *modActividades);

};

#endif // MYACOLITOACTIVIDADES_H
