#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H

#include "boxdevice.h"
#include "macaddress.h"
class devicePort;
class frame;

struct macRecord {
    macAddress mac;
    devicePort *port;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == 0 ) return QObject::trUtf8("Статическая");
        else return QObject::trUtf8("Динамическая");
    }
};

// Пока такое коротенькое объявление для свитча
// Все его комментарии действительны для других устройств
class switchDevice : public boxDevice
{
public:
    enum { switchDev = 5, staticMode = 0 , dinamicMode = 1 };
    int type() const { return switchDev; }
    switchDevice(int c = 4); //Пока конструктор и прорисовка
    ~switchDevice();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void receiveEvent(frame *fr,devicePort *sender);
    devicePort* addInterface(QString str,int t);
    QList<macRecord*> switchTable() { return mySwitchTable; }
    macRecord* addToTable( macAddress mac , QString name , int mode , int time);
    void deleteFromTable( macAddress mac );
    void updateMac(int u);
    void dialog();
    void showTable();
    bool hasTable() const { return true; }
private:
    QList<macRecord*> mySwitchTable;
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};


#endif // SWITCHDEVICE_H
