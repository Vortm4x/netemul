#ifndef DEVICEPORT_H
#define DEVICEPORT_H

class cableDev;
class frame;

class devicePort
{
public:
    devicePort();
    void setNum(int) { }
    bool isConnect() { return false; }
    bool isCableConnect(const cableDev*) { return true; }
    void setConnect(bool,cableDev*) { }
    void pushToSend(frame*) { }
};

#endif // DEVICEPORT_H
