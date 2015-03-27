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

#ifndef MYMODELOEVENTOS_H
#define MYMODELOEVENTOS_H

#include <QAbstractListModel>
#include <QCoreApplication>
#include <QList>
#include <QDate>
#include <QString>
#include <QVariant>
#include <QIcon>
#include <QMap>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QStandardItemModel>

/*!
  Clase publica de Evento
  En esta clase se almacena el nombre del evento,
  la lista de fechas del evento y el nombre de la conexion
  */

class Evento
{
public:

    explicit Evento();

    void nombre(QString n);
    QString nombre() const;

    void database(QString d);
    QString database() const;

    void comentario(QString c);
    QString comentario() const;

    void addFecha(QDate d);
    void addFecha(QList<QDate> lista);
    void addFecha(QList<QVariant> lista);
    void removeFecha(QDate d);
    QList<QDate> getFechas() const;

private:

    QString _nombre;    ///< Nombre del evento
    QString _database;  ///< Nombre de la database
    QString _comentario;    ///< Comentario del evento
    QList<QDate> _fechas;   ///< Lista de fechas

};

/*!
  Modelo de eventos, para mostrar los eventos
  almacenados en la base de datos
  @brief
  Este modelo de eventos esta basado en una base
  de datos MySQL, donde las tablas existentes
  son dos:
  -@e Listado: En la tabla de listado se almacena
  el nombre del evento y el comentario del mismo.
  -@e Fechas: En la tabla de fechas se almacenan
  los dias en los cuales tendra lugar el evento.

  @note
  Para mantener en la medida de lo posible la
  compatibilidad entre distintos modelos, las interfaces
  son similares entre este modelo y el modelo de AcoLITE
  */

class MyModeloEventos : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QIcon iconoOk READ getOkIcon WRITE setOkIcon)
    Q_PROPERTY(QIcon iconoError READ getErrorIcon WRITE setErrorIcon)
public:

    enum DatosEvento
    { Directorio=32,Nombre=33,Comentario=34,Fechas=35,Icono=36 };

    /*!
      Clase contenedora de datos de eventos
      */
    /*
    class ItemEvento
    {
    public:
        ItemEvento();
        ~ItemEvento();

        void setDirectorio(QString dir);
        void setNombre(QString nom);
        void setComentario(QString comentario);
        void addFecha(QDate fecha);
        void setIcono(QIcon *icono);

        QString getDirectorio() const;
        QString getNombre() const;
        QString getComentario() const;
        QString getFechasString() const;
        QList<QDate> getListaFechas() const;
        QIcon *getIcono();

    private:
        QString directorio; ///< Directorio donde se almacena el evento
        QString nombre;     ///< Nombre del evento
        QList<QDate> fechas;    ///< Listado de fechas del evento
        QString comentario;     ///< Comentario del evento
        QIcon *iconoAsociado;   ///< Icono asociado al evento;
    };
    */

    explicit MyModeloEventos(QObject *parent = 0,const QSqlDatabase &db=QSqlDatabase());
    ~MyModeloEventos();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QIcon getOkIcon() const;
    QIcon getErrorIcon() const;

signals:

    /*!
      Indica que se ha incluido un nuevo evento en la base de datos de eventos
      */

    void nuevoEvento(Evento *ev,QStandardItemModel *categorias);

public slots:

    void setOkIcon(QIcon icono);
    void setErrorIcon(QIcon icono);

    void addEvento(Evento *ev,QStandardItemModel *categorias);

    void reload();

private:

    QIcon okIcon;
    QIcon errorIcon;

    QSqlTableModel *_modeloNombres;     ///< Nombres y comentarios
    QSqlTableModel *_modeloFechas;      ///< Fechas

    void setupModel(const QSqlDatabase &db);

    /*!
      Devuelve las fechas que corresponden al evento
      indicado
      @param idx    Indice del evento
      @return QString
      @note No modifica el objeto
      */
    QString getFechasString(const QModelIndex idx) const;

    /*!
      Convierte una lista de fechas en una cadena de texto
      */

    QString convertToString(QList<QDate> lista) const;
};

#endif // MYMODELOEVENTOS_H
