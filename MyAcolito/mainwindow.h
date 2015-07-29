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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QSettings>

#include <Databases/myeventodb.h>
#include <Databases/mydatabase.h>

#include <WizardEvento/wizardevento.h>
#include <Acolitos/myacolitoasistentes.h>
#include <Acolitos/addpersonasdialog.h>
#include <Acolitos/myacolitohabitaciones.h>
#include <Acolitos/myacolitoactividades.h>
#include <Componentes/slidingstackedwidget.h>

//Libreria BReport
#include <BReport/libBReport.h>

#include "logdialog.h"
#include "conexionessql.h"
#include "preferenciasdialog.h"
#include "delegadoevento.h"
#include "eventodialog.h"
#include "gestorpaginas.h"
#include "gestorinformes.h"
#include "procesadoractividades.h"


//Libreria reportes
#include <ReportesAcolito/factoriaactividades.h>
#include <ReportesAcolito/factoriaasistentes.h>
#include <ReportesAcolito/factoriahabitaciones.h>

//Libreria scripts
#include <SManager/smanager.h>
#include <SManager/smscript.h>

//Parseado de documentos JSON
#include <AcolitoJSON/acolitojsonparser.h>

//Mensajes
#include "mensajesdialog.h"

namespace Ui {
class MainWindow;
}

/*!
  Ventana principal de la aplicacion MyAcolite
  */

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    Q_ENUMS(EstadoConexion)
    Q_PROPERTY(EstadoConexion stateConnection READ getEstado WRITE setEstado)
public:

    enum EstadoConexion
    { Conectado=1, Desconectado=0 };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    EstadoConexion getEstado() const;

    void setEstado(EstadoConexion est);

protected:

    void closeEvent(QCloseEvent *e);

signals:

    /*!
      Carga los acolitos
      */

    void reloadAcolitos(AcolitoDatabase *db);

    /*!
      Limpia los acolitos
      */

    void clearAcolitos();

private slots:

    /*!
      Intenta una conexion / desconexion al pulsar en la accion correspondiente
      */

    void switchEstadoConexion(bool estado);

    /*!
      Lanza el dialogo de preferencias
      */

    void verPreferencias();

    /*!
      Recibe una solicitud para crear un nuevo evento
      */

    void crearEvento(Evento *ev,QStandardItemModel *categorias);

    /*!
      Lanza un dialogo de eventos
      */

    void lanzarDialogoEventos();

    /*!
      Muestra un dialogo de error
      */

    void error(QString mensaje);

private:
    Ui::MainWindow *ui;

    EstadoConexion estadoConexion;
    MyEventoDB *dbEventos;      ///< Database de eventos
    WizardEvento *asistenteEventos; ///< Asistente de nuevos eventos
    MyAcolitoDB *conexionEvento;    ///< Conexion para el evento
    AcolitoDatabase *database;           ///< Database del evento

    GestorPaginas *gPaginas;    ///< Gestor de paginas
    GestorInformes *gInformes;  ///< Gestor de informes
    BReportManager *gestorReportes;     ///< Gestor de reportes
    SManager *gestorScripts;            ///< Gestor de scripts
    AcolitoJSONParser *parserJson;      ///< Parseador de documentos json

    QProgressDialog *progresoScriptDialog;  ///< Dialogo de progreso de scripts lanzados

    MyAcolitoAsistentes *acolitoAsistentes; ///< Acolito de asistentes
    MyAcolitoHabitaciones *acolitoHabitaciones; ///< Acolito de habitaciones
    MyAcolitoActividades *acolitoActividades;   ///< Acolito de actividades

    /*!
      Prepara la ventana principal
      */
    void setup();

    /*!
      Escribe las preferencias del programa en el registro
      */

    void writeSettings();

    /*!
      Lee las preferencias del programa del registro
      */

    void readSettings();

private slots:

    /*!
      Abre un evento
      */

    void abrirEvento(Evento *ev);

    /*!
      Procesa una carpeta de actividades
      */

    void procesarCarpeta();

    /*!
      DEBUG
      */

    void mostrarDebug();

    /*!
      Se ha lanzado un script
      */

    void scriptIniciado(SMScript *s);

    /*!
      Se ha terminado un script
      */

    void scriptFinalizado(SMScript* s);

    /*!
      Lanza el dialogo de mensajes
      */

    void dialogoMensajes();

};

#endif // MAINWINDOW_H
