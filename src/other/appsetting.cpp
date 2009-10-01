/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include "appsetting.h"

int appSetting::_defaultComputerCount = 0;
int appSetting::_defaultHubCount = 0;
int appSetting::_defaultRouterCount = 0;
int appSetting::_defaultSwitchCount = 0;
int appSetting::_ttlArp = 0;
int appSetting::_ttlMac = 0;
bool appSetting::_defaultHubManual = false;
bool appSetting::_defaultSwitchManual = false;
int appSetting::_speed = 100;
int appSetting::_language = 0;
int appSetting::_sendingNum = 1;
int appSetting::_waitingTime = 45;
int appSetting::_arpResponceTime = 45;
bool appSetting::_hasOpengl = false;
QString appSetting::_scriptPath = "";
QTranslator* appSetting::mas[LANGUAGE_COUNT];

appSetting::appSetting()
{
}

void appSetting::defaultNums()
{
    _defaultComputerCount = 1;
    _defaultHubCount = 4;
    _defaultSwitchCount = 4;
    _defaultRouterCount = 4;
    _ttlArp = 1200;
    _arpResponceTime = 45;
    _ttlMac = 300;
    _sendingNum = 1;
    _waitingTime = 45;
    _speed = 100;
    _language = 0;
    _hasOpengl = false;
    for ( int i = 1 ; i < LANGUAGE_COUNT ; i++)
        QCoreApplication::removeTranslator(mas[i]);
}

void appSetting::readSetting()
{
    QString translationsPath(TRANSLATIONS_PATH);
    for ( int i = 0 ; i < LANGUAGE_COUNT ; i++ ) mas[i] = new QTranslator;
    mas[1]->load("netemul_ru" , translationsPath);
    mas[2]->load("netemul_pt_BR" , translationsPath);
    mas[3]->load("netemul_es" , translationsPath);
    QSettings setting("FROST","netemul");
    _defaultComputerCount = setting.value("computer/socketCount",1).toInt() ;
    _defaultHubCount = setting.value("hub/socketCount",4).toInt() ;
    _defaultSwitchCount = setting.value("switch/socketCount",4).toInt() ;
    _defaultRouterCount = setting.value("router/socketCount",4).toInt() ;
    _defaultHubManual = setting.value("hub/manual",false).toBool() ;
    _arpResponceTime = setting.value("netcard/responcetime",45).toInt();
    _defaultSwitchManual = setting.value("switch/manual",false).toBool() ;
    _scriptPath = setting.value("main/scriptPath",SCRIPT_PATH).toString();
    _ttlArp = setting.value("ttl/Arp",1200).toInt() ;
    _ttlMac = setting.value("ttl/Mac",300).toInt();
    _speed = setting.value("main/speed",100).toInt();
    setLanguage( setting.value("main/language",0).toInt() );
    _sendingNum = setting.value("tcp/sendingNumber",1).toInt();
    _waitingTime = setting.value("tcp/waitingTime",45).toInt();
    _hasOpengl = setting.value("main/opengl",false).toBool();
}

void appSetting::writeSetting()
{
    for ( int i = 0 ; i < LANGUAGE_COUNT ; i++ ) delete mas[i];
    QSettings setting("FROST","netemul");
    setting.setValue("computer/socketCount" , _defaultComputerCount );
    setting.setValue("hub/socketCount" , _defaultHubCount );
    setting.setValue("switch/socketCount" , _defaultSwitchCount );
    setting.setValue("router/socketCount" , _defaultRouterCount );
    setting.setValue("hub/manual", _defaultHubManual );
    setting.setValue("switch/manual", _defaultSwitchManual );
    setting.setValue("netcard/responcetime",_arpResponceTime);
    setting.setValue("ttl/Arp", _ttlArp );
    setting.setValue("ttl/Mac", _ttlMac );
    setting.setValue("main/speed",_speed);
    setting.setValue("main/language", _language);
    setting.setValue("main/scriptPath",_scriptPath);
    setting.setValue("tcp/sendingNumber",_sendingNum);
    setting.setValue("tcp/waitingTime",_waitingTime);
    setting.setValue("main/opengl", _hasOpengl );
}

void appSetting::setLanguage(int n)
{
    if ( n == _language ) return;
    _language = n;
    for ( int i = 1 ; i < LANGUAGE_COUNT ; i++)
        if ( i != n ) QCoreApplication::removeTranslator(mas[i]);
    QCoreApplication::installTranslator(mas[n]);
}
