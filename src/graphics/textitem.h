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
#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsTextItem>
#include <QtScript>
#include <QGraphicsScene>
/*!
  Реализует текстовую надпись на сцене.
*/
class TextItem : public QGraphicsTextItem
{
    Q_OBJECT
    Q_PROPERTY( QString note READ toPlainText WRITE setNote )
public:
    enum { Type = UserType + 3 };
    int type() const { return Type; }
    void setNote(const QString &str) { setPlainText(str); adjustSize(); }        
    TextItem(QPointF p, QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0);
    TextItem(QObject *parent = 0 );
signals:
    void lostFocus(TextItem*);
protected:
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
private:
    void init();
};
//-------------------------------------------------------------


Q_DECLARE_METATYPE(TextItem*)
#endif // TEXTITEM_H
