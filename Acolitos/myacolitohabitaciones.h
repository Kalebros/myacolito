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

#ifndef MYACOLITOHABITACIONES_H
#define MYACOLITOHABITACIONES_H

#include <QWidget>

#include <QDataWidgetMapper>
#include <QDebug>

#include <BReport/libBReport.h>

#include "myacolito.h"
#include "booleanicondelegate.h"
#include "tiposhabdelegate.h"
#include "categoriasdelegate.h"
#include "addhabitaciondialog.h"
#include "addlistahabitacionesdialog.h"
#include "habitaciontiposdialog.h"

namespace Ui {
class MyAcolitoHabitaciones;
}

class MyAcolitoHabitaciones : public MyAcolito
{
    Q_OBJECT
    Q_PROPERTY (QIcon okIcon READ getOkIcono WRITE setOkIcono)
    Q_PROPERTY (QIcon falseIcon READ getFalseIcono WRITE setFalseIcono)
    
public:
    explicit MyAcolitoHabitaciones(QWidget *parent = 0,MyDatabase *db=0);
    ~MyAcolitoHabitaciones();

    QIcon getOkIcono() const
    { return _okIcono; }

    QIcon getFalseIcono() const
    { return _falseIcono; }

    QIcon getHabAbiertaIcono() const
    { return _openIcono; }

    QIcon getHabCerradaIcono() const
    { return _closeIcono; }

signals:

    void solicitarReporteHabitacion(QString mensaje,QVariantMap datos,ReportOptions opciones);

public slots:

    virtual void reloadModelos();
    virtual void clear();

    /*!
      Establece el icono a emplear en los delegados binarios, para valores true
      @param icono  Icono a emplear
      @note Modifica el objeto
      */
    void setOkIcono(QIcon icono);

    /*!
      Establece el icono a emplear en los delegados binarios, para valores false
      @param icono Icono a emplear
      @note Modifica el objeto
      */
    void setFalseIcono(QIcon icono);

    /*!
      Establece el icono a emplear para la habitacion abierta
      @param icono Icono a emplear
      @note Modifica el objeto
      */

    void setHabAbiertaIcono(QIcon icono);

    /*!
      Establece el icono a emplear para la habitacion cerrada
      @param icono Icono a emplear
      @note Modifica el objeto
      */

    void setHabCerradaIcono(QIcon icono);

    /*!
      Lanza el dialogo para añadir una nueva habitacion
      */

    void dialogoNuevaHabitacion();

    /*!
      Lanza el dialogo para añadir una lista de habitaciones
      */

    void dialogoNuevaListaHabitaciones();

    /*!
      Lanza el dialogo de tipos de habitaciones
      */

    void dialogoTiposHabitacion();

    /*!
      Solicita un reporte de habitacion
      */

    void reporteHabitacion();

    /*!
      Solicita imprimir un reporte de habitacion
      */

    void imprimirReporteHabitacion();
    
private:
    Ui::MyAcolitoHabitaciones *ui;
    QSortFilterProxyModel *modeloHabitaciones;
    QSortFilterProxyModel *modeloAsistentes;
    QDataWidgetMapper *mapeador;

    //Filtros
    QSortFilterProxyModel *filtroHabLibre;
    QSortFilterProxyModel *filtroHabAbierta;
    QSortFilterProxyModel *filtroReservada;

    QIcon _okIcono;     ///< Icono ok, true. Empleado en los delegados
    QIcon _falseIcono;  ///< Icono false. Empleado en los delegados
    QIcon _openIcono;   ///< Icono de habitacion abierta
    QIcon _closeIcono;  ///< Icono de habitacion cerrada

    /*!
      Prepara el acolito
      */

    void setup();

    /*!
      Prepara el mapeador de secciones
      */

    void setupMapeador();

    /*!
      Prepara y conecta los filtros
      */

    void setupFiltros();

    /*!
      Prepara las vistas con los delegados necesarios
      */

    void setupVistas();

    /*!
      Borra los filtros
      */

    void borrarFiltros();

    /*!
      Obtiene los datos de la habitacion para generar un reporte
      */

    QVariantMap generarDatosHabitacion();

private slots:

    /*!
      Mostrar opciones de visualizacion
      */

    void showOpcionesVisualizacion(bool visualizar);

    /*!
      Filtrar por habitaciones libres
      */

    void filtrarHabLibres(int estado);

    /*!
      Filtrar por habitaciones abiertas
      */

    void filtrarHabAbiertas(int estado);

    /*!
      Filtrar por habitaciones reservadas
      */

    void filtrarHabReservadas(int estado);

    /*!
      Filtrar grupo de la habitacion
      */

    void filtrarGrupoHabitacion(QModelIndex indiceA,QModelIndex indiceB);

    /*!
      Reserva una habitacion
      */

    void reservarHabitacion();

    /*!
      Abre una habitacion entregando la llave
      */

    void entregaLlave();

    /*!
      Muestra u oculta los widgets indicativos de la reserva
      */

    void mostrarWidgetsReserva(int estado);

    /*!
      Desaloja una habitacion
      */

    void desalojarHabitacion();

    /*!
      Recarga el filtro de habitaciones
      */

    void reloadFiltroHabitaciones();

};

#endif // MYACOLITOHABITACIONES_H
