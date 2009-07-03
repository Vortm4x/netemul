#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include "device.h"
#include "interface.h"
#include <QGraphicsScene>

struct routeRecord {
    Q_DECLARE_TR_FUNCTIONS(routeRecord)
public:
    ipAddress dest;
    ipAddress mask;
    ipAddress gateway;
    interface *out;
    int time;
    qint8 metric;
    int mode;
    QString modeString() const;
    friend QDataStream& operator<<(QDataStream &stream, const routeRecord &rec);
    friend QDataStream& operator>>(QDataStream &stream, routeRecord &rec);
};

class smartDevice : public device
{
public:
    enum {  connectMode = 3 , staticMode = 4 , ripMode = 5 };
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
    QList<routeRecord*>& routeTable() { return myRouteTable; }
    routeRecord* addToTable(ipAddress dest,ipAddress mask,ipAddress gateway,ipAddress out,
                            int time,qint8 metr = 0 ,int mode = staticMode);
    void deleteFromTable(int n);
    void deleteFromTable(routeRecord *r);
    void addConnection(cableDev *cable);
    void deleteConnection(cableDev *cable);
    void setGateway(const QString str);
    void setRouteMode(bool n) { myRouteMode = n; }
    bool routeMode() const { return myRouteMode; }
    ipAddress gateway() const;
    friend class ripProgramm;
protected:
    bool myRouteMode;
    QList<routeRecord*> myRouteTable;
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

#endif // SMARTDEVICE_H
