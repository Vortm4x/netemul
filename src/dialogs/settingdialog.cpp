#include "settingdialog.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

settingDialog::settingDialog()
{
    const char *labelArray[COUNT] = { "Общие", "Компьютер", "Концентратор", "Коммутатор", "Маршрутизатор" };
    QString imageArray[COUNT] = {"gadu.png", "laptop.png" , "modem.png" , "switch.png", "router.png"};

    QVBoxLayout *layout = new QVBoxLayout;
    resize(500, 300);
    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListView::IconMode);
    int i;
    for ( i=0 ; i < COUNT ; i++){
        QListWidgetItem *temp = new QListWidgetItem(QIcon(":/im/images/"+imageArray[i]),trUtf8(labelArray[i]));
        listWidget->addItem(temp);
    }
    listWidget->setIconSize(QSize(32,32));
    listWidget->setFixedHeight(70);
    listWidget->setMinimumWidth(COUNT*105);
    listWidget->setSpacing(5);
    listWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
    listWidget->setCurrentRow(0);

    for (i = 0; i < COUNT; i++) {
        widgetLayouts[i] = new QVBoxLayout;
    }

    createGeneral();
    createComputer();
    createHub();
    createSwitch();
    createRouter();

    stackedWidget = new QStackedWidget;
    for (i = 0; i < COUNT; i++){
        widgets[i] = new QWidget;
        stackedWidget->addWidget(widgets[i]);
        widgets[i]->setLayout(widgetLayouts[i]);
    }

    connect( listWidget , SIGNAL(currentRowChanged(int)), stackedWidget ,SLOT(setCurrentIndex(int)));

    btn_apply->setEnabled(false);
    connect(computerComboBox, SIGNAL(currentIndexChanged(int)), SLOT(applyEnable()));
    connect(hubComboBox, SIGNAL(currentIndexChanged(int)), SLOT(applyEnable()));
    connect(hubCheckBox, SIGNAL(clicked()), SLOT(applyEnable()));
    connect(switchComboBox, SIGNAL(currentIndexChanged(int)), SLOT(applyEnable()));
    connect(switchCheckBox, SIGNAL(clicked()), SLOT(applyEnable()));
    connect(routerComboBox, SIGNAL(currentIndexChanged(int)), SLOT(applyEnable()));
    connect(sp_ttlArp, SIGNAL(valueChanged(int)), SLOT(applyEnable()));
    connect(sp_ttlMac, SIGNAL(valueChanged(int)), SLOT(applyEnable()));
    connect(sp_rip, SIGNAL(valueChanged(int)), SLOT(applyEnable()));

    layout->addWidget(listWidget);
    layout->addWidget(stackedWidget);
    layout->addLayout(lay);
    setLayout(layout);
}


//Создаем закладку Общие
void settingDialog::createGeneral()
{
   QLabel *label = new QLabel(trUtf8("Время жизни записей ARP-таблицы"));
   sp_ttlArp = new QSpinBox;
   sp_ttlArp->setFixedWidth(100);
   sp_ttlArp->setRange(10,1200); // Возможное время жизни arp записи.
   sp_ttlArp->setSuffix(trUtf8(" сек"));
   QHBoxLayout *ttl = new QHBoxLayout;
   ttl->addWidget(label);
   ttl->addWidget(sp_ttlArp,0,Qt::AlignLeft);
   widgetLayouts[0]->addLayout(ttl);
   
   widgetLayouts[0]->addStretch(1);
}

//Создаем закладку Компьютер
void settingDialog::createComputer()
{
    QLabel *label = new QLabel(trUtf8("Количество интерфейсов по умолчанию:"));
    widgetLayouts[1]->addWidget(label);
    computerComboBox = new QComboBox;
    int i;
    for ( i = 0; i < 6 ; i++){
        computerComboBox->addItem(QString("%1").arg(i+1));
    }
    computerComboBox->setFixedWidth(50);
    widgetLayouts[1]->addWidget(computerComboBox);
    widgetLayouts[1]->addStretch(1);
}

//Создаем закладку Концентратор
void settingDialog::createHub()
{
    QLabel *label = new QLabel(trUtf8("Количество портов по умолчанию:"));
    widgetLayouts[2]->addWidget(label);
    hubComboBox = new QComboBox;
    unsigned int i;
    int arrayPort[] = {4,5,6,8,12,24,48};
    for ( i = 0; i < sizeof(arrayPort)/sizeof(arrayPort[0]) ; i++){
        hubComboBox->addItem(QString("%1").arg(arrayPort[i]));
    }
    hubComboBox->setFixedWidth(50);

    hubCheckBox = new QCheckBox(trUtf8("Управляемый через SNMP"));
    hubCheckBox->setCheckable(true);

    widgetLayouts[2]->addWidget(hubComboBox);
    widgetLayouts[2]->addWidget(hubCheckBox);
    widgetLayouts[2]->addStretch(1);
}

//Создаем закладку Коммутатор
void settingDialog::createSwitch()
{
    QLabel *label = new QLabel(trUtf8("Количество портов по умолчанию:"));
    widgetLayouts[3]->addWidget(label);
    switchComboBox = new QComboBox;
    unsigned int i;
    int arrayPort[] = {4,5,8,12,16,24,32,48};
    for ( i = 0; i < sizeof(arrayPort)/sizeof(arrayPort[0]) ; i++){
        switchComboBox->addItem(QString("%1").arg(arrayPort[i]));
    }
    switchComboBox->setFixedWidth(50);
    switchCheckBox = new QCheckBox(trUtf8("Управляемый через SNMP"));
    switchCheckBox->setCheckable(true);

    widgetLayouts[3]->addWidget(switchComboBox);
    widgetLayouts[3]->addWidget(switchCheckBox);
    
    label = new QLabel(trUtf8("Время жизни записей таблицы коммутации"));
    sp_ttlMac = new QSpinBox;
    sp_ttlMac->setFixedWidth(100);
    sp_ttlMac->setRange(10,300);
    sp_ttlMac->setValue(300);
    sp_ttlMac->setSuffix(trUtf8(" сек"));
    QHBoxLayout *ttl = new QHBoxLayout;
    ttl->addWidget(label);
    ttl->addWidget(sp_ttlMac,0,Qt::AlignLeft);
    widgetLayouts[3]->addLayout(ttl);
   
    widgetLayouts[3]->addStretch(1);
}

void settingDialog::createRouter()
{
    QLabel *label = new QLabel(trUtf8("Количество портов по умолчанию:"));
    widgetLayouts[4]->addWidget(label);
    routerComboBox = new QComboBox;
    unsigned int i;
    int arrayPort[] = {2,4,5,7,8,9};
    for ( i = 0; i < sizeof(arrayPort)/sizeof(arrayPort[0]) ; i++){
        routerComboBox->addItem(QString("%1").arg(arrayPort[i]));
    }
    routerComboBox->setFixedWidth(50);
    widgetLayouts[4]->addWidget(routerComboBox);
    
    label = new QLabel(trUtf8("Интервал обмена RIP-сообщениями"));
    sp_rip = new QSpinBox;
    sp_rip->setFixedWidth(100);
    sp_rip->setRange(10,30);
    sp_rip->setValue(30);
    sp_rip->setSuffix(trUtf8(" сек"));
    sp_rip->setEnabled(false);
    QHBoxLayout *ttl = new QHBoxLayout;
    ttl->addWidget(label);
    ttl->addWidget(sp_rip,0 ,Qt::AlignLeft);
    widgetLayouts[4]->addLayout(ttl);
    
    widgetLayouts[4]->addStretch(1);
}

void settingDialog::setComputerSockets(int i)
{
    myComputerSockets = i;
    for (i = 0 ; i < 6 ; i++) {
        if (computerComboBox->itemText(i).toInt() == myComputerSockets)  {
            computerComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void settingDialog::setHubSockets(unsigned int i)
{
    myHubSockets = i;
    int arrayPort[] = {4,5,6,8,12,24,48};
    for (i = 0 ; i < sizeof(arrayPort)/sizeof(arrayPort[0]) ; i++) {
        if (hubComboBox->itemText(i).toInt() == myHubSockets)  {
            hubComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void settingDialog::setHubManual(bool b)
{
    myHubManual = b;
    hubCheckBox->setChecked( b );
}

void settingDialog::setSwitchSockets(unsigned int i)
{
    mySwitchSockets = i;
    int arrayPort[] = {4,5,8,12,16,24,32,48};
    for (i = 0 ; i < sizeof(arrayPort)/sizeof(arrayPort[0]) ; i++) {
        if (switchComboBox->itemText(i).toInt() == mySwitchSockets)  {
            switchComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void settingDialog::setSwitchManual(bool b)
{
    mySwitchManual = b;
    switchCheckBox->setChecked(b);
}

void settingDialog::setRouterSockets(unsigned int i)
{
    myRouterSockets = i;
    int arrayPort[] = {1,2,4,5,7,8,9};
    for (i = 0 ; i < sizeof(arrayPort)/sizeof(arrayPort[0]) ; i++) {
        if (routerComboBox->itemText(i).toInt() == myRouterSockets)  {
            routerComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void settingDialog::apply()
{
    myComputerSockets = computerComboBox->currentText().toInt();
    myHubSockets = hubComboBox->currentText().toInt();
    mySwitchSockets = switchComboBox->currentText().toInt();
    myHubManual = hubCheckBox->isChecked();
    mySwitchManual = switchCheckBox->isChecked();
    myRouterSockets = routerComboBox->currentText().toInt();
    myTtlArp = sp_ttlArp->value();
    myTtlMac = sp_ttlMac->value();
    myRip = sp_rip->value();
    emit sendApply();
    if ( sender() == btn_ok ) accept();
}

void settingDialog::setTtlArp(int i)
{
    myTtlArp = i;
    sp_ttlArp->setValue(i);
}

void settingDialog::setTtlMac(int i)
{
    myTtlMac = i;
    sp_ttlMac->setValue(i);
}

void settingDialog::setRip(int i)
{
    myRip = i;
    sp_rip->setValue(i);
}
