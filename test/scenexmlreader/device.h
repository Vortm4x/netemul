#ifndef DEVICE_H
#define DEVICE_H

#include "deviceimpl.h"

class Device;

typedef QList<Device*> DeviceList;

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int x READ x WRITE setX )
    Q_PROPERTY( int y READ y WRITE setY )
public:
    explicit Device(QObject *parent = 0);

    void setX(int x) { m_x = x; }
    int x() const { return m_x; }
    void setY(int y) { m_y = y; }
    int y() const { return m_y; }

    Q_INVOKABLE void setDeviceImpl(DeviceImpl *im) {
//        qDebug("device impl seted!");
        m_impl = im;
        if ( m_impl ) {
            m_impl->setParent(this);
        }
    }
    DeviceImpl* deviceImpl() const { return m_impl; }

signals:

public slots:

private:
    int m_x;
    int m_y;
    DeviceImpl *m_impl;
};

#endif // DEVICE_H
