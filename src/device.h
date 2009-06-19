#ifndef DEVICE_H
#define DEVICE_H

#include <QGraphicsItem>
#include <QDataStream>
#include "deviceport.h"

class cableDev;
class QMenu;


class device : public QGraphicsItem
{
public:
    enum sizeDevices { rectDevX = -20 , rectDevY = -20 , rectDevWidth = 40 , rectDevHeight = 40 };
    device();
    ~device();
    QRectF boundingRect() const {
        return QRectF(rectDevX,rectDevY,rectDevWidth,rectDevHeight);
    }
    void setMenu(QMenu *context) { popUpMenu = context; }
    bool isConnect() { return cableList.count(); }
    virtual void addConnection(cableDev *cable) { cableList << cable;}
    virtual void deleteConnection(cableDev *cable) { cableList.removeOne(cable); }
    void addSocket( devicePort* inter) { mySockets << inter; }
    void removeSocket( devicePort* inter) { mySockets.removeOne(inter); delete inter; }
    QList<cableDev*> cables() const { return cableList; }
    QList<devicePort*> sockets() const { return mySockets; }
    QString nextName() { return QString("eth%1").arg(count++); }
    devicePort* socket(const QString name);
    template<class T>T* toT() { return qgraphicsitem_cast<T*>(this); }
    virtual QPointF getPointCable(QPointF otherDev) = 0;
    virtual devicePort* addInterface(QString str,int t) = 0;
    virtual void write(QDataStream &stream) const = 0;
    virtual void read(QDataStream &stream) = 0 ;
    virtual void dialog() = 0;
    virtual QString hasTable() const = 0;
private:
    QMenu *popUpMenu; // попАпМеню для устройства
protected:
    QList<devicePort*> mySockets;
    QList<cableDev*> cableList;
    friend QDataStream& operator<<(QDataStream &stream,const device &dev);
    friend QDataStream& operator>>(QDataStream &stream,device &dev);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event); // Событие контекстного меню
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    int count;
};

inline QDataStream& operator<<(QDataStream &stream,const device &dev)
{
    dev.write(stream);
    return stream;
}


inline QDataStream& operator>>(QDataStream &stream,device &dev)
{
    dev.read(stream);
    return stream;
}

#endif // DEVICE_H
