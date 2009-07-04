#include "ripprogramm.h"
#include "smartdevice.h"

ripProgramm::ripProgramm(smartDevice *d)
{
    myName = trUtf8("RIP");
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
    sendRip();
    t = 0;
}
//--------------------------------------------------
/*! Обрабатывает rip-сообщения от соседних
  маршрутизаторов
  @param b - Полученное сообщение.
*/
void ripProgramm::execute(ipPacket *p)
{
    udpPacket u;
    *p >> u;
    QDataStream d(u.data());
    int count = u.data().size() / 9;
    for ( int i = 0; i < count ; i++ ){
        routeRecord *t = new routeRecord;
        d >> t->dest >> t->mask >> t->metric;
        t->metric++;
        t->out = sd->ipToAdapter(p->receiver());
        t->gateway = p->sender();
        t->time = 0;
        checkTable(t);
    }
    delete p;
}
//---------------------------------------------------
/*!
    Посылает rip-сообщение.
*/
void ripProgramm::sendRip()
{
    QByteArray t;
    QDataStream d(&t , QIODevice::WriteOnly);
    foreach ( routeRecord *i , sd->myRouteTable ) // Перебираем таблицу
        d << i->dest << i->mask << i->metric; // Записываем нужное.
    foreach ( devicePort *i , sd->mySockets )
        if ( i->isConnect() ) {
            ipPacket *p = new ipPacket; // Создаем новый пакет.
            p->setSender( i->parentDev()->ip() );
            p->setBroadcast( i->parentDev()->mask() );
            udpPacket u; // И новую дейтаграмму
            u.setSender(mySocket);
            u.setReceiver(mySocket);
            u.setData(t); // В нее вектор
            *p << u; // Её в пакет.
            i->parentDev()->sendPacket(p); // Пакет отправляем.
        }
}
//---------------------------------------------------
/*!
  Проверяет таблицу на наличие одинаковых записей.
  @param r - Указатель на новую запись.
*/
void ripProgramm::checkTable(routeRecord *r)
{
    foreach (routeRecord *i , sd->myRouteTable )
        if ( (i->dest == r->dest) && (i->mask == r->mask) && (i->metric > r->metric) ) {
            sd->deleteFromTable(i);
            sd->addToTable(r);
            return;
        }
    sd->addToTable(r);
}
//---------------------------------------------------
