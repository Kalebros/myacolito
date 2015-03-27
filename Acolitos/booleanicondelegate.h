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

#ifndef BOOLEANICONDELEGATE_H
#define BOOLEANICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionComboBox>
#include <QApplication>
#include <QVariant>
#include <QWidget>
#include <QSize>
#include <QPainter>
#include <QComboBox>
#include <QRect>
#include <QPixmap>
#include <QIcon>

class BooleanIconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BooleanIconDelegate(QObject *parent = 0,QIcon trueIcon=QIcon(),QIcon falseIcon=QIcon());

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    
public slots:

private:

    QIcon iconoTrue;
    QIcon iconoFalse;
};

#endif // BOOLEANICONDELEGATE_H
