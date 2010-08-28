#ifndef COMPUTER_H
#define COMPUTER_H

#include "deviceimpl.h"
#include "interface.h"

class Computer : public DeviceImpl
{
    Q_OBJECT
public:
    explicit Computer(QObject *parent = 0);

    QString className() const { return "Computer"; }

    InterfaceList interfaces() const { return m_interfaces; }

    Q_INVOKABLE void addInterface(Interface *in) {
        if ( in ) {
            in->setParent(this);
            m_interfaces << in;
        }
    }

signals:

public slots:

private:
    InterfaceList m_interfaces;

};

#endif // COMPUTER_H
