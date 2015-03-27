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

#ifndef MYACOLITOASISTENTES_H
#define MYACOLITOASISTENTES_H

#include <QWidget>
#include <QIcon>
#include <QMessageBox>

#include "myacolito.h"
#include "addpersonasdialog.h"
#include "categoriasdelegate.h"
#include "booleanicondelegate.h"
#include "addgrupodialog.h"
#include "asistentecategoriasdialog.h"
#include "alojargrupodialog.h"
#include "alojarasistentedialog.h"

//Reportes
#include <BReport/libBReport.h>

namespace Ui {
class MyAcolitoAsistentes;
}

class MyAcolitoAsistentes : public MyAcolito
{
    Q_OBJECT
    Q_PROPERTY(QIcon okIcon READ getOkIcon WRITE setOkIcon)
    Q_PROPERTY(QIcon falseIcon READ getFalseIcon WRITE setFalseIcon)

public:
    explicit MyAcolitoAsistentes(QWidget *parent = 0,MyDatabase *db=0);
    ~MyAcolitoAsistentes();

    QIcon getOkIcon() const
    { return okIcon; }

    QIcon getFalseIcon() const
    { return falseIcon; }

public slots:

    virtual void reloadModelos();

    virtual void clear();

    /*!
      Lanza el dialogo para añadir nuevos asistentes
      */
    void dialogoNuevosAsistentes();

    /*!
      Lanza el dialogo para agrupar asistentes
      */

    void dialogoAgruparAsistentes();

    /*!
      Lanza el dialogo para editar categorias
      */

    void dialogoCategorias();

    /*!
      Lanza el dialogo para alojar a un grupo en una habitacion
      */

    void dialogoAlojarGrupo();

    /*!
      Lanza el dialogo para alojar a un asistente en una habitacion
      */

    void dialogoAlojarAsistente();

    void setOkIcon(QIcon icon);

    void setFalseIcon(QIcon icon);

signals:

    void solicitarReporte(QString mensaje,QVariantMap datos,ReportOptions opciones);

private:
    Ui::MyAcolitoAsistentes *ui;

    QSortFilterProxyModel *modeloAsistentes;
    QSortFilterProxyModel *filtroPagos;
    QSortFilterProxyModel *filtroActivo;
    QSortFilterProxyModel *filtroBusqueda;
    QSortFilterProxyModel *filtroVistaGrupos;

    QSortFilterProxyModel *modeloAlojamientos;
    QDataWidgetMapper *mapeadorBase;

    QIcon okIcon;
    QIcon falseIcon;

    QIcon openIcon;
    QIcon closeIcon;

    /*!
      Prepara la vista del acolito
      */
    void setupVista();

    /*!
      Prepara el acolito
      */

    void setup();

    /*!
      Prepara los filtros
      */

    void setupFiltros();

    /*!
      Prepara el mapeador
      */

    void setupMapeador();

    /*!
      Borra los filtros
      */

    void deleteFiltros();

    /*!
      Prepara el boton de entrega de llave
      */

    void setBotonLlave();

    /*!
      Recupera los datos del asistente marcado
      */

    QVariantMap getDatosAsistente();


private slots:

    /*!
      Mostrar opciones de visualizacion
      */

    void showOpcionesVisualizacion(bool estado);

    /*!
      Filtrado de opciones de activos
      */

    void setFiltradoActivo(int estado);

    /*!
      Filtrado de opciones de pago
      */

    void setFiltradoPago(int estado);

    /*!
      Filtrado de opciones de busqueda
      */

    void setColumnaBusqueda(int index);

    /*!
      Cambia el estado de activacion de un asistente al pulsar el boton
      */

    void cambiarEstadoAsistente();

    /*!
      Solicita el borrado de los asistentes seleccionados en la vista
      */

    void eliminarAsistentes();

    /*!
      Filtra las habitaciones para mostrar la habitacion asignada al grupo
      */

    void filtrarAlojamiento(QString grupo);

    /*!
      Recarga la información de alojamiento tras
      cambios en el modelo de alojamientos
      */

    void recargarDatosHabitacion();

    /*!
      Entrega la llave de la habitacion indicada
      */

    void entregarLlaveHabitacion();

    /*!
      Separa al asistente de su grupo
      */

    void separarAsistente();

    /*!
      Prepara e imprime los datos de un asistente
      */

    void printAsistente();

    /*!
      Prepara y muestra los datos de un asistente para exportar
      */

    void exportarAsistente();

    /*!
      DEPURADO STRING
      */

    void depurar(QString cad);
};

#endif // MYACOLITOASISTENTES_H
