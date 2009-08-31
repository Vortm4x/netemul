#ifndef APPSETTING_H
#define APPSETTING_H

#include <QString>
class QTranslator;

static const int LANGUAGE_COUNT = 4;

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
    static void setAnimateSpeed(int n) { _speed = n; }
    static int animateSpeed() { return _speed; }
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
