#include "ripprogramm.h"
#include "smartdevice.h"
#include "udppacket.h"
#include "routemodel.h"
#include <QtDebug>


/*!
  Инициализирует программу стандартными настройками.
  @param d - устройство на которое устанавливается программа.
*/
ripProgramm::ripProgramm()
{
    myName = QObject::trUtf8("RIP");
    mySocket = 520;
    interval = defaultTtl;
    timer = qrand()%30;
}
//--------------------------------------------------------------
/*!
  Отсчитывает интервалы, по истечении которых
  происходит отправка rip-сообщений.
*/
void ripProgramm::incTime()
{
    if ( ++timer < interval || !sd->isRouter() ) return;
    sendUpdate(true);
    timer = 0;
}
//--------------------------------------------------
/*!
  Обрабатывает rip-сообщения от соседних
  маршрутизаторов
  @param b - Полученное сообщение.
*/
void ripProgramm::execute(ipPacket &p)
{
    udpPacket u(p.unpack());
    QDataStream d(u.unpack());
    int count = u.size();
    for ( int i = 0; i < count ; i++ ){
        routeRecord *t = new routeRecord;
        d >> t->dest >> t->mask >> t->metric;
        Q_ASSERT( t->metric >= 0 && t->metric <= infinity);
        t->metric++;
        t->out = sd->findInterfaceIp( p.sender() );
        t->gateway = p.sender();
        t->time = 0;
        t->mode = routeModel::ripMode;
        t->change = routeModel::noChanged;
        checkTable(t);
    }
}
//---------------------------------------------------
/*!
    Посылает rip-сообщение.
*/
void ripProgramm::sendUpdate(bool /*isAll*/)
{
//    if ( !isAll) foreach ( routeRecord *i, sd->myRouteTable )
//                    if ( i->mode == routeModel::ripMode ) i->time++;
//    foreach ( interface *i , sd->myInterfaces )
//        if ( i->isConnect() ) {
//            QByteArray t;
//            QDataStream d(&t , QIODevice::WriteOnly);
//            if ( isAll ) {
//                foreach ( routeRecord *j , sd->myRouteTable ) {
//                    if ( j->out.isLoopBack() ) continue;
//                    if ( (j->gateway & i->mask()) == ( i->ip() & i->mask() ) ) continue;
//                    d << j->dest << j->mask;
//                    if ( j->time == 6 ) d << infinity;
//                    else d << j->metric;
//                }
//            }
//            else {
//                routeRecord *r = findChanged();
//                if ( !r ) return;
//                if ( (r->gateway & i->mask()) == ( i->ip() & i->mask() ) ) continue;
//                d << r->dest << r->mask << r->metric;
//            }
//            ipPacket p; // Создаем новый пакет.
//            p->setSender( i->ip() );
//            p->setBroadcast( i->mask() );
//            p->setUpProtocol( ipPacket::udp );
//            udpPacket u; // И новую дейтаграмму
//            u.setSender(mySocket);
//            u.setReceiver(mySocket);
//            u.pack(t); // В нее вектор
//            p->pack(u.toData()); // Её в пакет.
//            i->sendPacket(p); // Пакет отправляем.
//        }
//    foreach ( routeRecord *i, sd->myRouteTable )
//        if ( i->time == 6 ) sd->deleteFromTable(i,false);
}
//---------------------------------------------------
/*!
  Проверяет таблицу на наличие одинаковых записей.
  @param r - Указатель на новую запись.
*/
void ripProgramm::checkTable(routeRecord *r)
{
    routeRecord *i = model->recordAt( r->dest ); // Ищем запись с таким же адресом назначения
    if ( !i && r->metric < 16 ) { // Если её нет, добавляем эту и выходим.
        model->addToTable(r);
        return;
    }
    if ( !i ) return;
    if ( i->mode != routeModel::ripMode ) return; // Если это не RIP запись выходим.
    if ( r->metric >= 16 ) {
        model->deleteFromTable(i); // Если метрика >=16 значит этой сети больше нет, удаляем.
        delete r;
        return;
    }
    if ( i->metric > r->metric || ( i->gateway == r->gateway &&
                                    i->metric != r->metric ) ) { // Если метрика этой записи меньше прищедшей
        i->metric = r->metric;
        i->gateway = r->gateway;
        i->time = 0;
        i->out = r->out;
        return;
    }
    if ( i->metric == r->metric ) { // Если равны значит сеть еще жива на том же расстоянии.
        i->time = 0;
        return;
    }
    delete r;
}
//---------------------------------------------------
/*!
  Обработчик программных прерываний от устройства.
  @param u - номер прерывания.
*/
bool ripProgramm::interrupt(int u)
{
    routeRecord *t = findChanged();
    if ( !t ) return false;
    switch (u) {
        case smartDevice::addNet : // Если добавляется сеть рассылаем всем новую таблицу.
            if ( interval - timer > 3 ) sendUpdate(false);
            t->change = routeModel::noChanged;
            return true;
        case smartDevice::delNet : // И когда удаляется тоже.
            t->metric = 16;
            if ( interval - timer > 3 ) sendUpdate(false);
            return true;
        default:
            break;
    }
    return false;
}
//---------------------------------------------------
/*!
  Находит последнюю измененую запись.
  @return указатель на запись.
*/
routeRecord* ripProgramm::findChanged() const
{
//    foreach ( routeRecord *i , sd->myRouteTable )
//        if ( i->change == routeModel::changed ) return i;
    return NULL;
}
//---------------------------------------------------

/*!
  Записывает отличительные черты RIP в поток.
  @param stream - поток для записи.
*/
void ripProgramm::write(QDataStream &stream) const
{
    stream << (int)RIP;
    programmRep::write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты RIP из потока.
  @param stream - поток для чтения.
*/
void ripProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
}
//---------------------------------------------------

