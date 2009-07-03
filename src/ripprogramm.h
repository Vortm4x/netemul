#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programm.h"

class smartDevice;
class QByteArray;

class ripProgramm : public programm
{
public:
    ripProgramm(smartDevice *d);
    void execute(QByteArray &b);
    void incTime();
    void sendRip();
private:
    smartDevice *sd;
    int t;
    int inter;
};

#endif // RIPPROGRAMM_H
