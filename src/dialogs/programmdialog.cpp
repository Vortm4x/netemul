#include "programmdialog.h"
#include "smartdevice.h"
#include "programm.h"
#include "installdialog.h"
#include <QCheckBox>

programmDialog::programmDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

/*!
  Обновляет список программ.
*/
void programmDialog::updateList()
{
    list->clear();
    foreach ( programm i, s->programms() ){
        QListWidgetItem *item = new QListWidgetItem(i->name());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        if ( i->isEnable() ) item->setCheckState(Qt::Checked);
        else item->setCheckState(Qt::Unchecked);
        list->addItem(item);
    }
}
//-------------------------------------------------
void programmDialog::setDevice( smartDevice *d )
{
    s = d;
    updateList();
}
/*!
  Слот, вызываемый при нажатии на кнопку Ok,
  выполняет все принятые изменения, закрывает диалог.
*/
void programmDialog::apply()
{
    for ( int i = 0; i < list->count(); i++) {
        QListWidgetItem *n = list->item(i);
        if (n->checkState() == Qt::Checked ) s->programmAt( n->text())->setEnable(true);
        else s->programmAt( n->text())->setEnable(false);
    }
    accept();
}
//----------------------------------------------------
/*!
  Слот вызывает диалог установки программ,
  обновляет список установленных программ.
*/
void programmDialog::add()
{
    installDialog *d = new installDialog;
    d->setDevice( s );
    d->exec();
    btn_ok->setFocus();
    updateList();
}
//-----------------------------------------------------
/*!
  Слот удаляет выделенную программу.
*/
void programmDialog::remove()
{
    QListWidgetItem *w = list->currentItem();
    s->removeProgramm( s->programmAt(w->text()) );
    updateList();
}
//-----------------------------------------------------
void programmDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
