#include "ipeditdesignerplugin.h"
#include "ipedit.h"

#include <QtPlugin>

ipEditDesignerPlugin::ipEditDesignerPlugin(QObject *parent) : QObject(parent)
{

}

QString ipEditDesignerPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"ipEdit\" name=\"IpEdit\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>380</width>\n"
           "    <height>35</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>Ip address</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>The field for ip-address.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";

}

QWidget* ipEditDesignerPlugin::createWidget(QWidget *parent)
{
    return new IpEdit(parent,tr("Enter text: "));
}

Q_EXPORT_PLUGIN2(ipeditdesignerplugin , ipEditDesignerPlugin )
