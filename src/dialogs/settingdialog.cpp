#include "settingdialog.h"
#include "appsetting.h"

settingDialog::settingDialog()
{
    setupUi(this);

    sp_ttlArp->setValue( appSetting::ttlArp() );

    computerComboBox->setCurrentIndex( computerComboBox->findText(QString::number(
            appSetting::defaultComputerCount() ) ) );

    hubComboBox->setCurrentIndex( hubComboBox->findText(
                                  QString::number( appSetting::defaultHubCount() ) ) );

    switchComboBox->setCurrentIndex( switchComboBox->findText(
                       QString::number( appSetting::defaultSwitchCount() ) ) );

    sp_ttlMac->setValue( appSetting::ttlMac() );

    routerComboBox->setCurrentIndex( routerComboBox->findText(QString::number(
            appSetting::defaultRouterCount() ) ) );

    int s = 10000 / appSetting::animateSpeed() / 20;
    sl_speed->setValue(s);
}

void settingDialog::applyDisable()
{
    btn_apply->setEnabled(false);
}

void settingDialog::applyEnable()
{
    btn_apply->setEnabled(true);
}

void settingDialog::apply()
{
    appSetting::setDefaultComputerCount( computerComboBox->currentText().toInt() );
    appSetting::setDefaultHubCount(  hubComboBox->currentText().toInt() );
    appSetting::setDefaultSwitchCount(  switchComboBox->currentText().toInt() );
    appSetting::setDefaultRouterCount(  routerComboBox->currentText().toInt() );
    appSetting::setTtlArp( sp_ttlArp->value() );
    appSetting::setTtlMac( sp_ttlMac->value() );
    int s = 10000 / (sl_speed->value()*20);
    appSetting::setAnimateSpeed(s);
    if ( sender() == btn_ok ) accept();
}

void settingDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

