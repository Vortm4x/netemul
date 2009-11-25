#ifndef CABLETEXTITEM_H
#define CABLETEXTITEM_H

#include <QGraphicsTextItem>

class cableTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 4 };
    int type() const { return Type; }
    cableTextItem(QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0);

    ~cableTextItem() { }

    void updatePosition();
protected:
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);

public:
    bool isStart() const { return myStart; }
    void setStart(bool b) { myStart = b; }
private:
    bool myStart;
};

#endif // CABLETEXTITEM_H
