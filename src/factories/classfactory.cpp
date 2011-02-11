#include "classfactory.h"
#include "device.h"
#include "computer.h"
#include "switchdevice.h"
#include "routerdevice.h"
#include "hubdevice.h"
#include "hubchip.h"
#include "switchchip.h"
#include "routemodel.h"
#include "switchmodel.h"
#include "ripprogram.h"
#include "arpmodel.h"
#include "dhcpdaemon.h"
#include "dhcpclientprogram.h"
#include "dhcpserverprogram.h"
#include "spoofingprogram.h"
#include "statistics.h"
#include "textitem.h"

#include <QtDebug>

ClassFactory::ClassFactory()
{
}

QObject* ClassFactory::createInstance(const QString &str, QObject *parent)
{    
    if ( str == "Device" ) {
        return new Device(parent);
    } else if ( str == "Computer" ) {
        return new Computer(parent);
    } else if ( str == "RouterDevice" ) {
        return new RouterDevice(parent);
    } else if ( str == "SwitchDevice" ) {
        return new SwitchDevice(parent);
    } else if ( str == "HubDevice" ) {
        return new HubDevice(parent);
    } else if ( str == "Interface" ) {
        return new Interface(parent);
    } else if ( str == "SwitchChip" ) {
        return new SwitchChip(parent);
    } else if ( str == "HubChip" ) {
        return new HubChip(parent);
    } else if ( str == "RouteModel" ) {
        return new RouteModel(parent);
    } else if ( str == "VirtualNetwork" ) {
        return new VirtualNetwork(parent);
    } else if ( str == "MacRecordObject" ) {
        return new MacRecordObject(parent);
    } else if ( str == "RipProgram" ) {
        return new RipProgram(parent);
    } else if ( str == "SpoofingProgram" ) {
        return new SpoofingProgram(parent);
    } else if ( str == "DhcpClientProgram" ) {
        return new DhcpClientProgram(parent);
    } else if ( str == "InterfaceStateObject" ) {
        return new InterfaceStateObject(parent);
    } else if ( str == "DhcpServerProgram" ) {
        return new DhcpServerProgram(parent);
    } else if ( str == "StatisticsObject" ) {
        return new StatisticsObject(parent);
    } else if ( str == "TextItem" ) {
        return new TextItem(parent);
    } else if ( str == "RouteRecordObject" ) {
        return new RouteRecordObject(parent);
    } else if ( str == "ArpModel" ) {
        return new ArpModel(parent);
    } else if ( str == "ArpRecordObject" ) {
        return new ArpRecordObject(parent);
    } else if ( str == "DhcpDaemon" ) {
        return new DhcpDaemon(parent);
    } else {
        return 0;
    }
}
