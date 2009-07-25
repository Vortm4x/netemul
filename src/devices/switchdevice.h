#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H

#include "boxdevice.h"
#include "macaddress.h"

class frame;
class switchModel;
class switchTableSetting;
class switchChip;

// Пока такое коротенькое объявление для свитча
// Все его комментарии действительны для других устройств
class switchDevice : public boxDevice
{
public:
    friend class switchTableSetting;
    enum { switchDev = 5 };
    int type() const { return switchDev; }
    switchDevice(int c = 4); //Пока конструктор и прорисовка
    ~switchDevice();
    void updateMac(int u);
    void dialog();
    void tableDialog();
    bool hasTable() const { return true; }
    switchChip* concreteChip();
    QString tableName() const { return trUtf8("Таблица коммутации"); }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

class switchTableSetting {
public:
    switchTableSetting(switchDevice *s) : sw(s) { }
    switchModel* switchTable();
    int socketsCount() const { return sw->chip->socketsCount(); }
    void addToTable(const macAddress &m , const QString &p, int mode , int time);
    void removeFromTable(int row);
private:
    switchDevice *sw;
};


#endif // SWITCHDEVICE_H
