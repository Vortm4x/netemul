#include "appsetting.h"
#include <QSettings>

int appSetting::_defaultComputerCount = 0;
int appSetting::_defaultHubCount = 0;
int appSetting::_defaultRouterCount = 0;
int appSetting::_defaultSwitchCount = 0;
int appSetting::_ttlArp = 0;
int appSetting::_ttlMac = 0;
bool appSetting::_defaultHubManual = false;
bool appSetting::_defaultSwitchManual = false;
int appSetting::_speed = 100;

appSetting::appSetting()
{
}

void appSetting::readSetting()
{
    QSettings setting("FROST","netemul");
    _defaultComputerCount = setting.value("computer/socketCount",1).toInt() ;
    _defaultHubCount = setting.value("hub/socketCount",4).toInt() ;
    _defaultSwitchCount = setting.value("switch/socketCount",4).toInt() ;
    _defaultRouterCount = setting.value("router/socketCount",4).toInt() ;
    _defaultHubManual = setting.value("hub/manual",false).toBool() ;
    _defaultSwitchManual = setting.value("switch/manual",false).toBool() ;
    _ttlArp = setting.value("ttl/Arp",1200).toInt() ;
    _ttlMac = setting.value("ttl/Mac",300).toInt();
    _speed = setting.value("main/speed",100).toInt();
}

void appSetting::writeSetting()
{
    QSettings setting("FROST","netemul");
    setting.setValue("computer/socketCount" , _defaultComputerCount );
    setting.setValue("hub/socketCount" , _defaultHubCount );
    setting.setValue("switch/socketCount" , _defaultSwitchCount );
    setting.setValue("router/socketCount" , _defaultRouterCount );
    setting.setValue("hub/manual", _defaultHubManual );
    setting.setValue("switch/manual", _defaultSwitchManual );
    setting.setValue("ttl/Arp", _ttlArp );
    setting.setValue("ttl/Mac", _ttlMac );
    setting.setValue("main/speed",_speed);
}
