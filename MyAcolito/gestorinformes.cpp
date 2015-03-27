/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ram�rez Marti
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

#include "gestorinformes.h"

GestorInformes::GestorInformes(QObject *parent) :
    QObject(parent),
    _database(0)
{
}

void GestorInformes::setDatabase(AcolitoDatabase *database)
{
    _database=database;
}

void GestorInformes::reporteActividadesPorDias()
{
    if(_database) {
        QSortFilterProxyModel *modeloActividades=_database->getModeloActividades();
        QSortFilterProxyModel *modeloFechas=_database->getModeloFechas();

        if(modeloActividades && modeloFechas) {
            QVariantMap datosInforme;
            modeloActividades->sort(11,Qt::AscendingOrder); //Ordenar segun la hora de inicio
                                                            //Consultar columna en Databases/mymodeloactividades
            modeloFechas->sort(0,Qt::AscendingOrder);
            //Recuperar informacion de los dias
            QList<QDate> listaFechas;
            int numFechas=modeloFechas->rowCount();
            for(int i=0;i<numFechas;i++) {
                QDate fecha=modeloFechas->index(i,0).data(Qt::EditRole).toDate();
                listaFechas.append(fecha);
            }

            modeloActividades->setFilterKeyColumn(9);    //Consultar columna de filtrado en Databases/mymodeloactividades
            QList<QVariant> datosDias;
            //Por cada dia, recuperar los datos de cada actividad
            foreach(QDate fecha,listaFechas) {
                QVariantMap dia;
                dia.insert("fecha",fecha);
                modeloActividades->setFilterFixedString(fecha.toString("yyyy-MM-dd"));

                int numActividades=modeloActividades->rowCount();
                dia.insert("numActividades",numActividades);
                QList<QVariant> listadoActividades;
                for(int i=0;i<numActividades;i++) {
                    QVariantMap datosActividad;
                    datosActividad.insert("inicio",modeloActividades->index(i,11).data(Qt::EditRole).toTime());
                    datosActividad.insert("nombre",modeloActividades->index(i,1).data(Qt::EditRole).toString());
                    datosActividad.insert("categoria",modeloActividades->index(i,15).data(Qt::EditRole).toString());
                    datosActividad.insert("lugar",modeloActividades->index(i,19).data(Qt::EditRole).toString());
                    listadoActividades.append(datosActividad);
                }
                dia.insert("actividades",listadoActividades);
                datosDias.append(dia);
            }

            datosInforme.insert("listado",datosDias);
            emit solicitarInforme(tr("FactoriaActividades::ActividadesPorDias"),datosInforme,ShowOnDialog);
        }
    }
}

void GestorInformes::reporteActividadesPorTipos()
{
    if(_database) {
        QSortFilterProxyModel *modeloActividades=_database->getModeloActividades();
        QSortFilterProxyModel *modeloTipos=_database->getModeloTiposActividad();

        QSortFilterProxyModel *filtroActividades=new QSortFilterProxyModel(this);   // Filtro para los dias
        filtroActividades->setSourceModel(modeloActividades);

        if(modeloActividades && modeloTipos) {
            QVariantMap datosInforme;
            modeloActividades->sort(11,Qt::AscendingOrder); //Ordenar segun la hora de inicio
                                                            //Consultar columna en Databases/mymodeloactividades

            filtroActividades->sort(9,Qt::AscendingOrder);  // Ordenar segun la fecha
                                                            // Consultar columna en Databases/mymodeloActividades
            modeloTipos->sort(0,Qt::AscendingOrder);
            //Recuperar informacion de las categorias
            QList<QString> listaTipos;
            int numTipos=modeloTipos->rowCount();
            for(int i=0;i<numTipos;i++)
                listaTipos.append(modeloTipos->index(i,0).data(Qt::EditRole).toString());

            filtroActividades->setFilterKeyColumn(15);    //Consultar columna de filtrado en Databases/mymodeloactividades
            QList<QVariant> datosTipos;
            //Por cada tipo, recuperar los datos de cada actividad
            foreach(QString tipo,listaTipos) {
                QVariantMap categoria;
                categoria.insert("categoria",tipo);
                filtroActividades->setFilterFixedString(tipo);

                int numActividades=filtroActividades->rowCount();
                categoria.insert("numActividades",numActividades);
                QList<QVariant> listadoActividades;
                for(int i=0;i<numActividades;i++) {
                    QVariantMap datosActividad;
                    datosActividad.insert("inicio",filtroActividades->index(i,11).data(Qt::EditRole).toTime());
                    datosActividad.insert("nombre",filtroActividades->index(i,1).data(Qt::EditRole).toString());
                    datosActividad.insert("fecha",filtroActividades->index(i,9).data(Qt::EditRole).toDate());
                    datosActividad.insert("lugar",filtroActividades->index(i,19).data(Qt::EditRole).toString());
                    listadoActividades.append(datosActividad);
                }
                categoria.insert("actividades",listadoActividades);
                datosTipos.append(categoria);
            }

            datosInforme.insert("listado",datosTipos);
            emit solicitarInforme(tr("FactoriaActividades::ActividadesPorTipos"),datosInforme,ShowOnDialog);
        }
    }
}

void GestorInformes::reporteActividadesPorLocalizacion()
{
    if(_database) {
        QSortFilterProxyModel *modeloActividades=_database->getModeloActividades();
        QSortFilterProxyModel *modeloLocalizacion=_database->getModeloLocalizaciones();

        QSortFilterProxyModel *filtroActividades=new QSortFilterProxyModel(this);   // Filtro para los dias
        filtroActividades->setSourceModel(modeloActividades);

        if(modeloActividades && modeloLocalizacion) {
            QVariantMap datosInforme;
            modeloActividades->sort(11,Qt::AscendingOrder); //Ordenar segun la hora de inicio
                                                            //Consultar columna en Databases/mymodeloactividades

            filtroActividades->sort(9,Qt::AscendingOrder);  // Ordenar segun la fecha
                                                            // Consultar columna en Databases/mymodeloActividades
            modeloLocalizacion->sort(0,Qt::AscendingOrder);
            //Recuperar informacion de las localizaciones
            QList<QString> listaLugares;
            int numLugares=modeloLocalizacion->rowCount();
            for(int i=0;i<numLugares;i++)
                listaLugares.append(modeloLocalizacion->index(i,0).data(Qt::EditRole).toString());

            filtroActividades->setFilterKeyColumn(19);    //Consultar columna de filtrado en Databases/mymodeloactividades
            QList<QVariant> datosLugares;
            //Por cada tipo, recuperar los datos de cada actividad
            foreach(QString lugar,listaLugares) {
                QVariantMap localizacion;
                localizacion.insert("lugar",lugar);
                filtroActividades->setFilterFixedString(lugar);

                int numActividades=filtroActividades->rowCount();
                localizacion.insert("numActividades",numActividades);
                QList<QVariant> listadoActividades;
                for(int i=0;i<numActividades;i++) { //Consultar columnas de datos en Databases/mymodeloactividades
                    QVariantMap datosActividad;
                    datosActividad.insert("inicio",filtroActividades->index(i,11).data(Qt::EditRole).toTime());
                    datosActividad.insert("nombre",filtroActividades->index(i,1).data(Qt::EditRole).toString());
                    datosActividad.insert("fecha",filtroActividades->index(i,9).data(Qt::EditRole).toDate());
                    datosActividad.insert("categoria",filtroActividades->index(i,15).data(Qt::EditRole).toString());
                    listadoActividades.append(datosActividad);
                }
                localizacion.insert("actividades",listadoActividades);
                datosLugares.append(localizacion);
            }

            datosInforme.insert("listado",datosLugares);
            emit solicitarInforme(tr("FactoriaActividades::ActividadesPorLocalizacion"),datosInforme,ShowOnDialog);
        }
    }
}

void GestorInformes::reporteAsistentesPorCategorias()
{
    if(_database) {
        QSortFilterProxyModel *modeloCategorias=_database->getModeloCategorias();
        QSortFilterProxyModel *modeloAsistentes=_database->getModeloAsistentes();
        QSortFilterProxyModel *modeloHabitaciones=_database->getModeloAlojamientos();

        if(modeloCategorias && modeloAsistentes && modeloHabitaciones) {
            modeloAsistentes->sort(1,Qt::AscendingOrder);
            modeloCategorias->sort(0,Qt::AscendingOrder);

            //Recuperar informacion de las categorias de asistentes
            QList<QVariant> listado;
            int numCategorias=modeloCategorias->rowCount();
            modeloAsistentes->setFilterKeyColumn(7); //Columna de categoria
            for(int i=0;i<numCategorias;i++) {
                QVariantMap categoria;
                categoria.insert("categoria",
                                 modeloCategorias->index(i,0).data(Qt::EditRole).toString());
                //Filtrar por categorias
                modeloAsistentes->setFilterFixedString(modeloCategorias->index(i,0).data(Qt::EditRole).toString());
                int numAct=modeloAsistentes->rowCount();
                QList<QVariant> asistentes;
                for(int i=0;i<numAct;i++) {
                    QVariantMap entrada;
                    entrada.insert("nombre",
                                   modeloAsistentes->index(i,1).data(Qt::EditRole).toString());
                    entrada.insert("DNI",
                                   modeloAsistentes->index(i,2).data(Qt::EditRole).toString());
                    entrada.insert("email",
                                   modeloAsistentes->index(i,3).data(Qt::EditRole).toString());
                    entrada.insert("habitacion",
                                   _database->habitacionDelGrupo(modeloAsistentes->index(i,6).data(Qt::EditRole).toString()));
                    entrada.insert("pago",
                                   modeloAsistentes->index(i,9).data(Qt::EditRole).toBool());
                    asistentes.append(entrada);
                }
                categoria.insert("asistentes",asistentes);
                listado.append(categoria);
            }
            QVariantMap datos;
            datos.insert("listado",listado);
            emit solicitarInforme(tr("FactoriaAsistentes::AsistentesPorCategorias"),datos,ShowOnDialog);
        }
    }

}

