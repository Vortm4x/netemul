#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsTextItem>
#include <QtScript>

/*!
  Реализует текстовую надпись на сцене.
*/
class textItem : public QGraphicsTextItem
{
    Q_OBJECT
    Q_PROPERTY(QString note READ toPlainText() WRITE setNote)
public:
    enum { Type = UserType + 3 };
    int type() const { return Type; }
    void setNote(const QString &str) { setPlainText(str); adjustSize(); }
    textItem(QPointF p, QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0);
signals:
    void lostFocus(textItem*);
protected:
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
};
//-------------------------------------------------------------


Q_DECLARE_METATYPE(textItem*)
#endif // TEXTITEM_H
