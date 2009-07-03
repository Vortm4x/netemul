#include "ripprogramm.h"
#include "smartdevice.h"

ripProgramm::ripProgramm(smartDevice *d)
{
    sd = d;
    mySocket = 520;
    inter = 30;
    t = qrand()%30;
}

/*! Отсчитывает интервалы, по истечении которых
  происходит отправка rip-сообщений.
*/
void ripProgramm::incTime()
{
    if ( ++t < inter || !sd->myRouteMode) return;
}
//--------------------------------------------------
/*! Обрабатывает rip-сообщения от соседних
  маршрутизаторов
  @param b - сообщение маршрутизатора.
*/
void ripProgramm::execute(QByteArray &b)
{
    Q_UNUSED(b);
}
//---------------------------------------------------
//*!
