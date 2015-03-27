/***********************************************************************
** BReport, copyright @2012,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of BReport.
**
** BReport is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** BReport is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with BReport.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#ifndef BREPORT_H
#define BREPORT_H

#include "BReport_global.h"

#include <QObject>
#include <QVariant>
#include <QDir>
#include <QFile>
#include <QList>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QTextFormat>
#include <QTextCharFormat>

#include <QDebug>

#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QModelIndexList>

/*!
  Generador de un reporte o informe

  @brief

  El generador de informes genera el informe en base a los
  datos suministrados, devolviendo un QTextDocument valido

  @note Para poder usar reflexión y emplear un BReport dentro de un BFactoryReport,
  es necesario que el constructor se declare usando la macro Q_INVOKABLE, para
  poder llamar al constructor desde la factoria.

  @note Por definicion, el primer valor de la lista de datos se refiere al directorio
  definido por defecto de salida del reporte en caso de querer guardarlo
  */

/*!
  @enum OpcionesTexto
  Opciones para presentar texto la fuente del texto.
  Ahora mismo, se reconocen las siguientes:
  - @b Normal: El texto sin cambios
  - @b Negrita: El texto aparece en negrita
  - @b Subrayado: El texto aparece subrayado
  - @b Italica: El texto aparece en cursiva.
  */


class BREPORTSHARED_EXPORT BReport : public QObject
{
    Q_OBJECT
    Q_FLAGS(OpTexto OpcionesTexto)
public:
    explicit BReport(QObject *parent = 0,QString nombreBase=QString());


    enum OpTexto {
        Normal=0x0,
        Negrita=0x1,
        Subrayado=0x2,
        Italica=0x4
    };
    Q_DECLARE_FLAGS(OpcionesTexto,OpTexto)

    /*!
      Establece los datos necesarios para generar el reporte
      @brief
      Por definicion, el primer dato recibido es el directorio
      de salida por defecto para el reporte, el cual
      sera ignorado en caso de emplear la opcion @ref ShowOnDialog
      @param datosVariant   Lista de datos en formato QVariant
      @note Modifica el objeto
      */
    void setData(QVariantMap datosVariant);

    /*!
      Devuelve los datos almacenados actualmente para generar el reporte
      @return QVariantMap, lista de datos en formato QVariant
      @note No modifica el objeto
      */
    QVariantMap getData() const;

    /*!
      Genera el informe con los datos actuales
      @return QTextDocument*, reporte generado
      @note Modifica el objeto
      */
    virtual QTextDocument *generateReport()=0;

    /*!
      Devuelve el nombre del reporte
      @brief
      Esta opcion es util para la generacion de reportes
      de forma automatizada (sin interaccion con el usuario)
      @return QString, nombre del reporte
      @note No modifica el objeto
      */
    virtual QString getNombreReport() const=0;


    /*!
      Añade el directorio de salida por defecto de los datos al nombre del reporte
      @note No modifica el objeto
      */

    QString nombreCompleto() const;

    /*!
      Devuelve el ultimo informe generado
      @return QTextDocument*, ultimo reporte generado
      @note No modifica el objeto
      */

    QTextDocument *lastReport();

signals:

public slots:

protected:

    QVariantMap datos;     ///< Datos necesarios para generar el informe, cambiaran para cada reporte
    QTextDocument *documento;   ///< Documento de base para generar el reporte
    QTextCursor *cursor;        ///< Cursor de texto
    QString nombreInforme;      ///< Nombre base del informe

    /*!
      Inicializa el objeto para generar el reporte
      @note Modifica el objeto
      */
    void initReport();

    /*!
      Introduce una tabla en un documento a partir de un modelo de datos
      @param tabla  Modelo de datos
      @param doc    Documento del reporte donde queremos insertar la tabla
      @param cursor Cursor que indica la posicion dentro del documento
      @param formatoCab Formato de texto para la cabecera de la tabla
      @return Tabla añadida para modificarla si fuera necesario
      @note Modifica el documento y el cursor
      */
    static QTextTable *addTabla(QAbstractTableModel *tabla,QTextDocument *doc,QTextCursor *cursor,QTextCharFormat formatCab);

    /*!
      @overload
      */
    static QTextTable *addTabla(QStandardItemModel *modelo,QTextDocument *doc,QTextCursor *cursor,QTextCharFormat formatCab);

    /*!
      Añade una linea resaltada al documento, del tipo negrita: texto
      @param doc    Documento del reporte donde queremos insertar la linea resaltada
      @param cursor Cursor que indica la posicion dentro del documento
      @param titulo Titulo de la linea resaltada
      @param opTitulo   Opciones de texto para el titulo
      @param texto  Texto de la linea resaltada
      @param opTexto Opciones de texto para el texto
      @note Modifica el documento y el cursor
      */
    static void addLineaResaltada(QTextDocument *doc,QTextCursor *cursor,QString titulo,OpcionesTexto opTitulo,QString texto,OpcionesTexto opTexto);

    /*!
      Inserta un titulo en el documento, en negrita y aumentando la fuente
      @param doc    Documento del reporte donde queremos insertar el titulo
      @param cursor Cursor que indica la posicion dentro del documento
      @param titulo Texto del titulo a insertar
      @param fontSize   Tamanyo de fuente del titulo
      @note Modifica el documento y el cursor
      */
    static void addTitulo(QTextDocument *doc,QTextCursor *cursor,QString titulo,int fontSize);

    /*!
      Inserta una linea en el documento
      @param doc    Documento del reporte donde queremos insertar el titulo
      @param cursor Cursor que indica la posicion dentro del documento
      @param linea  Linea de texto a insertar
      @param opciones   Opciones de texto de la linea a insertar
      @note Modifica el documento y el cursor
      @note Despues de realizar la insercion, se recupera el formato de texto original.
      @note Si pasamos la opcion de texto @ref Normal, el resultado es el mismo de la
      version sin argumento de opciones
      */

    static void addLinea(QTextDocument *doc,QTextCursor *cursor,QString linea,OpcionesTexto opciones);

    /*!
      @overload
      */

    static void addLinea(QTextDocument *doc, QTextCursor *cursor, QString linea);

    /*!
      Cambia el formato del texto dependiendo de las opciones suministradas
      @param opciones
      @return Formato original para recuperarlo posteriormente
      @note Modifica el cursor
      */

    static QTextCharFormat cambiarTipoChar(QTextCursor *cursor,OpcionesTexto op);

    /*!
      Cambia el formato del bloque dependiendo de las opciones suministradas
      @param opciones
      @return Formato original para recuperarlo posteriormente
      @note Modifica el cursor
      */

    static QTextCharFormat cambiarTipoBloque(QTextCursor *cursor,OpcionesTexto op);

    /*!
      Adapta una cadena de texto a un tamaño maximo
       @param texto Cadena a adaptar
       @param numCaracteres Numero de caracteres maximos que puede tener la cadena
       @return Cadena de texto adaptada
     */

    static QString elideText(QString texto,int numCaracteres);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BReport::OpcionesTexto)

#endif // BREPORT_H
