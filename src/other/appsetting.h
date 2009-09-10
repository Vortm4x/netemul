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
#ifndef APPSETTING_H
#define APPSETTING_H

#include <QString>
class QTranslator;

static const int LANGUAGE_COUNT = 4;
static const int main_digit = 10000;
static const int little_digit = 20;

class appSetting
{
public:
    static int defaultComputerCount() { return _defaultComputerCount; }
    static void setDefaultComputerCount(int n) { _defaultComputerCount = n; }
    static int defaultHubCount() { return _defaultHubCount; }
    static void setDefaultHubCount(int n) { _defaultHubCount = n; }
    static int defaultSwitchCount() { return _defaultSwitchCount; }
    static void setDefaultSwitchCount(int n) { _defaultSwitchCount = n; }
    static int defaultRouterCount() { return _defaultRouterCount; }
    static void setDefaultRouterCount(int n) { _defaultRouterCount = n; }
    static void setDefaultHubManual(bool b) { _defaultHubManual = b; }
    static bool defaultHubManual() { return _defaultHubManual; }
    static void setDefaultSwitchManual(bool b) { _defaultSwitchManual = b; }
    static bool defaultSwitchManual() { return _defaultSwitchManual; }
    static void setScriptPath(const QString path) { _scriptPath = path; }
    static QString scriptPath() { return _scriptPath; }
    static void setTtlArp(int n) { _ttlArp = n; }
    static int ttlArp() { return _ttlArp; }
    static void setArpResponceTime(int n) { _arpResponceTime = n; }
    static int arpResponceTime() { return _arpResponceTime; }
    static void setTtlMac(int n) { _ttlMac = n; }
    static int ttlMac() { return _ttlMac; }
    static void setAnimateSpeed(int n) { _speed = main_digit / (n*little_digit); }
    static int animateSpeed() { return main_digit / _speed / little_digit; }
    static int realAnimateSpeed() { return _speed; }
    static void setLanguage(int n);
    static int language() { return _language; }
    static void readSetting();
    static void writeSetting();
private:
    static QTranslator *mas[LANGUAGE_COUNT];
    static int _defaultComputerCount;
    static int _defaultHubCount;
    static int _defaultRouterCount;
    static int _defaultSwitchCount;
    static int _ttlArp;
    static int _ttlMac;
    static int _speed;
    static int _language;
    static bool _defaultHubManual;
    static bool _defaultSwitchManual;
    static int _arpResponceTime;
    static QString _scriptPath;
protected:
    appSetting();
    ~appSetting();
};



#endif // APPSETTING_H
