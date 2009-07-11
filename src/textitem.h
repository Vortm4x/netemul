#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsTextItem>

/*!
  Реализует текстовую надпись на сцене.
*/
class textItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 9 };
    int type() const { return Type; }
    textItem(QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0);
signals:
    void lostFocus(textItem*);
protected:
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
};
//-------------------------------------------------------------

#endif // TEXTITEM_H
