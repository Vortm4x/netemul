#include "hubproperty.h"
#include "hubdevice.h"
#include "ipedit.h"
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>

hubProperty::hubProperty()
{
    QVBoxLayout *all = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *t = new QLabel(trUtf8("Количество портов: "));
    cb_count = new QComboBox;
    QStringList s;
    s << "4" << "5" << "6" << "8" << "12" << "16" << "24" << "48";
    cb_count->addItems(s);
    connect( cb_count , SIGNAL(currentIndexChanged(int)) ,SLOT(applyEnable()) );
    temp->addWidget(t);
    temp->addWidget(cb_count);
    all->addLayout(temp);
    chk_manual = new QCheckBox(trUtf8("Управление через SNMP: "));
    connect( chk_manual , SIGNAL(clicked(bool)) , SLOT(check(bool)));
    all->addWidget(chk_manual);

    temp = new QHBoxLayout;
    lb_mac = new QLabel(trUtf8("Mac-адрес: "));
    le_mac = new QLineEdit;
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    temp->addWidget(lb_mac);
    temp->addWidget(le_mac);
    connect( le_mac , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));
    all->addLayout(temp);
    le_ip = new ipEdit(trUtf8("Ip-адрес: "));
    all->addWidget(le_ip);
    connect( le_ip , SIGNAL(textChanged(QString)) , SLOT(applyEnable()) );

    le_mask = new ipEdit(trUtf8("Маска подсети: "));
    all->addWidget(le_mask);
    connect( le_mask , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));

    lb_statics= new QLabel;
    all->addWidget(lb_statics);
    all->addStretch(1);
    te_text = new QPlainTextEdit;
    connect( te_text , SIGNAL(textChanged()) , SLOT(applyEnable()) );
    te_text->setFixedHeight(100);
    te_text->setMaximumBlockCount(5);
    all->addWidget( new QLabel(trUtf8("Пояснения:")));
    all->addWidget(te_text);
    all->addStretch(1);
    all->addLayout(lay);
    setLayout(all);
}

hubProperty::~hubProperty()
{

}

void hubProperty::check(bool p)
{
    lb_mac->setVisible(p);
    le_mac->setVisible(p);
    le_ip->setVisible(p);
    le_mask->setVisible(p);
    lb_statics->setVisible(p);
    applyEnable();
}

void hubProperty::setHub(hubSetting *s)
{
    st = s;
    cb_count->setCurrentIndex( cb_count->findText( QString::number(st->socketsCount() ) ));
    le_mac->setText( st->snmpMac() );
    le_ip->setText( st->snmpIp() );
    le_mask->setText( st->snmpMask() );
    lb_statics->setText( st->statics() );
    chk_manual->setChecked( st->isManual() );
    te_text->setPlainText( st->note() );
    btn_apply->setEnabled(false);
}

void hubProperty::apply()
{
    int t = st->socketsCount();
    if ( t != cb_count->currentText().toInt() )
        if ( !st->setSocketsCount( cb_count->currentText().toInt() ) ) {
            QMessageBox::warning(this,trUtf8("Error"),
            trUtf8("Для изменения количества портов отсоедините все провода.") , QMessageBox::Ok , QMessageBox::Ok);
            cb_count->setCurrentIndex( cb_count->findText( QString::number(t) ) );
            return;
        }
    st->setManual( chk_manual->isChecked() );
    st->setMac(le_mac->text());
    st->setIp(le_ip->text());
    st->setIp(le_mask->text());
    st->setNote( te_text->toPlainText() );
    if ( sender() == btn_ok ) accept();
}

