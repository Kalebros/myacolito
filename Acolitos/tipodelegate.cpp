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

#include "tipodelegate.h"

TipoDelegate::TipoDelegate(QObject *parent, AcolitoDatabase *db) :
    QStyledItemDelegate(parent),
    database(db)
{
}

QWidget *TipoDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor=new QComboBox(parent);

    editor->setModel(database->getModeloTiposActividad());
    editor->setModelColumn(0);

    return editor;
}

void TipoDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);

    QComboBox *combo=qobject_cast<QComboBox*>(editor);
    combo->showPopup();
}

void TipoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString tipo=index.data().toString();

    QComboBox *comboBox=qobject_cast<QComboBox*>(editor);

    comboBox->setCurrentIndex(comboBox->findText(tipo));
}

void TipoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox=qobject_cast<QComboBox*>(editor);
    QString categoria=comboBox->currentText();

    model->setData(index,categoria,Qt::EditRole);
}

void TipoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionComboBox estiloComboBox;

    estiloComboBox.currentText=index.data().toString();
    estiloComboBox.rect=option.rect;
    estiloComboBox.state=option.state;

    QApplication::style()->drawComplexControl(QStyle::CC_ComboBox,&estiloComboBox,painter);
    QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel,&estiloComboBox,painter);
}
