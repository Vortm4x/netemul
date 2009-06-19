#include "abstractchip.h"

abstractChip::abstractChip()
{
    quint8 t[4] = { 0 , 0 , 0 ,0 };
    myMac.setRandom();
    myIp.setIp(t);
    myMask.setIp(t);
    myReceiveFrame = 0;
    myReceivePacket = 0;
    mySendFrame = 0 ;
    mySendPacket = 0;
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



