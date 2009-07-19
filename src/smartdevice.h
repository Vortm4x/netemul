#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include "interface.h"
#include "deviceimpl.h"
#include "programmdialog.h"
#include <QGraphicsScene>

class programm;

/*!
  Запись таблицы маршрутизации.
*/
struct routeRecord {
    Q_DECLARE_TR_FUNCTIONS(routeRecord)
public:
    ipAddress dest;
    ipAddress mask;
    ipAddress gateway;
    ipAddress out;
    int time;
    qint8 metric;
    int mode;
    quint8 change; //!< Флаг показывающий изменена запись или нет.
    QString modeString() const;
    friend QDataStream& operator<<(QDataStream &stream, const routeRecord &rec);
    friend QDataStream& operator>>(QDataStream &stream, routeRecord &rec);
};
//-----------------------------------------------------------------------
/*!
  Интелектуальное устройство, абстрактный класс объединяющий в себе
  свойства компьютера и роутера.
*/
class smartDevice : public deviceImpl
{
    Q_DECLARE_TR_FUNCTIONS(smartDevice)
public:
    /*! Источники записи таблицы маршрутизации. */
    enum { connectMode = 3 , staticMode = 4 , ripMode = 5 };
    enum { addNet = 100 , delNet = 101 };
    enum { RIP = 50 };
    enum { UDP = 25 ,TCP = 26 };
    /*! Значения для флага записи из таблицы маршрутизации. */
    enum { changed = 0 , noChanged = 1 };
    smartDevice() { myRouteMode = false; }
    virtual ~smartDevice();
    QString tableName() const { return trUtf8("Таблица маршрутизации"); }
    void tableDialog() const;
    void adapterDialog() const;
    void programmsDialog() const;
    bool isSmart() const { return true; }
    devicePort* socket(const QString &name) { return adapter(name)->socket(); }
    QStringList sockets() const;
    QStringList interfacesIp() const;
    bool isConnectSocket(const QString &socket) const { return adapter(socket)->isConnect(); }
    QString socketName(devicePort *p) const;

    interface* adapter(QString s) const;
    interface* ipToAdapter(const ipAddress a);
    void sendMessage(ipAddress dest, int size , int pr);
    void receivePacket(ipPacket *p,interface *f);
    void treatPacket(ipPacket *p);
    void routePacket(ipPacket *p);
    void connectedNet(interface *p);
    ipAddress findInterfaceIp(ipAddress a);
    routeRecord* recordAt(const ipAddress &a) const;
    routeRecord* recordAt(const interface *p);
    programm* programmAt(const quint16 p) const;
    programm* programmAt(const QString n) const;
    void removeProgramm(programm *p);
    void installProgramm( programm *p) { myProgramms << p; }
    void updateArp(int u);
    bool sendInterrupt(int u);
    int socketsCount() const { return myInterfaces.count(); }
    QList<routeRecord*>& routeTable() { return myRouteTable; }
    QList<programm*>& programms() { return myProgramms; }
    routeRecord* addToTable(ipAddress d,ipAddress m,ipAddress g,ipAddress o,qint8 metr,int mode);
    routeRecord* addToTable(routeRecord *r,bool tr = true);
    void deleteFromTable(int n);
    void deleteFromTable(routeRecord *r,bool tr = true);
    void addConnection(cableDev *cable);
    void deleteConnection(cableDev *cable);
    void setGateway(const QString str);
    void setRouteMode(bool n) { myRouteMode = n; }
    bool routeMode() const { return myRouteMode; }
    void incTime();
    ipAddress gateway() const;
    friend class ripProgramm;
    friend class adapterSetting;
protected:
    bool myRouteMode;
    QVector<interface*> myInterfaces;
    QList<programm*> myProgramms; //!< Программы установленные на устройстве.
    QList<routeRecord*> myRouteTable; //!< Таблица маршрутизации.
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};
//-------------------------------------------------------------------
inline bool operator<(const routeRecord &e1 , const routeRecord &e2)
{
    if ( e1.mask != e2.mask ) return e1.mask < e2.mask;
    return e1.dest < e2.dest;
}
inline bool operator>(const routeRecord &e1 , const routeRecord &e2)
{
    if ( e1.mask != e2.mask ) return e1.mask > e2.mask;
    return e1.dest < e2.dest;
}
inline bool routeGreat(const routeRecord *e1 , const routeRecord *e2) { return *e1 > *e2; }
inline QDataStream& operator<<(QDataStream &stream, const routeRecord &rec)
{
    stream << rec.dest;
    stream << rec.mask << rec.gateway;
    stream << rec.time << rec.metric << rec.out;
    return stream;
}
inline QDataStream& operator>>(QDataStream &stream, routeRecord &rec)
{
    stream >> rec.dest >> rec.mask >> rec.gateway >> rec.time >> rec.metric >> rec.out;
    return stream;
}
/*!
  Модель данных для настроек адаптеров.
*/
class adapterSetting {
    Q_DECLARE_TR_FUNCTIONS(adapterSetting)
public:
    adapterSetting(smartDevice *s) : sd(s) { }
    void setCurrent(int n) { cur = n; }
    void resetStatics() { sd->myInterfaces.at(cur)->resetStatics(); }
    void connectedNet() { sd->connectedNet(sd->myInterfaces.at(cur)); }
    int socketsCount() const { return sd->socketsCount(); }
    bool isConnect() const { return sd->myInterfaces.at(cur)->isConnect(); }
    QString name() const { return sd->myInterfaces.at(cur)->name(); }
    void setName(const QString &str) { sd->myInterfaces.at(cur)->setName(str); }
    QString mac() const { return sd->myInterfaces.at(cur)->mac().toString(); }
    QString ip() const { return sd->myInterfaces.at(cur)->ip().toString(); }
    QString mask() const { return sd->myInterfaces.at(cur)->mask().toString(); }
    void setMac(const QString &str) { sd->myInterfaces.at(cur)->setMac(str); }
    void setIp(const QString &str) { sd->myInterfaces.at(cur)->setIp(str); }
    void setMask(const QString &str) { sd->myInterfaces.at(cur)->setMask(str); }
    void setCheckedSocket(const QString &str) { sd->setCheckedSocket(str); }
    QString receiveFrame() const
    { return trUtf8("Получено кадров: %1").arg( sd->myInterfaces.at(cur)->countRecFrame() ); }
    QString receivePacket() const
    { return  trUtf8("Получено пакетов: %1").arg( sd->myInterfaces.at(cur)->countRecPacket() ); }
    QString sendFrame() const
    { return  trUtf8("Отправлено кадров: %1").arg( sd->myInterfaces.at(cur)->countSendFrame() ); }
    QString sendPacket() const
    { return  trUtf8("Отправлено пакетов: %1").arg(sd->myInterfaces.at(cur)->countSendPacket() ); }
private:
    int cur;
    smartDevice *sd;
};
//-------------------------------------------------------------
#endif // SMARTDEVICE_H
