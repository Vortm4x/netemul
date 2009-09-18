/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#include "settingdialog.h"
#include "appsetting.h"

settingDialog::settingDialog()
{
    setupUi(this);
    currentValues();
    setAttribute(Qt::WA_DeleteOnClose);
    btn_apply->setEnabled(false);
}

void settingDialog::currentValues()
{
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

    int s = appSetting::animateSpeed();
    sl_speed->setValue(s);

    languageBox->setCurrentIndex( appSetting::language() );

    sb_send->setValue(appSetting::sendingNum());

    sb_waitingTime->setValue(appSetting::waitingTime());
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
    appSetting::setAnimateSpeed(sl_speed->value());
    appSetting::setLanguage( languageBox->currentIndex() );
    appSetting::setSendingNum(sb_send->value());
    appSetting::setWaitingTime(sb_waitingTime->value());
    btn_apply->setEnabled(false);
    if ( sender() == btn_ok ) accept();
}

void settingDialog::defaultSettings()
{
    appSetting::defaultNums();
    currentValues();
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

