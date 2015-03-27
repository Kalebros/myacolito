/***********************************************************************
** SManager, copyright @2013,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** This file is part of SManager.
**
** SManager is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SManager is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with SManager.  If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "smanager.h"

SManager::SManager(QObject *parent) :
    QObject(parent),
    _isActive(false),
    documento(0),
    procesoActivo(0)
{
}

bool SManager::isActive() const
{
    return _isActive;
}

void SManager::execScript(SMScript *script)
{
    if(_isActive)
        return;

    if(!script)
        return;

    scriptActivo=script;

    QString path=scriptActivo->getPath();
    if(path.isEmpty() || path.isNull()) {
        emit error("El path del script está vacío.");
        return;
    }

    QFileInfo archivoEnPath(path);

    if(!archivoEnPath.exists()) {
        emit error("El archivo especificado no existe o no es alcanzable.");
        return;
    }

    if(documento) {
        delete documento;
        documento=0;
    }

    if(procesoActivo) {
        disconnect(procesoActivo);
        delete procesoActivo;
        procesoActivo=0;
    }

    procesoActivo=new QProcess(this);

    connect(procesoActivo,SIGNAL(started()),this,SLOT(procesoIniciado()));
    connect(procesoActivo,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procesoTerminado(int,QProcess::ExitStatus)));
    connect(procesoActivo,SIGNAL(error(QProcess::ProcessError)),this,SLOT(errorProceso(QProcess::ProcessError)));


    //Procesar cadena de argumentos
    /*
    QString arg=script->getArgumentos();

    //Sustituir caracteres
    arg=arg.replace('/',"\\");
    arg=arg.replace('\\',QString("\\"));

    qDebug() << arg;

    //Procesar cadena de path
    QString path=script->getPath();
    path=path.replace('/',"\\");
    path=path.replace('\\',QString("\\"));

    qDebug() << path;
    */

    QStringList argumentos=scriptActivo->getArgumentos().split(" ",QString::SkipEmptyParts);

    procesoActivo->start(path,argumentos);
}

void SManager::procesoIniciado()
{
    _isActive=true;
    _isCanceled=false;
    emit this->initScript(scriptActivo);
}

void SManager::errorProceso(QProcess::ProcessError error)
{
    if(error==QProcess::FailedToStart || error==QProcess::Crashed) {
        emit this->error("No se pudo iniciar el script o se ha estrellado.");
        return;
    }
    if(error==QProcess::Timedout) {
        emit this->error("El proceso no responde.");
        return;
    }
}

void SManager::cancelScript()
{
    if(_isActive && procesoActivo) {
        if(procesoActivo->state()==QProcess::Running) {
            procesoActivo->kill();
            procesoActivo->waitForFinished();
            delete procesoActivo;
            procesoActivo=0;
            _isActive=false;
            _isCanceled=true;
        }
    }
}

void SManager::cambioEstado(QProcess::ProcessState estado)
{

}

void SManager::procesoTerminado(int code, QProcess::ExitStatus status)
{
    if(status==QProcess::CrashExit) {
        emit error("El script terminó con un error o no pudo terminar.");
        return;
    }

    if(!_isCanceled)
        documento=new QJsonDocument(QJsonDocument::fromJson(procesoActivo->readAllStandardOutput()));
    else documento=0;

    delete procesoActivo;
    procesoActivo=0;
    _isActive=false;
    emit finishedScript(scriptActivo);
}
