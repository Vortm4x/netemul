#include "senddialog.h"
#include "device.h"
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QSlider>
#include <QSpinBox>
#include <QBoxLayout>
#include <QtDebug>
#include <QCheckBox>
#include <QMessageBox>

sendDialog::sendDialog(aim cur,device* t)
{
    myState = cur;
    myDevice = t->contentDevice();
    setWindowTitle(trUtf8("Отправка"));
    all = new QVBoxLayout;
    prepare();
    QHBoxLayout *temp = new QHBoxLayout;
    temp->addWidget(okButton);
    temp->addWidget(cancelButton);
    all->addLayout(temp);
    setLayout(all);
    connect( okButton , SIGNAL(clicked()) , this , SLOT(checkAccept()));
    connect( cancelButton , SIGNAL(clicked()) , this , SLOT(reject()));
}

void sendDialog::checkSelected(int cur)
{
    okButton->setEnabled( cur >= 0 );
}

void sendDialog::checkAccept()
{
    if (myState == sender) {
        mySize = sizeSlider->value();
        myBroadcast = check->checkState();
        if ( rtn_tcp->isChecked() ) myProtocol = TCP;
        else myProtocol = UDP;
    } else {
        myDest = myDevice->nameToIp( list->currentItem()->text() );
    }
    accept();
}

void sendDialog::prepare()
{
    cancelButton = new QPushButton(QIcon(":/im/images/not.png"),trUtf8("Отмена"));
    if ( myState == sender ) {
        sizeSlider = new QSlider(Qt::Horizontal);
        sizeCaption = new QLabel(trUtf8("Укажите размер в килобайтах"));
        sizeBox = new QSpinBox;
        sizeSlider->setRange(1,1000);
        sizeBox->setRange(1,1000);
        sizeBox->setSuffix(trUtf8(" KB"));
        connect( sizeBox, SIGNAL(valueChanged(int)),sizeSlider, SLOT(setValue(int)));
        connect(sizeSlider , SIGNAL(valueChanged(int)) ,sizeBox, SLOT(setValue(int)));
        sizeBox->setValue(50);
        check = new QCheckBox(trUtf8("Широковещательный"));
        check->setChecked(false);
        check->setEnabled(false);
        okButton = new QPushButton(QIcon(":/im/images/ok.png"),trUtf8("Далее"));
        QGroupBox *box = new QGroupBox(trUtf8("Выбирете протокол:"));
        QVBoxLayout *btn = new QVBoxLayout;
        rtn_udp = new QRadioButton(trUtf8("UDP"));
        rtn_udp->setChecked(true);
        rtn_tcp = new QRadioButton(trUtf8("TCP"));
        rtn_tcp->setEnabled(false);
        btn->addWidget(rtn_udp);
        btn->addWidget(rtn_tcp);
        box->setLayout(btn);
        all->addWidget(box);
        all->addWidget(sizeCaption);
        QHBoxLayout *temp = new QHBoxLayout;
        temp->addWidget(sizeSlider);
        temp->addWidget(sizeBox);
        all->addLayout(temp);
        all->addWidget(check);
        all->addStretch(1);
    }
    else {
        list = new QListWidget;
        QStringList portList = myDevice->sockets();
        foreach ( QString i , portList)
            if ( myDevice->isConnectSocket(i) ) {
                QListWidgetItem *temp = new QListWidgetItem( QIcon(":/im/images/ok.png") , i);
                list->addItem(temp);
            }
        caption = new QLabel(trUtf8("Укажите адаптер приемника"));
        okButton = new QPushButton(QIcon(":/im/images/ok.png"),trUtf8("Отправить"));
        if (list->count()) {
            okButton->setEnabled(true);
            list->setCurrentRow(0);
        }
        else okButton->setEnabled(false);
        all->addWidget(caption);
        all->addWidget(list);
        connect( list , SIGNAL(currentRowChanged(int)) ,SLOT(checkSelected(int)));
    }
}

