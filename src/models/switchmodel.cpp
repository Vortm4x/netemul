#include "switchmodel.h"
#include "deviceport.h"

switchModel::switchModel(QObject *parent /* = 0 */) : QAbstractTableModel(parent)
{

}

int switchModel::rowCount(const QModelIndex &/* p */) const
{
    if ( table.isEmpty() ) return 0;
    return table.count();
}

int switchModel::columnCount(const QModelIndex &/* p */) const
{
    if ( table.isEmpty() ) return 0;
    return 4;
}

Qt::ItemFlags switchModel::flags(const QModelIndex &m) const
{
    return QAbstractTableModel::flags(m);
}

QVariant switchModel::headerData(int s , Qt::Orientation o , int role) const
{
    if ( role != Qt::DisplayRole ) return QVariant();
    if ( o == Qt::Horizontal ) {
        switch (s) {
            case 0: return trUtf8("MAC-адрес");
            case 1: return trUtf8("Порт");
            case 2: return trUtf8("Тип записи");
            case 3: return trUtf8("Время жизни");
        }
    }
    return s+1;
}

QVariant switchModel::data(const QModelIndex &m, int role) const
{
    if ( !m.isValid() || table.isEmpty() ) return QVariant();
    if ( role == Qt::DisplayRole ) {
        macRecord *t = table.at( m.row() );
        switch ( m.column() ) {
            case 0: return t->mac.toString();
            case 1: return trUtf8("LAN%1").arg(t->port->num());
            case 2: return t->modeString();
            case 3: return t->time;
        }
    }
    return QVariant();
}

macRecord* switchModel::addToTable(const macAddress &mac ,devicePort *p , int mode , int time)
{
    foreach ( macRecord *i , table )
        if ( i->mac == mac ) return NULL;
    macRecord *t = new macRecord;
    t->time = time;
    t->mode = mode;
    t->port = p;
    t->mac = mac;
    table << t;
    reset();
    return t;
}

void switchModel::deleteFromTable(const macAddress &mac)
{
    foreach ( macRecord *i , table )
        if ( i->mac == mac ) {
            table.removeOne(i);
            delete i;
            reset();
            return;
        }
}

void switchModel::contains(const macAddress &m , devicePort *s)
{
    bool cont = false;
    foreach ( macRecord *i , table )
        if ( i->mac == m ) {
            if ( i->port != s && i->mode != staticMode ) i->port = s;
            cont = true;
            break;
        }
    if ( !cont ) addToTable( m , s , dinamicMode , 0);
}

devicePort* switchModel::portWithMac(const macAddress &m)
{
    foreach ( macRecord *i , table )
        if ( i->mac == m ) {
            i->time = 0;
            return i->port;
        }
    return 0;
}
