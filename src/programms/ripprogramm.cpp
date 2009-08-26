#include "ripprogramm.h"
#include "smartdevice.h"
#include "udpsocket.h"
#include "routemodel.h"
#include <QtDebug>

/*!
  Инициализирует программу стандартными настройками.
  @param d - устройство на которое устанавливается программа.
*/
ripProgramm::ripProgramm()
{
    myName = QObject::tr("RIP");
    mySocket = 520;
    interval = defaultTtl;
    timer = qrand()%30;
}
//--------------------------------------------------------------

ripProgramm::~ripProgramm()
{
    clearTemp();
    delete receiver;
}

void ripProgramm::clearTemp()
{
    qDeleteAll(tempList);
    tempList.clear();
}

void ripProgramm::setDevice(smartDevice *s)
{
    sd = s;
    model = sd->routeTable();
    receiver = new udpSocket(s,mySocket);
    receiver->setBind("0.0.0.0");
}
/*!
  Отсчитывает интервалы, по истечении которых
  происходит отправка rip-сообщений.
*/
void ripProgramm::incTime()
{
    if ( !sd->isRouter() ) return;
    while ( receiver->sizeReceiveData() ) {
        QByteArray data;
        receiver->read(data);
        execute(data);
    }
    timer++;
    if ( timer >= interval ) {
        sendUpdate(true);
        timer = 0;
        return;
    }
    if ( timer != 0 && timer%5 == 0 && !tempList.isEmpty() ) sendUpdate(false);
}
//--------------------------------------------------
/*!
  Обрабатывает rip-сообщения от соседних
  маршрутизаторов
  @param b - Полученное сообщение.
*/
void ripProgramm::execute(QByteArray data)
{
    ipAddress sender;
    quint16 size;
    QDataStream d(data);
    d >> size;
    d >> sender;
    int count = (size-4)/9;
    for ( int i = 0; i < count ; i++ ) {
        routeRecord *t = new routeRecord;
        d >> t->dest >> t->mask >> t->metric;
        Q_ASSERT( t->metric >= 0 && t->metric <= infinity);
        t->metric++;
        t->out = sd->findInterfaceIp( sender );
        t->gateway = sender;
        t->time = 0;
        t->mode = routeModel::ripMode;
        t->change = routeModel::noChanged;
        checkTable(t);
    }
}
//---------------------------------------------------
/*!
    Посылает rip-сообщение.
    @param isAll - если true, рассылается вся таблица при регулярном update, если false то только изменившиеся записи.
*/
void ripProgramm::sendUpdate(bool isAll)
{
    if ( isAll ) model->update();
    foreach ( interface *i , sd->interfaces() ) {
        if ( !i->isConnect() ) continue;
        QByteArray b;
        QDataStream d(&b, QIODevice::WriteOnly );
        d << quint16(0);
        d << i->ip();
        if ( isAll ) {
            for ( int j = 0 ; j < model->rowCount() ; j++ ) {
                routeRecord *t = model->recordAt(j);
                if ( t->dest.isLoopBack() ) continue;
                if ( ( t->gateway & i->mask() ) == ( i->ip() & i->mask() ) ) continue;
                d << t->dest << t->mask;
                if ( t->time == ttl ) d << infinity;
                else d << t->metric;
            }
        }
        else {
            foreach ( routeRecord *j , tempList ) {
                if ( ( j->gateway & i->mask()) == ( i->ip() & i->mask() ) ) continue;
                d << j->dest << j->mask << j->metric;
            }
        }
        d.device()->seek(0);
        d << quint16(b.size()-2);
        ipAddress temp = i->ip() | ~i->mask();
        udpSocket sock(sd, mySocket);
        sock.write(temp,mySocket,b);
    }
    if (tempList.size()) clearTemp();
    model->deleteOldRecord(ttl);
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
    if ( !i ) {
        delete r;
        return;
    }
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
    routeRecord *t = 0;
    switch (u) {
        case smartDevice::addNet : // Если добавляется сеть рассылаем всем новую таблицу.
            addToTemp( model->changedRecord() );
            return true;
        case smartDevice::delNet : // И когда удаляется тоже.
            t = model->changedRecord();
            t->metric = 16;
            addToTemp(t);
            return true;
        default:
            break;
    }
    return false;
}
//---------------------------------------------------

void ripProgramm::addToTemp(routeRecord *r)
{
    routeRecord *t = new routeRecord;
    *t = *r;
    tempList << t;
}
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

