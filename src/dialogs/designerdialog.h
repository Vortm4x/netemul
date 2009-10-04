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
#ifndef DESIGNERDIALOG_H
#define DESIGNERDIALOG_H

#include <QDialog>

class QLabel;
class QRadioButton;
class QLineEdit;
class QCheckBox;
class QTabWidget;
class QHBoxLayout;
class QSpinBox;
class QComboBox;
class ipEdit;
class computer;

class designerDialog : public QDialog
{
    Q_OBJECT
public:
    designerDialog(computer *com);
    ~designerDialog() {}
    QWidget* createMain();
    QWidget* createFrameTab();
    QWidget* createIpTab();
    QWidget* createArpTab();
    QWidget* createTcpTab();
    QWidget* createUdpTab();
    QHBoxLayout* createLayout( QList<QWidget*> list );
private slots:
    void apply();
    void changeFrameState();
    void changeIpState();
private:
    QTabWidget *tw_design;
    QLabel *lb_senderMac;
    QLabel *lb_receiverMac;
    QLabel *lb_arpSenderMac;
    QLabel *lb_arpReceiverMac;
    QLabel *lb_senderPort;
    QLabel *lb_receiverPort;
    QLabel *lb_sequence;
    QLabel *lb_ack;
    QLabel *lb_interface;
    QLabel *lb_count;
    QLineEdit *le_senderMac;
    QLineEdit *le_receiverMac;
    QLineEdit *le_arpSenderMac;
    QLineEdit *le_arpReceiverMac;
    QRadioButton *rb_ip;
    QRadioButton *rb_arp;
    QRadioButton *rb_tcp;
    QRadioButton *rb_udp;
    QRadioButton *rb_response;
    QRadioButton *rb_request;
    QCheckBox *cb_syn;
    QCheckBox *cb_ack;
    QCheckBox *cb_fin;
    QCheckBox *cb_reset;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;
    QSpinBox *sb_tcpSenderPort;
    QSpinBox *sb_tcpReceiverPort;
    QSpinBox *sb_udpSenderPort;
    QSpinBox *sb_udpReceiverPort;
    QSpinBox *sb_sequence;
    QSpinBox *sb_ack;    
    QSpinBox *sb_count;
    QComboBox *combo_interface;
    ipEdit *senderIp;
    ipEdit *receiverIp;
    ipEdit *arpSenderIp;
    ipEdit *arpReceiverIp;
    computer *myComputer;
};

#endif // DESIGNERDIALOG_H
