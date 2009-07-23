#ifndef COMPUTER_H
#define COMPUTER_H

#include "smartdevice.h"
#include "deviceport.h"
#include <QDataStream>

class devicePort;
/*!
  Реализует компьютер.
*/
class computer : public smartDevice
{
public:
    enum { compDev = 3 };
    int type() const { return compDev; }
    computer(int c = 1);
    ~computer() { }

    void dialog();
    bool hasTable() const { return true; }

    bool isCanSend() const { return true; }
protected:
    void write(QDataStream &stream) const { stream << compDev; smartDevice::write(stream);}
    void read(QDataStream &stream) { smartDevice::read(stream); }
};
//---------------------------------------------

class computerSetting : public smartSetting {
public:
    computerSetting( computer *c ) : smartSetting(c) , cp(c) { }
    QString gateway() const { return cp->gateway().toString(); }
    void setGateway(const QString &str) { cp->setGateway(str); }
private:
    computer *cp;
};
#endif // COMPUTER_H
