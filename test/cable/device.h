#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "deviceport.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);

    void addPort( DevicePort *port ) {
        if ( port ) {
            port->setParent(this);
            m_ports << port;
        }
    }

    ~Device() {
        qDeleteAll(m_ports);
    }

    DevicePort* findPortByName(const QString &name) {
        foreach ( DevicePort *p , m_ports ) {
            if ( p->name() == name ) {
                return p;
            }
        }
        return 0;
    }

signals:

public slots:

private:
    PortList m_ports;

};

#endif // DEVICE_H
