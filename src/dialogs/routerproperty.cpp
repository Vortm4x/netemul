#include "routerproperty.h"
#include "routerdevice.h"
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPlainTextEdit>

/*!
  Создает интерфейс диалога.
*/
routerProperty::routerProperty()
{
    QVBoxLayout *all = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    temp->addWidget( new QLabel(tr("Number of ports: ") ) );
    cb_count = new QComboBox;
    connect( cb_count , SIGNAL(currentIndexChanged(int)) , SLOT(applyEnable()) );
    QStringList t;
    t << "2" << "4" << "5"<< "7" << "8" << "9";
    cb_count->addItems(t);
    temp->addWidget(cb_count);
    all->addLayout(temp);
    cb_route = new QCheckBox( tr("Enable routing") );
    connect( cb_route , SIGNAL(toggled(bool)) , SLOT(applyEnable()) );
    all->addWidget( cb_route);
    te_text = new QPlainTextEdit;
    connect( te_text , SIGNAL(textChanged()) , SLOT(applyEnable()) );
    te_text->setFixedHeight(100);
    te_text->setMaximumBlockCount(5);
    all->addWidget( new QLabel(trUtf8("Description:")));
    all->addWidget(te_text);
    all->addStretch(1);
    all->addLayout(lay);
    setLayout(all);
}
//----------------------------------------------------------------
/*!
 Задает диалогу устройство для работы.
 @param r - указатель на роутер.
*/
void routerProperty::setRouter(routerSetting *r)
{
    rt = r;
    cb_route->setChecked(r->isRouter());
    cb_count->setCurrentIndex( cb_count->findText( QString::number( rt->socketsCount() ) ) );
    te_text->setPlainText( rt->note() );
    btn_apply->setEnabled(false);
}
//-----------------------------------------------------------------
void routerProperty::apply()
{
    rt->setRouter( cb_route->isChecked() );
    rt->setNote( te_text->toPlainText() );
    int m = cb_count->currentText().toInt();
    if ( m != rt->socketsCount() ) rt->setSocketsCount( m );
    cb_count->setCurrentIndex( cb_count->findText( QString::number( rt->socketsCount() ) ) );
    if ( sender() == btn_ok ) accept();
}
