#include "routemodel.h"

routeModel::routeModel(QObject *parent /* = 0 */) : QAbstractTableModel(parent)
{    
}

routeModel::~routeModel()
{
    qDeleteAll(table);
    table.clear();
}

int routeModel::rowCount(const QModelIndex &r) const
{
    if ( r.isValid() ||( table.count() == 0 ) ) return 0;
    return table.size();
}

int routeModel::columnCount( const QModelIndex&/* r = QModelIndex() */ ) const
{
    if ( table.isEmpty() ) return 0;
    return 6;
}

QVariant routeModel::headerData( int s , Qt::Orientation o, int role ) const
{
    if ( table.isEmpty() ) return QVariant();
    if ( o == Qt::Horizontal )
        if ( role == Qt::DisplayRole )
            switch (s) {
                case 0: return trUtf8("Адрес назначения");
                case 1: return trUtf8("Маска");
                case 2: return trUtf8("Шлюз");
                case 3: return trUtf8("Интерфейс");
                case 4: return trUtf8("Метрика");
                case 5: return trUtf8("Источник");
            }
    if ( o == Qt::Vertical )
        if ( role == Qt::DisplayRole ) return s+1;
    return QAbstractTableModel::headerData(s, o, role);
}

Qt::ItemFlags routeModel::flags( const QModelIndex &r) const
{
    if ( !r.isValid() || table.isEmpty() ) return 0;
    return QAbstractTableModel::flags(r);
}

QVariant routeModel::data(const QModelIndex &r, int role /* = Qt::DisplayRole */ ) const
{
    if ( !r.isValid() || table.isEmpty() ) return QVariant();
    routeRecord *rec = table.at(r.row());
    if ( role == Qt::DisplayRole )
        switch ( r.column() ) {
            case 0: return rec->dest.toString();
            case 1: return rec->mask.toString();
            case 2: return rec->gateway.toString();
            case 3: return rec->out.toString();
            case 4: return rec->metric;
            case 5: return rec->modeString();
        }
    return QVariant();
}

/*!
  Добавляет в таблицу маршрутизации новую запись.
  @param d - сеть назначения.
  @param m - маска сети.
  @param g - адрес следующего маршрутизатора.
  @param o - интерфейс с которого отправляем.
  @param metr - метрика
  @param mode - источник записи.
  @return указатель на новую запись
*/
routeRecord* routeModel::addToTable(ipAddress d,ipAddress m,ipAddress g,ipAddress o,qint8 metr,int mode)
{
    routeRecord *r = new routeRecord;
    r->dest = d;
    r->mask = m;
    r->metric = metr;
    r->gateway = g;
    r->mode = mode;
    r->time = 0;
    r->change = noChanged;
    r->out = o;
    return addToTable(r);
}
//---------------------------------------------------------------
/*!
  Добавляет запись в таблицу маршрутизации.
  @param r - указатель на запись.
  @param tr - нужно ли вызывать прерывание(по умолчанию нужно).
  @return указатель добавленную на запись.
*/
routeRecord* routeModel::addToTable(routeRecord *r)
{
    table << r;
    qStableSort(table.begin(),table.end(),routeGreat);    
    reset();
    return r;    
}
//------------------------------------------------------------
void routeModel::deleteFromTable(int n)
{
    int v = 0;
    foreach ( routeRecord *i , table )
        if ( v++ == n ) {
            deleteFromTable(i);
            return;
        }
}
/*!
  Удаляет запись из таблицы маршрутизации.
  @param r - указатель на запись.
  @param tr - нужно ли вызывать прерывание(по умолчанию нужно).
*/
void routeModel::deleteFromTable(routeRecord *r)
{
    table.removeOne(r);
    delete r;
    qStableSort(table.begin(),table.end(),routeGreat);
    reset();
}
//--------------------------------------------------------------
/*!
  Находит в таблице маршрутизации.
  @param a - адрес назначения.
  @return указатель на запись, если такой записи нет то NULL.
*/
routeRecord* routeModel::recordAt(const ipAddress &a) const
{
    foreach ( routeRecord *i , table )
        if ( i->dest == ( a & i->mask ) ) return i;
    return NULL;
}
//---------------------------------------------

void routeModel::checkConnectedNet(ipAddress ip, ipAddress mask, bool add)
{
    ipAddress dest = mask & ip;
    foreach ( routeRecord *i , table )
        if ( i->dest == dest && i->mask == mask ) {
            if ( i->gateway == ip && add) return;
            deleteFromTable(i);
            if ( add ) break; else return;
        }
    addToTable( dest , mask , ip , ip , 0 , connectMode );
}

/*!
  @return строчка описывающая источник записи.
*/
QString routeRecord::modeString() const
{
    switch ( mode ) {
        case routeModel::staticMode : return QObject::trUtf8("Статическая");
        case routeModel::ripMode : return QObject::trUtf8("RIP");
        case routeModel::connectMode : return QObject::trUtf8("Подключена");
    }
    return QString();
}
//----------------------------------------------------------------

bool routeModel::isConnectedMode(QModelIndex curr)
{
    return (table.at(curr.row())->modeString() != trUtf8("Подключена"));
}

/*!
  * Находит в таблице измененую запись.
  * @return указатель на измененую запись, 0 - если такой записи нет.
  */
routeRecord* routeModel::changedRecord()
{
    foreach ( routeRecord *i , table )
        if ( i->change == changed ) return i;
    return 0;
}
//---------------------------------------------------------------------
/*!
  * Обновляет содержиое всех записей увеличивая время жизни.
  */
void routeModel::update()
{
    foreach ( routeRecord *i , table )
        if ( i->mode != connectMode && i->mode != staticMode ) i->time++;
}
//-----------------------------------------------------------------------
/*!
  * Удаляет из таблицы записи время жизни которых подошло к концу.
  * @param time - время жизни достигнув которого запись считается устаревшей.
  */
void routeModel::deleteOldRecord(int time)
{
    foreach ( routeRecord *i , table )
        if ( i->time >= time ) deleteFromTable(i);
}
