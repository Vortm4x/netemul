#include "settingdialog.h"
#include "appsetting.h"

settingDialog::settingDialog()
{
    setupUi(this);

    sp_arpResponce->setValue( appSetting::arpResponceTime() );

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

    int s = main_digit / appSetting::animateSpeed() / little_digit;
    sl_speed->setValue(s);

    languageBox->setCurrentIndex( appSetting::language() );
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
    appSetting::setArpResponceTime( sp_arpResponce->value() );
    appSetting::setTtlMac( sp_ttlMac->value() );
    int s = main_digit / (sl_speed->value()*little_digit);
    appSetting::setAnimateSpeed(s);
    appSetting::setLanguage( languageBox->currentIndex() );
    if ( sender() == btn_ok ) accept();
}

void settingDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        languageBox->setCurrentIndex( appSetting::language() );
        listWidget->setSpacing( listWidget->spacing() );
        break;
    default:
        break;
    }
}

