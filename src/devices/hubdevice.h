#ifndef HUBDEVICE_H
#define HUBDEVICE_H

#include "boxdevice.h"

class frame;

class hubDevice : public boxDevice
{
    Q_OBJECT
public:
    int type() const { return hubDev; }
    enum { hubDev = 4 };
    hubDevice(int c = 0);
    ~hubDevice();
    void dialog();
    void detectCollision();
    QString deviceName() const { return "hub"; }
    friend class hubSetting;
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    quint32 collision;
};

class hubSetting : public boxSetting
{
public:
    hubSetting(hubDevice *d) : boxSetting(d) , hd(d) { }
    quint32 collisions() const { return hd->collision; }
    void reset() { boxSetting::reset(); hd->collision = 0; }
private:
    hubDevice *hd;
};


#endif // HUBDEVICE_H
