/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#include "macaddressdelegate.h"
#include <QLineEdit>

macAddressDelegate::macAddressDelegate(QObject *parent /* = 0 */) : QItemDelegate(parent)
{

}

QWidget* macAddressDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit *line = new QLineEdit(parent);
    line->setInputMask("HH:HH:HH:HH:HH:HH;_");
    return line;
}

void macAddressDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString data = index.model()->data(index,Qt::EditRole ).toString();
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    line->setText(data);
}

void macAddressDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    QString data = line->text();
    model->setData(index,data,Qt::EditRole);
}

void macAddressDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex&) const
{
    editor->setGeometry( option.rect );
}

