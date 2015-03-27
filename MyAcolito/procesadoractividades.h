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

#ifndef PROCESADORACTIVIDADES_H
#define PROCESADORACTIVIDADES_H

#include <QDialog>

#include <Databases/mydatabase.h>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDate>
#include <QTime>
#include <QInputDialog>
#include <QTextStream>

namespace Ui {
class ProcesadorActividades;
}

class ProcesadorActividades : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProcesadorActividades(QWidget *parent = 0,AcolitoDatabase *db=0);
    ~ProcesadorActividades();
    
private:
    Ui::ProcesadorActividades *ui;

    AcolitoDatabase *database;

private slots:

    void empezarProcesado();
    void seleccionarDirectorio();
};

#endif // PROCESADORACTIVIDADES_H
