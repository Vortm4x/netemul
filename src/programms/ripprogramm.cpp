/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#include "ripprogramm.h"
#include "smartdevice.h"
#include "routemodel.h"
#include "udpsocket.h"
#ifndef __TESTING__
#include "ripproperty.h"
#endif

/*!
  Инициализирует программу стандартными настройками.
  @param d - устройство на которое устанавливается программа.
*/
ripProgramm::ripProgramm()
{
    myName = tr("RIP");
    mySocket = 520;
    interval = defaultTtl;
    timer = qrand()%30;
    myTriggerUpdate = true;
    mySplitMode = SPLIT_HORIZONT;
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
    if ( s == 0 ) return;
    device = s;
    model = device->routeTable();
    receiver = new udpSocket(s,mySocket);
    receiver->setBind("0.0.0.0");
    connect( receiver , SIGNAL(readyRead(QByteArray)) , SLOT(execute(QByteArray)) );
}
/*!
  Отсчитывает интервалы, по истечении которых
  происходит отправка rip-сообщений.
*/
void ripProgramm::incTime()
{
    if ( !myEnable || !device->isRouter() ) return;
    timer++;
    if ( timer >= interval ) {
        sendUpdate(true);
        timer = 0;
        return;
    }
    if ( mySplitMode != SPLIT_NONE && timer != 0 && timer%5 == 0 && !tempList.isEmpty() ) sendUpdate(false);
}
//--------------------------------------------------
/*!
  Обрабатывает rip-сообщения от соседних
  маршрутизаторов
  @param b - Полученное сообщение.
*/
void ripProgramm::execute(QByteArray data)
{
    if ( !myEnable ) return;
    ipAddress sender;
    quint16 size;
    QDataStream d(data);
    d >> size;
    d >> sender;
    int count = (size-4)/9;
    for ( int i = 0; i < count ; i++ ) {
        routeRecord *t = new routeRecord;
        d >> t->dest >> t->mask >> t->metric;
        Q_ASSERT( t->metric >= 0 && t->metric <= RIP_INFINITY);
        t->metric++;
        t->out = device->findInterfaceIp( sender );
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
    foreach ( interface *i , device->interfaces() ) {
        if ( !i->isConnect() ) continue;
        QByteArray b;
        QDataStream d(&b, QIODevice::WriteOnly );
        d << quint16(0);
        d << i->ip();
        if ( isAll ) {
            for ( int j = 0 ; j < model->rowCount() ; j++ ) {
                routeRecord *t = model->recordAt(j);
                if ( t->dest.isLoopBack() ) continue;
                qint8 metric = ( t->time == ttl ) ? RIP_INFINITY : t->metric;
                if ( ( t->gateway & i->mask() ) == ( i->ip() & i->mask() ) ) {
                    if ( mySplitMode == SPLIT_HORIZONT ) continue;
                    else if ( mySplitMode == SPLIT_WIH_POISON && metric < RIP_INFINITY ) continue;
                }
                d << t->dest << t->mask << metric;
            }
        }
        else {
            foreach ( routeRecord *j , tempList ) {
                qint8 metric = ( j->time == ttl ) ? RIP_INFINITY : j->metric;
                if ( ( j->gateway & i->mask() ) == ( i->ip() & i->mask() ) ) {
                    if ( mySplitMode == SPLIT_HORIZONT ) continue;
                    else if ( mySplitMode == SPLIT_WIH_POISON && metric < RIP_INFINITY ) continue;
                }
                d << j->dest << j->mask << metric;
            }
        }
        d.device()->seek(0);
        d << quint16(b.size()-2);
        ipAddress temp = i->ip() | ~i->mask();
        udpSocket sock(device, mySocket);
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
    if ( r->metric >= 16 && r->mode != routeModel::connectMode ) {
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
    if ( !myEnable || mySplitMode == SPLIT_NONE ) return false;
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
void ripProgramm::showProperty()
{
#ifndef __TESTING__
    ripProperty *d = new ripProperty;
    d->setProgramm(this);
    d->exec();
#endif
}

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
    stream << RIP;
    programmRep::write(stream);
    stream << mySplitMode << myTriggerUpdate;
}
//---------------------------------------------------
/*!
  Считывает отличительные черты RIP из потока.
  @param stream - поток для чтения.
*/
void ripProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
    stream >> mySplitMode >> myTriggerUpdate;
}
//---------------------------------------------------




