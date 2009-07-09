#include "ripprogramm.h"
#include "smartdevice.h"
#include <QtDebug>

/*!
  Инициализирует программу стандартными настройками.
  @param d - устройство на которое устанавливается программа.
*/
ripProgramm::ripProgramm(smartDevice *d)
{
    myName = trUtf8("RIP");
    sd = d;
    mySocket = 520;
    inter = defaultTtl;
    t = qrand()%30;
}
//--------------------------------------------------------------
/*!
  Отсчитывает интервалы, по истечении которых
  происходит отправка rip-сообщений.
*/
void ripProgramm::incTime()
{
    if ( ++t < inter || !sd->myRouteMode) return;
    sendRip();
    t = 0;
}
//--------------------------------------------------
/*!
  Обрабатывает rip-сообщения от соседних
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
        if ( t->metric < 0 || t->metric > 16 ) {
            qDebug() << "1";
            delete t;
            continue;
        }
        t->metric = qMin( t->metric+1 , 16 );
        t->out = sd->ipToAdapter( sd->findInterfaceIp( p->sender() ) );
        t->gateway = p->sender();
        t->time = 0;
        t->mode = smartDevice::ripMode;
        t->change = smartDevice::noChanged;
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
    foreach ( routeRecord *i , sd->myRouteTable ) { // Перебираем таблицу
        d << i->dest << i->mask;
        if ( i->mode == smartDevice::ripMode ) i->time++;
        if ( i->time == 6 ) {
            sd->deleteFromTable(i);
            d << qint8(16);
        }
        else d << i->metric;
    }
    foreach ( devicePort *i , sd->mySockets )
        if ( i->isConnect() ) {
            ipPacket *p = new ipPacket; // Создаем новый пакет.
            p->setSender( i->parentDev()->ip() );
            p->setBroadcast( i->parentDev()->mask() );
            p->setUpProtocol( ipPacket::udp );
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
    routeRecord *i = sd->recordAt( r->dest ); // Ищем запись с таким же адресом назначения
    if ( !i ) { // Если её нет, добавляем эту и выходим.
        sd->addToTable(r);
        return;
    }
    if ( i->mode != smartDevice::ripMode ) return; // Если это не RIP запись выходим.
    if ( i->metric > r->metric ) { // Если метрика этой записи меньше прищедшей
        sd->deleteFromTable(i); // Добавляем лучшую.
        sd->addToTable(r);
        return;
    }
    if ( i->metric == r->metric ) { // Если равны значит сеть еще жива на том же расстоянии.
        i->time = 0;
        return;
    }
    if ( r->metric >= 16 ) sd->deleteFromTable(i); // Если метрика >=16 значит этой сети больше нет, удаляем.
}
//---------------------------------------------------
/*!
  Обработчик программных прерываний от устройства.
  @param u - номер прерывания.
*/
void ripProgramm::interrupt(int u)
{
    switch (u) {
        case smartDevice::addNet : // Если добавляется сеть рассылаем всем новую таблицу.
        case smartDevice::delNet : // И когда удаляется тоже.
            sendRip();
            break;
        default:
            break;
    }
}
//---------------------------------------------------

