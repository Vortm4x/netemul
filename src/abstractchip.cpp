#include "abstractchip.h"

/*!
  Задает устройству случайный Mac-адрес, пустой ip и маску подсети.
*/
abstractChip::abstractChip()
{
    myMac.setRandom();
    myIp.setIp("0.0.0.0");
    myMask.setIp("0.0.0.0");
    resetStatics();
}
//--------------------------------------------------------------
/*!
  Сбрасывает всю статистику, накопленную этим устройством.
*/
void abstractChip::resetStatics()
{
    myReceiveFrame = 0;
    myReceivePacket = 0;
    mySendFrame = 0 ;
    mySendPacket = 0;
}
//------------------------------------------------


