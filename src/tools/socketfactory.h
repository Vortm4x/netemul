#ifndef SOCKETFACTORY_H
#define SOCKETFACTORY_H

#include <QString>

class SmartDevice;
class AbstractSocket;

class SocketFactory
{
    SocketFactory();
public:
    enum { UDP = 0 , TCP = 1 };
    static AbstractSocket* getTempSocket(SmartDevice *dev,quint16 port , int type);
    static AbstractSocket* getTCPSocket(SmartDevice *dev,quint16 port);
};

#endif // SOCKETFACTORY_H
