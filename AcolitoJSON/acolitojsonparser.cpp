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

#include "acolitojsonparser.h"

AcolitoJSONParser::AcolitoJSONParser(QObject *parent, AcolitoDatabase *database) :
    QObject(parent),
    db(database)
{
}

bool AcolitoJSONParser::parseJSONDocument(QJsonDocument *doc)
{
    if(!doc)
        return false;

    if(!doc->isObject())
        return false;

    QJsonObject objetoPadre=doc->object();
    bool res=false;

    if(objetoPadre.contains("Asistentes")) {
        res=parseJSONAsistente(objetoPadre.value("Asistentes").toObject());
    }

    if(objetoPadre.contains("Actividades")) {
        bool aux=parseJSONActividades(objetoPadre.value("Actividades").toObject());
        if(!aux)
            res=false;
    }

    return res;
}

bool AcolitoJSONParser::parseJSONActividades(QJsonObject actividades)
{
    if(!actividades.contains("nuevas")) {
        qDebug() << "ERROR: Faltan campos en el objeto JSON Actividades: " << actividades.keys();
        return false;
    }

    if(!actividades.value("nuevas").isArray())
        return false;

    QList<QVariantMap> listadoParseado;
    QJsonArray arrayActividades=actividades.value("nuevas").toArray();

    for(QJsonArray::Iterator it=arrayActividades.begin();it!=arrayActividades.end();it++) {
        QJsonObject jsonActividad=(*it).toObject();
        QVariantMap actividad;
        actividad.insert("nombre",jsonActividad.value("nombre").toString());
        actividad.insert("asociacion",jsonActividad.value("asociacion").toString());
        actividad.insert("responsable",jsonActividad.value("responsable").toString());
        actividad.insert("correo",jsonActividad.value("correo").toString());
        actividad.insert("requisitos",jsonActividad.value("requisitos").toString());
        actividad.insert("coorganizadores",jsonActividad.value("coorganizadores").toString());
        actividad.insert("mPropio",jsonActividad.value("mpropio").toString());
        actividad.insert("mOrganizacion",jsonActividad.value("morg").toString());
        actividad.insert("tipo",jsonActividad.value("tipoActividad").toString());
        actividad.insert("dia",jsonActividad.value("fecha").toVariant());
        actividad.insert("espacio",jsonActividad.value("espacio").toString());
        actividad.insert("horaInicio",jsonActividad.value("inicio").toVariant());
        actividad.insert("horaFin",jsonActividad.value("fin").toVariant());
        actividad.insert("todaJornada",false);
        actividad.insert("observaciones",jsonActividad.value("observaciones").toString());
        actividad.insert("maximoPart",jsonActividad.value("maxParticipantes").toVariant());
        actividad.insert("minimoPart",jsonActividad.value("minParticipantes").toVariant());
        actividad.insert("descripcion",jsonActividad.value("descripcion").toString());
        actividad.insert("lugar",jsonActividad.value("lugar").toString());

        listadoParseado << actividad;
    }

    db->addListaActividades(listadoParseado);
    //TODO
    return true;
}

bool AcolitoJSONParser::parseJSONAsistente(QJsonObject listaAsis)
{
    if(!listaAsis.contains("nuevas")) {
        qDebug() << "ERROR: Faltan campos en el objeto JSON Asistentes: " << listaAsis.keys();
        return false;
    }

    if(!listaAsis.value("nuevas").isArray())
        return false;

    QList<Asistente*> grupoAsistente;
    Asistente *asis;

    QJsonArray arrayAsistentes=listaAsis.value("nuevas").toArray();

    for(QJsonArray::iterator it=arrayAsistentes.begin();it!=arrayAsistentes.end();it++) {
        QJsonArray grupo=(*it).toArray();
        for(QJsonArray::iterator itAux=grupo.begin();itAux!=grupo.end();itAux++) {
            QJsonObject jsonAsis=(*itAux).toObject();
            asis=new Asistente(this);
            asis->nombre=jsonAsis.value("nombre").toString();
            asis->DNI=jsonAsis.value("dni").toString();
            QString comentarios=jsonAsis.value("comentarios").toString();
            QString telefono=jsonAsis.value("telefono").toString();
            QString mail=jsonAsis.value("mail").toString();
            if(comentarios.isEmpty() || comentarios.isNull())
                asis->comentarios=QString(" ");
            else
                asis->comentarios=comentarios;
            if(telefono.isEmpty() || telefono.isNull())
                        asis->telefono=QString(" ");
            else asis->telefono=telefono;
            if(mail.isEmpty() || mail.isNull())
                asis->mail=QString(" ");
            else asis->mail=mail;
            asis->pago=jsonAsis.value("haPagado").toBool();
            asis->tipo=jsonAsis.value("categoria").toString();

            grupoAsistente.append(asis);
        }
        db->addGrupoAsistentes(grupoAsistente);
        grupoAsistente.clear();
    }

    return true;
}
