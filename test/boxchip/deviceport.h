#ifndef DEVICEPORT_H
#define DEVICEPORT_H

class frame;
class cableDev;

class devicePort
{
public:
    devicePort();
    void setNum(int) { }
    bool isBusy() const { return false; }
    bool isConnect() { return false; }
    bool isCableConnect(const cableDev*) { return true; }
    void setConnect(bool,cableDev*) { }
    void pushToSend(frame&) { }
    void queueEvent() { }
    bool hasReceive() { return false; }
};

#endif // DEVICEPORT_H
