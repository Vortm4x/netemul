#ifndef DEVICEIMPL_H
#define DEVICEIMPL_H

#include <deviceport.h>

#define SHOW_DIALOG(A) A *d = new A; \
                      d->setDevice(this); \
                      d->exec(); \
                      delete d;

class deviceImpl
{
public:
    deviceImpl();
    virtual ~deviceImpl() { }

    virtual int type() const = 0;

    void setCheckedSocket(const QString &s);   
    virtual devicePort* socket(const QString &name) = 0;
    virtual QStringList sockets() const = 0;
    virtual bool isConnectSocket(const QString &socket) const = 0;
    virtual QString socketName(devicePort *p) const = 0;

    const QString& note() const { return myNote; }
    void setNote(const QString &n) { myNote = n; }

    virtual QString tableName() const { return QString(); }
    virtual bool isSmart() const { return false; }

    void sendEvent();
    virtual bool hasTable() const { return false; }
    virtual void dialog() = 0;
    virtual void tableDialog() const { qFatal("error not compability!"); }
    virtual void adapterDialog() const { qFatal("error not compability!"); }
    virtual void programmsDialog() const { qFatal("error not compability"); }

    virtual void write(QDataStream &stream) const = 0;
    virtual void read(QDataStream &stream) = 0;

    virtual void secondTimerEvent() { }
    virtual void deciSecondTimerEvent() { }

    virtual bool isCanSend() const { return false; }
    virtual void sendMessage(ipAddress dest, int size , int pr) { Q_UNUSED(dest)
                                                                  Q_UNUSED(size) Q_UNUSED(pr) qFatal("error!!!"); }
private:
    QString myNote;
};

#endif // DEVICEIMPL_H
