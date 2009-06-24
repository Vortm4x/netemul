#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include "device.h"
#include "interface.h"
#include <QGraphicsScene>

struct routeRecord {
    ipAddress dest;
    ipAddress mask;
    ipAddress gateway;
    interface *out;
    int time;
    int metric;
    int mode;
    QString modeString() const;
    friend QDataStream& operator<<(QDataStream &stream, const routeRecord &rec);
    friend QDataStream& operator>>(QDataStream &stream, routeRecord &rec);
};

class smartDevice : public device
{
public:
    enum {  connectMode = 3 , staticMode = 4 , ripMode = 5 };
    enum { noRoute = 0 , staticRoute = 1 , ripRoute = 2 };
    enum { UDP = 25 ,TCP = 26 };
    smartDevice();
    ~smartDevice();
    interface* adapter(QString s);
    interface* ipToAdapter(ipAddress a);
    void editorShow();
    void adapterShow();
    void arpShow();
    void sendMessage(ipAddress dest, int size , int pr);
    void receivePacket(ipPacket *p,interface *f);
    virtual void treatPacket(ipPacket *p) { Q_UNUSED(p) }
    void routePacket(ipPacket *p);
    void connectedNet(devicePort *p);
    void updateArp(int u);
    void sendRip(int u);
    QList<routeRecord*> routeTable() { return myRouteTable; }
    routeRecord* addToTable(ipAddress dest,ipAddress mask,ipAddress gateway,ipAddress out,
                            int time,int metr = 0 ,int mode = staticMode);
    void deleteFromTable(int n);
    void deleteFromTable(routeRecord *r);
    void addConnection(cableDev *cable);
    void deleteConnection(cableDev *cable);
    void setGateway(const QString str);
    void setRouteMode(int n) { myRouteMode = n; }
    int routeMode() const { return myRouteMode; }
    ipAddress gateway() const;
protected:
    int time;
    int myRouteMode;
    QList<routeRecord*> myRouteTable;
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

#endif // SMARTDEVICE_H
