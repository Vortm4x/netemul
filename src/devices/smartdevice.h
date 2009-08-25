#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include "deviceimpl.h"
#include "interface.h"
#include "programm.h"
#include <QVector>

class routeModel;
class arpModel;
class routeRecord;
class abstractSocket;

/*!
  Интелектуальное устройство, абстрактный класс объединяющий в себе
  свойства компьютера и роутера.
*/
class smartDevice : public deviceImpl
{
    Q_OBJECT
public:
    /*! Источники записи таблицы маршрутизации. */    
    enum { addNet = 100 , delNet = 101 };
    enum { UDP = 0 ,TCP = 1 };
    enum { User = 7777 };
    /*! Значения для флага записи из таблицы маршрутизации. */    
    smartDevice();
    virtual ~smartDevice();
    QString tableName() const { return tr("Routing Table"); }
    virtual void tableDialog();
    virtual void adapterDialog();
    virtual void programmsDialog();
    virtual void arpDialog();
    virtual void showLogDialog(logDialog *log) const;

    virtual bool canManageInterface() const = 0;

    virtual statistics deviceStatistics() const;
    bool isSmart() const { return true; }
    bool isReady() const;
    void checkReady() const;
    void checkTable();
    bool isBusy() const;
    QStringList sockets() const;
    QStringList interfacesIp() const;
    void addInterface();
    bool isConnectSocket(const QString &socket) const { return adapter(socket)->isConnect(); }
    QString socketName(const cableDev *c) const;
    QString nameToIp(const QString &name) const { return adapter(name)->ip().toString(); }
    const interface* adapter(const QString &s) const;
    interface* ipToAdapter(const ipAddress a);
    void addConnection(const QString &port, cableDev *c);
    void deleteConnection(cableDev *c);
    void sendMessage(const QString &a, int size ,int type);
    void receivePacket(ipPacket &p,interface *f);
    void treatPacket(ipPacket &p);
    void routePacket(ipPacket &p);
    void connectedNet(interface *p);
    void deciSecondTimerEvent();
    void secondTimerEvent();
    ipAddress findInterfaceIp(ipAddress a);
    programm programmAt(const quint16 p) const;
    programm programmAt(const QString n) const;
    void removeProgramm(programm p);
    void installProgramm( programm p);
    bool sendInterrupt(int u);
    int socketsCount() const { return myInterfaces.count(); }
    QList<programm> programms() { return myProgramms; }
    QVector<interface*> interfaces() { return myInterfaces; }
    void setRouter(bool n) { myRouter = n; }
    bool isRouter() const { return myRouter; }
    bool hasTable() const { return true; }
    ipAddress gateway() const;
    routeModel* routeTable() { return myRouteTable; }
    QList<arpModel*> arpModels();
    friend class tcpSocket;
    friend class udpSocket;
    friend class ripProgramm;
    friend class adapterSetting;
public slots:
    void setCheckedSocket(const QString &str);
    void tableChanged(routeRecord*,int n);
    void setIp(const QString &a, const QString &ip);
    void setMask(const QString &a, const QString &ip);
    void setGateway(const QString &str);
    virtual quint64 sendPacketCount(const QString &name) { return adapter(name)->countSendPacket(); }
    virtual quint64 receivePacketCount(const QString &name) { return adapter(name)->countRecPacket(); }
    virtual quint64 sendFrameCount(const QString &name) { return adapter(name)->countSendFrame(); }
    virtual quint64 receiveFrameCount(const QString &name) { return adapter(name)->countRecFrame(); }
private:
    interface* adapter(const QString &name);
protected:
    interface* addInterface(const QString &name);
    void deleteInterface(const QString &name);
    bool myRouter;
    mutable bool myReady;
    mutable bool isDirty;
    QVector<interface*> myInterfaces;
    QList<programm> myProgramms; //!< Программы установленные на устройстве.
    QList<abstractSocket*> mySockets; //!< Список установленных соединений.
    routeModel *myRouteTable; //!< Таблица маршрутизации.
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
};
//-------------------------------------------------------------------
/*!
  Модель данных для настроек адаптеров.
*/
class adapterSetting {
public:
    adapterSetting(smartDevice *s) : sd(s) { }
    void setCurrent(int n) { cur = n; }
    int current() const { return cur; }
    void resetStatics() { sd->myInterfaces.at(cur)->resetStatics(); }
    void connectedNet() { sd->connectedNet(sd->myInterfaces.at(cur)); }
    int socketsCount() const { return sd->socketsCount(); }
    bool isConnect() const { return sd->myInterfaces.at(cur)->isConnect(); }
    QString name() const { return sd->myInterfaces.at(cur)->name(); }
    QString mac() const { return sd->myInterfaces.at(cur)->mac().toString(); }
    QString ip() const { return sd->myInterfaces.at(cur)->ip().toString(); }
    QString mask() const { return sd->myInterfaces.at(cur)->mask().toString(); }
    void setMac(const QString &str) { sd->myInterfaces.at(cur)->setMac(str); }
    void setIp(const QString &str) { sd->myInterfaces.at(cur)->setIp(str); }
    void setMask(const QString &str) { sd->myInterfaces.at(cur)->setMask(str); }
    void setCheckedSocket(const QString &str) { sd->setCheckedSocket(str); }
    QString statics() const { return sd->myInterfaces.at(cur)->staticsString(); }
    void sendArpRequest(ipAddress a) { sd->myInterfaces.at(cur)->sendArpRequest(a); }
    bool canManageInterface() const { return sd->canManageInterface(); }
    void addInterface() { sd->addInterface(); }
    void deleteInterface(const QString &name) { sd->deleteInterface(name); }
private:
    int cur;
    smartDevice *sd;
};
//-------------------------------------------------------------

class smartSetting  : public deviceSetting {
public:
    smartSetting(smartDevice *d) : deviceSetting(d) , sd(d) { }
    int socketsCount() const { return sd->socketsCount(); }
    bool isRouter() const { return sd->isRouter(); }
    void setRouter(bool b) { sd->setRouter(b); }
private:
    smartDevice *sd;
};


#endif // SMARTDEVICE_H
