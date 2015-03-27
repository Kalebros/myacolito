#include "actividadespordias.h"

ActividadesPorDias::ActividadesPorDias(QObject *parent) :
    BReport(parent,"reporteDias")
{
    _fTexto=QFont("Verdana",12);
    _fCabecera=QFont("Verdana",14);
}

QString ActividadesPorDias::getNombreReport() const
{
    return this->nombreCompleto();
}


QTextDocument *ActividadesPorDias::generateReport()
{
    initReport();

    QTextCharFormat formatoTexto;
    formatoTexto.setFont(_fTexto);
    cursor->setCharFormat(formatoTexto);

    QList<QVariant> listaDias=datos.value("listado").toList();
    foreach(QVariant aux,listaDias) {
        QVariantMap datosDia=aux.toMap();
        QDate fecha=datosDia.value("fecha").toDate();

        QString fString=fecha.toString("dddd, dd 'de' ");
        QString mString=fecha.toString("MMMM");
        fString.replace(0,1,fString.at(0).toUpper());
        mString.replace(0,1,mString.at(0).toUpper());
        fString.append(mString);

        formatoTexto.setFont(_fTexto);
        cursor->setCharFormat(formatoTexto);
        BReport::addTitulo(documento,cursor,fString,14);

        BReport::addLineaResaltada(documento,cursor,tr("Numero de actividades"),Negrita | Subrayado,
                                   datosDia.value("numActividades").toString(),Italica);

        cursor->insertBlock();

        QTextTable *tablaActividad=crearTablaActividades(datosDia.value("numActividades").toInt());

        QList<QVariant> listaActividades=datosDia.value("actividades").toList();

        formatoTexto.setFont(_fTexto);
        foreach(QVariant aux,listaActividades) {
            QVariantMap actividad=aux.toMap();
            cursor->insertText(actividad.value("inicio").toTime().toString("hh:mm"),
                               formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(BReport::elideText(actividad.value("nombre").toString(),
                                         100),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(actividad.value("categoria").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
            cursor->insertText(actividad.value("lugar").toString(),formatoTexto);
            cursor->movePosition(QTextCursor::NextCell);
        }

        cursor->movePosition(QTextCursor::End);
        cursor->insertBlock();
        cursor->insertBlock();
        cursor->insertBlock();
    }

    return documento;
}

QTextTable *ActividadesPorDias::crearTablaActividades(int numActividades)
{
    QTextTable *tabla=cursor->insertTable(numActividades+1,4);

    QTextTableFormat formatoTabla;
    formatoTabla=tabla->format();
    formatoTabla.clearColumnWidthConstraints();
    formatoTabla.setWidth(QTextLength(QTextLength::PercentageLength,100));
    tabla->setFormat(formatoTabla);

    QTextCharFormat formatoTexto;
    formatoTexto.setFont(_fCabecera);
    cursor->insertText(tr("Inicio"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->insertText(tr("Actividad"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->insertText(tr("Tipo"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);
    cursor->insertText(tr("Lugar"),formatoTexto);
    cursor->movePosition(QTextCursor::NextCell);

    return tabla;
}
