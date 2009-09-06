#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QPlainTextEdit>
#include "switchproperty.h"
#include "switchdevice.h"
#include "ipedit.h"

switchProperty::switchProperty()
{
    QVBoxLayout *all = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *t = new QLabel(tr("Number of ports: "));
    cb_count = new QComboBox;
    QStringList s;
    s << "4" << "5" << "8" << "12" <<"16"<< "24"<<"32" << "48";
    cb_count->addItems(s);
    connect( cb_count , SIGNAL(currentIndexChanged(int)) ,SLOT(applyEnable()) );
    temp->addWidget(t);
    temp->addWidget(cb_count);
    all->addLayout(temp);

    temp = new QHBoxLayout;
    lb_mac = new QLabel(tr("Mac-address: "));
    le_mac = new QLineEdit;
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    temp->addWidget(lb_mac);
    temp->addWidget(le_mac);
    all->addLayout(temp);
    connect( le_mac , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));

    le_ip = new ipEdit(tr("Ip-address: "));
    all->addWidget(le_ip);
    connect( le_ip , SIGNAL(textChanged(QString)) , SLOT(applyEnable()) );

    le_mask = new ipEdit(tr("Mask: "));
    all->addWidget(le_mask);
    connect( le_mask , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));

    lb_statics = new QLabel;
    all->addWidget(lb_statics);

    QPushButton *btn_reset = new QPushButton(QIcon(":/im/images/refresh.png"),tr("Reset statistics"));
    connect( btn_reset , SIGNAL(clicked()), SLOT(reset()) );
    all->addWidget(btn_reset , 0 , Qt::AlignRight );

    te_text = new QPlainTextEdit;
    connect( te_text , SIGNAL(textChanged()) , SLOT(applyEnable()) );
    te_text->setFixedHeight(100);
    te_text->setMaximumBlockCount(5);
    all->addWidget( new QLabel(tr("Description:")));
    all->addWidget(te_text);
    all->addStretch(1);
    all->addLayout(lay);
    setLayout(all);
}

void switchProperty::setSwitch(boxSetting *d)
{
    sw = d;
    cb_count->setCurrentIndex( cb_count->findText( QString::number( d->socketsCount() ) ) );
    le_mac->setText( d->snmpMac() );
    le_ip->setText( d->snmpIp() );
    le_mask->setText( d->snmpMask() );
    lb_statics->setText( d->statics() );
    te_text->setPlainText(  d->note() );
    btn_apply->setEnabled(false);
}

void switchProperty::reset()
{
    sw->reset();
    setSwitch(sw);
}

void switchProperty::apply()
{
    int t = sw->socketsCount();
    if ( t != cb_count->currentText().toInt() )
        if ( !sw->setSocketsCount( cb_count->currentText().toInt() ) ) {
            QMessageBox::warning(this,tr("Error"),
            tr("First, remove the cables!") , QMessageBox::Ok , QMessageBox::Ok);
            cb_count->setCurrentIndex( cb_count->findText( QString::number(t) ) );
            return;
        }
    sw->setMac(le_mac->text());
    sw->setIp(le_ip->text());
    sw->setMask(le_mask->text());
    sw->setNote( te_text->toPlainText() );
    if ( sender() == btn_ok ) accept();
}



