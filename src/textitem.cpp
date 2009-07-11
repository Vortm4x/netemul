#include "textitem.h"
#include <QPainter>

/*!
  Передает параметры предку и делает надпись перемещаемой и выделяемой.
*/
textItem::textItem(QGraphicsItem *parent /* = 0 */ , QGraphicsScene *scene /* = 0 */)
        : QGraphicsTextItem(parent,scene)
{
     setFlag(QGraphicsItem::ItemIsMovable);
     setFlag(QGraphicsItem::ItemIsSelectable);
     setPlainText(trUtf8("Комментарий"));
     QFont s("Times", 12 , QFont::Bold);
     setFont(s);
}
//-------------------------------------------------------------------------
/*!
  Происходит при потере элементом фокуса.
*/
void textItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction); // Запрещаем редактировать
    emit lostFocus(this); // Сообщаем сцене
    QGraphicsTextItem::focusOutEvent(event); // Передаем событие предку.
}
//---------------------------------------------------------------------------
/*!
  Происходит при двойном щелчке по элементу.
*/
void textItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event); // Передаем предку.
}
//----------------------------------------------------------------------------
/*!

*/
void textItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->setBrush(Qt::yellow);
    painter->drawRoundedRect(boundingRect(), 5, 5);
    QGraphicsTextItem::paint(painter,option,widget);
}
//----------------------------------------------------------------------------
