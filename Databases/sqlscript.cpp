#include "sqlscript.h"

SQLScript::SQLScript(QString path)
{
    this->path=path;
}

SQLScript::~SQLScript()
{
    path.clear();
}

void SQLScript::openFile()
{
    archivo.setFileName(path);
    archivo.open(QFile::Text | QFile::ReadOnly);
}

QString SQLScript::readFile()
{
    QString res=QString(archivo.readAll());

    return res;
}

QStringList SQLScript::getScriptProcesado()
{
    if(path.isEmpty() || path.isNull())
        return QStringList();

    openFile();

    if(!archivo.isOpen())
        return QStringList();

    QString raw=readFile();

    QStringList res=raw.split(QString("-- END"),QString::SkipEmptyParts);
    res.removeLast();

    return res;
}
