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
#include "textitem.h"
#include <QPainter>
#include <qgraphicsscene.h>

/*!
  Передает параметры предку и делает надпись перемещаемой и выделяемой.
*/
TextItem::TextItem(QPointF p, QGraphicsItem *parent /* = 0 */ , QGraphicsScene *scene /* = 0 */)
        : QGraphicsTextItem(parent)
{
    setPos(p);
    if (scene)
    {
        scene->addItem(this);  // Add to the scene if the pointer is valid
    }
    init();
}

TextItem::TextItem(QObject *parent)
{
    init();
}

void TextItem::init()
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setZValue(1000.0);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    QFont s("Times", 12 , QFont::Bold);
    setFont(s);
}

//-------------------------------------------------------------------------
/*!
  Происходит при потере элементом фокуса.
*/
void TextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction); // Запрещаем редактировать
    emit lostFocus(this); // Сообщаем сцене
    QGraphicsTextItem::focusOutEvent(event); // Передаем событие предку.
}
//---------------------------------------------------------------------------
/*!
  Происходит при двойном щелчке по элементу.
*/
void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event); // Передаем предку.
}
//----------------------------------------------------------------------------
/*!

*/
void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->setBrush(Qt::yellow);
    painter->drawRoundedRect(boundingRect(), 5, 5);
    QGraphicsTextItem::paint(painter,option,widget);
}
//----------------------------------------------------------------------------
