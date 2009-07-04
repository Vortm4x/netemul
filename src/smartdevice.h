#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include "device.h"
#include "interface.h"
#include "routeeditor.h"
#include "adapterproperty.h"
#include "tablearp.h"
#include "programmdialog.h"
#include <QGraphicsScene>

class programm;

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
    void sendMessage(ipAddress dest, int size , int pr);
    void receivePacket(ipPacket *p,interface *f);
    void treatPacket(ipPacket *p);
    void routePacket(ipPacket *p);
    void connectedNet(devicePort *p);
    ipAddress findInterfaceIp(ipAddress a);
    routeRecord* recordAt(const ipAddress a) const;
    programm* programmAt(const quint16 p) const;
    programm* programmAt(const QString n) const;
    void removeProgramm(programm *p);
    void installProgramm( programm *p) { myProgramms << p; }
    void updateArp(int u);
    QList<routeRecord*>& routeTable() { return myRouteTable; }
    QList<programm*>& programms() { return myProgramms; }
    routeRecord* addToTable(ipAddress dest,ipAddress mask,ipAddress gateway,ipAddress out,
                            int time,qint8 metr = 0 ,int mode = staticMode);
    routeRecord* addToTable(routeRecord *r);
    void deleteFromTable(int n);
    void deleteFromTable(routeRecord *r);
    void addConnection(cableDev *cable);
    void deleteConnection(cableDev *cable);
    void setGateway(const QString str);
    void setRouteMode(bool n) { myRouteMode = n; }
    bool routeMode() const { return myRouteMode; }
    void incTime();
    ipAddress gateway() const;
    template<class T>
    void showDialog() {
        T *d = new T();
        d->setDevice( this );
        d->exec();
        delete d;
    }
    friend class ripProgramm;
protected:
    bool myRouteMode;
    QList<programm*> myProgramms; //!< Программы установленные на устройстве.
    QList<routeRecord*> myRouteTable; //!< Таблица маршрутизации.
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

#endif // SMARTDEVICE_H
