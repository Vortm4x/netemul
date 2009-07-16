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
    Q_DECLARE_TR_FUNCTIONS(computer)
public:
    computer(int c = 1);
    ~computer() { }
    devicePort* addInterface(QString str,int t);

    void dialog();
    QString hasTable() const { return trUtf8("Таблица маршрутизации"); }
protected:
    void write(QDataStream &stream) const { stream << compDev; smartDevice::write(stream);}
    void read(QDataStream &stream) { smartDevice::read(stream); }
};
//---------------------------------------------
#endif // COMPUTER_H
