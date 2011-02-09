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
}

void settingDialog::currentValues()
{
    sp_arpResponce->setValue( AppSetting::arpResponceTime() );

    sp_ttlArp->setValue( AppSetting::ttlArp() );

    computerComboBox->setCurrentIndex( computerComboBox->findText(QString::number(
            AppSetting::defaultComputerCount() ) ) );

    hubComboBox->setCurrentIndex( hubComboBox->findText(
                                  QString::number( AppSetting::defaultHubCount() ) ) );

    switchComboBox->setCurrentIndex( switchComboBox->findText(
                       QString::number( AppSetting::defaultSwitchCount() ) ) );

    sp_ttlMac->setValue( AppSetting::ttlMac() );

    routerComboBox->setCurrentIndex( routerComboBox->findText(QString::number(
            AppSetting::defaultRouterCount() ) ) );

    int s = AppSetting::animateSpeed();
    sl_speed->setValue(s);

    languageBox->setCurrentIndex( AppSetting::language() );

    sb_send->setValue(AppSetting::sendingNum());

    sb_waitingTime->setValue(AppSetting::waitingTime());

    cb_opengl->setChecked( AppSetting::hasOpengl() );

    cb_autosave->setChecked( AppSetting::isAutosave() );

    sb_autosave->setValue( AppSetting::autosaveInterval() );

    btn_apply->setEnabled(false);
}

void settingDialog::applyEnable()
{
    btn_apply->setEnabled(true);
}

void settingDialog::apply()
{
    AppSetting::setDefaultComputerCount( computerComboBox->currentText().toInt() );
    AppSetting::setDefaultHubCount(  hubComboBox->currentText().toInt() );
    AppSetting::setDefaultSwitchCount(  switchComboBox->currentText().toInt() );
    AppSetting::setDefaultRouterCount(  routerComboBox->currentText().toInt() );
    AppSetting::setTtlArp( sp_ttlArp->value() );
    AppSetting::setArpResponceTime( sp_arpResponce->value() );
    AppSetting::setTtlMac( sp_ttlMac->value() );
    AppSetting::setAnimateSpeed(sl_speed->value());
    AppSetting::setLanguage( languageBox->currentIndex() );
    AppSetting::setSendingNum(sb_send->value());
    AppSetting::setWaitingTime(sb_waitingTime->value());
    AppSetting::setHasOpengl( cb_opengl->isChecked() );
    AppSetting::setAutosaveInterval( sb_autosave->value() );
    AppSetting::setAutosave( cb_autosave->isChecked() );
    btn_apply->setEnabled(false);
    if ( sender() == btn_ok ) accept();
}

void settingDialog::autosaveChanged(bool isAutosave)
{
    sb_autosave->setEnabled(isAutosave);
}

void settingDialog::defaultSettings()
{
    AppSetting::defaultNums();
    currentValues();   
}

void settingDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        languageBox->setCurrentIndex( AppSetting::language() );
        listWidget->setSpacing( listWidget->spacing() );
        break;
    default:
        break;
    }
}

