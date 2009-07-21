#ifndef DEVICEIMPL_H
#define DEVICEIMPL_H

#include <QObject>

class cableDev;

#define SHOW_DIALOG(A) A *d = new A; \
                      d->setDevice(this); \
                      d->exec(); \
                      delete d;

class deviceImpl : public QObject
{
public:
    deviceImpl();
    virtual ~deviceImpl() { }

    virtual int type() const = 0;

    virtual void setCheckedSocket(const QString &s) { Q_UNUSED(s) }
    virtual QStringList sockets() const = 0;
    virtual bool isConnectSocket(const QString &socket) const = 0;
    virtual QString socketName(const cableDev *c) const = 0;

    const QString& note() const { return myNote; }
    void setNote(const QString &n) { myNote = n; }

    virtual QString tableName() const { return QString(); }
    virtual bool isSmart() const { return false; }

    virtual bool hasTable() const { return false; }
    virtual void dialog() = 0;
    virtual void tableDialog() const { qFatal("error not compability!"); }
    virtual void adapterDialog() const { qFatal("error not compability!"); }
    virtual void programmsDialog() const { qFatal("error not compability"); }
    virtual void sendMessage(const QString &a,int size , int pr) { Q_UNUSED(a) Q_UNUSED(size) Q_UNUSED(pr)
                                                                   qFatal("ERROR!"); }

    virtual void write(QDataStream &stream) const = 0;
    virtual void read(QDataStream &stream) = 0;

    virtual void secondTimerEvent() { }
    virtual void deciSecondTimerEvent() { }
    virtual QString nameToIp(const QString &name) const { Q_UNUSED(name) return QString(); }

    virtual bool isReady() { return true; }
    virtual void addConnection(const QString &port , cableDev *c) = 0;
    virtual bool isCanSend() const { return false; }
private:
    QString myNote;
};

class deviceSetting {
public:
    deviceSetting(deviceImpl *d) : dev(d) { }
    QString note() const { return dev->note(); }
    void setNote(const QString &str) { dev->setNote(str); }
private:
    deviceImpl *dev;
};

#endif // DEVICEIMPL_H
