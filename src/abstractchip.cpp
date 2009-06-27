#include "abstractchip.h"

/*!
  Задает устройству случайный Mac-адрес , пустой ip и маску подсети.
*/
abstractChip::abstractChip()
{
    myMac.setRandom();
    myIp.setIp("0.0.0.0");
    myMask.setIp("0.0.0.0");
    resetStatics();
}

QDataStream& operator<<(QDataStream &stream,const abstractChip &chip)
{
    stream << chip.myType;
    stream << chip.myMac;
    stream << chip.myIp;
    stream << chip.myMask;
    stream << chip.myReceiveFrame;
    stream << chip.mySendFrame;
    stream << chip.myReceivePacket;
    stream << chip.mySendPacket;
    return stream;
}

QDataStream& operator>>(QDataStream &stream,abstractChip &chip)
{
    stream >> chip.myType;
    stream >> chip.myMac;
    stream >> chip.myIp;
    stream >> chip.myMask;
    stream >> chip.myReceiveFrame;
    stream >> chip.mySendFrame;
    stream >> chip.myReceivePacket;
    stream >> chip.mySendPacket;
    return stream;
}
/*!
  Сбрасывает всю статистику накопленную этим устройством.
*/
void abstractChip::resetStatics()
{
    myReceiveFrame = 0;
    myReceivePacket = 0;
    mySendFrame = 0 ;
    mySendPacket = 0;
}
//------------------------------------------------


