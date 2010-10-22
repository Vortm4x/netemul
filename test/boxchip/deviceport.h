#ifndef DEVICEPORT_H
#define DEVICEPORT_H

class frame;
class Cable;

class DevicePort
{
public:
    DevicePort();
    DevicePort(int) { }
    void setNum(int) { }
    bool isBusy() const { return false; }
    bool isConnect() { return false; }
    bool isCableConnect(const Cable*) { return true; }
    void setConnect(bool,Cable*) { }
    void pushToSend(frame&) { }
    void queueEvent() { }
    bool hasReceive() { return false; }
    void setShared(bool) const { }
    bool isCableBusy() const { return false; }
    void setChecked(bool) { }
    int trafficDigit() const { return 0; }
};

#endif // DEVICEPORT_H
