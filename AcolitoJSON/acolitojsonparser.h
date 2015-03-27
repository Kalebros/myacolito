#ifndef ACOLITOJSONPARSER_H
#define ACOLITOJSONPARSER_H

#include "AcolitoJSON_global.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <QLocale>

#include <Databases/acolitodatabase.h>
#include <Databases/mymodeloasistente.h>
#include <Databases/mymodeloactividades.h>

/*!
    Parsea un documento JSON para obtener informacion de actividades,
    asistentes y habitaciones.

    En principio, el parser solo funciona para introducir nuevos datos
    en la database, no para eliminar ni realizar operaciones complejas
    y/o completas; dichas operaciones deberan realizarse a mano.

    @b FORMATO:

    El formato del documento JSON deberá de ser el siguiente:

    - Un objeto con los siguientes atributos: 'Actividades','Asistentes','Habitaciones',
    'Categorias','TiposActividades','TiposHabitaciones','Localizaciones'
    - 'Actividades': Objeto con dos atributos, 'Nuevas' (array de datos de actividades), 'Errores' (array de textos)
    - 'Asistentes': Igual que actividades
    - 'Habitaciones': Igual que asistentes y actividades
    - 'Categorias': Igual
    - 'TiposActividades': Igual
    - 'TiposHabitaciones': Igual
    - 'Localizaciones': Igual

    Por ahora sólo soporta la parte de asistentes.

    - @b Asistentes: Los asistentes tienen el siguiente formato:

    @code
    {"Asistentes":
            {"nuevas": [
                    [ {
                        "categoria": <tipo de categoria>,
                        "dni": <dni del asistente>,
                        "mail": <correo electronico>,
                        "nombre": <nombre del asistente>,
                        "telefono": <numero o numeros de telefono>,
                        "comentarios": <otros datos de interes>,
                        "haPagado": <bool de pago>
                        },
                      {
                        "categoria": <tipo de categoria>,
                        "dni": <dni del asistente>,
                        "mail": <correo electronico>,
                        "nombre": <nombre del asistente>,
                        "telefono": <numero o numeros de telefono>,
                        "comentarios": <otros datos de interes>,
                        "haPagado": <bool de pago>
                        } ],
                        [ {
                        <...>
                        } ]
                     ]
              }
    }
    @endcode
 */

class ACOLITOJSONSHARED_EXPORT AcolitoJSONParser : public QObject
{
    Q_OBJECT
public:
    explicit AcolitoJSONParser(QObject *parent = 0,AcolitoDatabase *database=0);

    /*!
      Parsea un documento JSON
      */

    bool parseJSONDocument(QJsonDocument *doc);


    
signals:

private:

    AcolitoDatabase *db;        ///< Database del evento

    /*!
      Parsea un objeto Asistentes
      */

    bool parseJSONAsistente(QJsonObject listaAsis);

    /*!
      Parsea un objeto Actividades
      @todo Implementar
      */

    bool parseJSONActividades(QJsonObject actividades);
};

#endif // ACOLITOJSONPARSER_H
