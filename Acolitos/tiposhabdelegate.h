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

#ifndef TIPOSHABDELEGATE_H
#define TIPOSHABDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
#include <QComboBox>
#include <QApplication>
#include <QStyle>

#include <Databases/acolitodatabase.h>

class TiposHabDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TiposHabDelegate(QObject *parent = 0,AcolitoDatabase *db=0);
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    
public slots:

private:

    AcolitoDatabase *database;   ///< Database empleada
};

#endif // TIPOSHABDELEGATE_H
