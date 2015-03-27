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

#include "booleanicondelegate.h"

BooleanIconDelegate::BooleanIconDelegate(QObject *parent, QIcon trueIcon, QIcon falseIcon) :
    QStyledItemDelegate(parent),
    iconoTrue(trueIcon),
    iconoFalse(falseIcon)
{

}

QWidget *BooleanIconDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor=new QComboBox(parent);

    editor->addItem(iconoTrue,tr("Sí"),QVariant(true));
    editor->addItem(iconoFalse,tr("No"),QVariant(false));

    return editor;
}

void BooleanIconDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox=qobject_cast<QComboBox*>(editor);

    if(index.data(Qt::EditRole).toBool())
        comboBox->setCurrentIndex(0);
    else comboBox->setCurrentIndex(1);
}

void BooleanIconDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox=qobject_cast<QComboBox*>(editor);

    QVariant estado=comboBox->itemData(comboBox->currentIndex());

    model->setData(index,estado);
}


void BooleanIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionComboBox estiloComboBox;
    QIcon iconoVista;

    if(index.data().toBool()) {
        estiloComboBox.currentText=QString::fromLatin1("Sí"); //Este archivo esta codificado en latin 1
        iconoVista=QIcon(iconoTrue);
    }
    else {
        estiloComboBox.currentText=tr("No");
        iconoVista=QIcon(iconoFalse);
    }
    estiloComboBox.state=option.state;
    estiloComboBox.rect=option.rect;
    estiloComboBox.currentIcon=iconoVista;
    estiloComboBox.iconSize=option.decorationSize;

    const QWidget *widget=qobject_cast<const QWidget*>(this);
    QApplication::style()->drawComplexControl(QStyle::CC_ComboBox,&estiloComboBox,painter,widget);
    QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel,&estiloComboBox,painter,widget);
}

void BooleanIconDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    QComboBox *combo=qobject_cast<QComboBox*>(editor);
    combo->showPopup();
}
