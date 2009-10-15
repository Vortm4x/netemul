#ifndef IPEDITDESIGNERPLUGIN_H
#define IPEDITDESIGNERPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class ipEditDesignerPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    ipEditDesignerPlugin(QObject *parent = 0);
    bool isContainer() const { return false; }
    bool isInitialized() const { return false; }
    QIcon icon() const { return QIcon(); }
    QString domXml() const;
    QString group() const { return "Input Widgets"; }
    QString includeFile() const { return "ipedit.h"; }
    QString name() const { return "ipEdit" ; }
    QString toolTip() const { return tr("The field for ip-address"); }
    QString whatsThis() const { return tr("The field for ip-address"); }
    QWidget* createWidget(QWidget *parent);
};

#endif // IPEDITDESIGNERPLUGIN_H
