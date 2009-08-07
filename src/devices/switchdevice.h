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
    Q_OBJECT
public:
    friend class switchTableSetting;
    enum { switchDev = 5 };
    int type() const { return switchDev; }
    switchDevice(int c = 0); //Пока конструктор и прорисовка
    ~switchDevice();
    void updateMac(int u);
    void dialog();
    void tableDialog();
    bool hasTable() const { return true; }
    switchChip* concreteChip();
    void secondTimerEvent();
    QString tableName() const { return tr("Table switching"); }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

class switchTableSetting {
public:
    switchTableSetting(switchDevice *s) : sw(s) { }
    switchModel* switchTable();
    int socketsCount() const { return sw->chip->socketsCount(); }
    void removeFromTable(int row);
    devicePort* socket(const QString &s) { return sw->chip->socket(s); }
private:
    switchDevice *sw;
};


#endif // SWITCHDEVICE_H
