#ifndef DEVICEIMPL_H
#define DEVICEIMPL_H

#include <QtScript>

class cableDev;

#define SHOW_DIALOG(A) A *d = new A; \
                      d->setDevice(this); \
                      d->exec(); \
                      delete d;

class deviceImpl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString note READ note WRITE setNote)
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
    virtual void tableDialog() { qFatal("error not compability Table!"); }
    virtual void adapterDialog() { qFatal("error not compability adapter!"); }
    virtual void programmsDialog() { qFatal("error not compability"); }
    virtual void sendMessage(const QString &a,int size , int pr) { Q_UNUSED(a) Q_UNUSED(size) Q_UNUSED(pr)
                                                                   qFatal("ERROR!"); }

    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);

    virtual void secondTimerEvent() { }
    virtual void deciSecondTimerEvent() { }
    virtual QString nameToIp(const QString &name) const { Q_UNUSED(name) return QString(); }

    virtual bool isReady() { return true; }
    virtual void addConnection(const QString &port , cableDev *c) = 0;
    virtual bool isCanSend() const { return false; }
public slots:
    virtual void setIp(const QString &a , const QString &ip);
    virtual void setMask(const QString &a, const QString &ip);
    virtual void setGateway(const QString &str);
    virtual void setSocketsCount(int n);
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

Q_DECLARE_METATYPE(deviceImpl*)






#endif // DEVICEIMPL_H
