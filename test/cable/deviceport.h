#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <QObject>
#include <QByteArray>

class DevicePort;
typedef QList<DevicePort*> PortList;

class DevicePort : public QObject
{
    Q_OBJECT
public:
    explicit DevicePort(QObject *parent = 0);
    DevicePort(const QString &name , QObject *parent = 0) : QObject(parent) , m_name(name) {
    }

    bool isShared() const { return false; }
    void receiveFrame(const QByteArray &data) { }
    void detectCollision() { }

    void setName(const QString &name) {
         m_name = name;
    }
    QString name() const { return m_name; }

signals:

public slots:

private:
    QString m_name;

};

#endif // DEVICEPORT_H
