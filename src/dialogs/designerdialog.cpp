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
#include "designerdialog.h"
#include <QLabel>
#include <QTabWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include "ipedit.h"
#include "computer.h"
#include "tcppacket.h"
#include "udppacket.h"

designerDialog::designerDialog(computer *comp)
{
    myComputer = comp;
    QVBoxLayout *lay = new QVBoxLayout;
    tw_design = new QTabWidget;
    tw_design->addTab( createMainTab(), tr("Main") );
    tw_design->addTab( createFrameTab(), tr("Frame") );
    tw_design->addTab( createIpTab(), tr("Ip") );
    tw_design->addTab( createArpTab(), tr("Arp") );
    tw_design->addTab( createTcpTab(), tr("Tcp") );
    tw_design->addTab( createUdpTab(), tr("Udp") );
    tw_design->setTabEnabled( IP_TAB, false );
    tw_design->setTabEnabled( TCP_TAB, false );
    tw_design->setTabEnabled( UDP_TAB, false );
    lay->addWidget( tw_design );
    QList<QWidget*> list;
    btn_ok = new QPushButton( QIcon(":/im/images/ok.png"), tr("Ok") );
    btn_cancel = new QPushButton( QIcon(":/im/images/not.png"), tr("Cancel") );
    list << btn_ok << btn_cancel;
    lay->addStretch(1);
    lay->addLayout( createLayout( list ) );    
    setLayout( lay );
    setAttribute( Qt::WA_DeleteOnClose );
    connect( btn_cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( btn_ok, SIGNAL(clicked()), SLOT(apply()) );
    setWindowTitle(tr("Packet designer") );
}

QWidget* designerDialog::createMainTab()
{
    QWidget *main = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout;
    lb_interface = new QLabel( tr("Choose interface: ") );
    combo_interface = new QComboBox;
    QStringList l;
    foreach ( interface *i, myComputer->interfaces() )
        l << i->name();
    combo_interface->addItems( l );
    QList<QWidget*> list;
    list << lb_interface << combo_interface;
    lay->addLayout( createLayout( list ) );
    list.clear();
    lb_count = new QLabel( tr("Count: ") );
    sb_count = new QSpinBox;
    sb_count->setMinimum(1);
    sb_count->setMaximum(1000);
    list << lb_count << sb_count;
    lay->addLayout( createLayout( list ) );
    lay->addStretch(1);
    main->setLayout( lay );
    return main;
}

QWidget* designerDialog::createFrameTab()
{
    QWidget *frame = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout;
    QList<QWidget*> list;
    lb_senderMac = new QLabel( tr("Sender mac: ") );
    le_senderMac = new QLineEdit;
    le_senderMac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    le_senderMac->setText("00:00:00:00:00:00");
    list << lb_senderMac << le_senderMac;
    lay->addLayout( createLayout( list ) );    
    list.clear();
    lb_receiverMac = new QLabel( tr("Receiver mac: ") );
    le_receiverMac = new QLineEdit;
    le_receiverMac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    le_receiverMac->setText("00:00:00:00:00:00");
    list << lb_receiverMac << le_receiverMac;
    lay->addLayout( createLayout( list ) );    
    list.clear();
    QGroupBox *box = new QGroupBox;
    rb_ip = new QRadioButton( tr("Ip") );
    rb_arp = new QRadioButton( tr("Arp") );
    rb_arp->setChecked(true);
    list << rb_ip << rb_arp;
    box->setLayout( createLayout( list ) );
    lay->addWidget( box );
    lay->addStretch(1);
    frame->setLayout( lay );
    connect( rb_arp, SIGNAL(clicked(bool)), SLOT(changeFrameState()) );
    connect( rb_ip, SIGNAL(clicked()), SLOT(changeFrameState()) );
    return frame;
}

QWidget* designerDialog::createIpTab()
{
    QWidget *ip = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout;
    senderIp = new ipEdit( this,tr("Sender ip: ") );
    receiverIp = new ipEdit( this,tr("Receiver ip: ") );
    lay->addWidget( senderIp );
    lay->addWidget( receiverIp );
    QGroupBox *box = new QGroupBox;
    QList<QWidget*> list;
    rb_tcp = new QRadioButton( tr("Tcp") );
    rb_udp = new QRadioButton( tr("Udp") );
    rb_udp->setChecked(true);
    list << rb_tcp << rb_udp;
    box->setLayout( createLayout( list ) );
    lay->addWidget( box );
    lay->addStretch(1);
    ip->setLayout( lay );
    connect( rb_tcp, SIGNAL(clicked()), SLOT(changeIpState()) );
    connect( rb_udp, SIGNAL(clicked()), SLOT(changeIpState()) );
    return ip;
}

QWidget* designerDialog::createArpTab()
{
    QWidget *arp = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout;
    QList<QWidget*> list;
    QGroupBox *box = new QGroupBox;
    rb_response = new QRadioButton( tr("Response") );
    rb_request = new QRadioButton( tr("Request") );
    rb_request->setChecked(true);
    list << rb_response << rb_request;
    box->setLayout( createLayout( list ) );
    lay->addWidget( box );
    list.clear();
    lb_arpSenderMac = new QLabel( tr("Sender mac: ") );
    le_arpSenderMac = new QLineEdit;
    le_arpSenderMac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    le_arpSenderMac->setText("00:00:00:00:00:00");
    list << lb_arpSenderMac << le_arpSenderMac;
    lay->addLayout( createLayout( list ) );    
    list.clear();
    lb_arpReceiverMac = new QLabel( tr("Reciver mac: ") );
    le_arpReceiverMac = new QLineEdit;
    le_arpReceiverMac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    le_arpReceiverMac->setText("00:00:00:00:00:00");
    list << lb_arpReceiverMac << le_arpReceiverMac;
    lay->addLayout( createLayout( list ) );
    arpSenderIp = new ipEdit( this,tr("Sender ip: ") );
    arpReceiverIp = new ipEdit( this,tr("Receiver ip: ") );
    lay->addWidget( arpSenderIp );
    lay->addWidget( arpReceiverIp );
    lay->addStretch(1);
    arp->setLayout( lay );
    return arp;
}

QWidget* designerDialog::createTcpTab()
{
    QWidget *tcp = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout;
    QList<QWidget*> list;
    lb_senderPort = new QLabel( tr("Sender port: ") );
    sb_tcpSenderPort = new QSpinBox;
    sb_tcpSenderPort->setMaximum(0);
    sb_tcpSenderPort->setMaximum( MAX_PORT );
    list << lb_senderPort << sb_tcpSenderPort;
    lb_receiverPort = new QLabel( tr("Receiver port: ") );
    sb_tcpReceiverPort = new QSpinBox;
    sb_tcpReceiverPort->setMaximum(0);
    sb_tcpReceiverPort->setMaximum( MAX_PORT );
    list << lb_receiverPort << sb_tcpReceiverPort;
    lay->addLayout( createLayout( list ) );    
    list.clear();
    lb_sequence = new QLabel( tr("Sequence number: ") );
    sb_sequence = new QSpinBox;
    sb_sequence->setMinimum(0);
    sb_sequence->setMaximum( MAX_TCP_NUMBER );
    list << lb_sequence << sb_sequence;
    lb_ack = new QLabel( tr("Ack number: ") );
    sb_ack = new QSpinBox;
    sb_ack->setMinimum(0);
    sb_ack->setMaximum( MAX_TCP_NUMBER );
    list << lb_ack << sb_ack;
    lay->addLayout( createLayout( list ) );    
    list.clear();
    QGroupBox *box = new QGroupBox( tr("Flags") );
    QVBoxLayout *temp = new QVBoxLayout;
    cb_syn = new QCheckBox( tr("SYN") );
    cb_ack = new QCheckBox( tr("ACK") );
    list << cb_syn << cb_ack;
    temp->addLayout( createLayout( list ) );    
    list.clear();
    cb_reset = new QCheckBox( tr("RST") );
    cb_fin = new QCheckBox( tr("FIN") );
    list << cb_reset << cb_fin;
    temp->addLayout( createLayout( list ) );
    box->setLayout( temp );
    lay->addWidget( box );
    lay->addStretch(1);
    tcp->setLayout( lay );
    return tcp;
}

QWidget* designerDialog::createUdpTab()
{
    QWidget *udp = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout;
    QList<QWidget*> list;
    lb_senderPort = new QLabel( tr("Sender port: ") );
    sb_udpSenderPort = new QSpinBox;
    sb_udpSenderPort->setMaximum(0);
    sb_udpSenderPort->setMaximum( MAX_PORT );
    list << lb_senderPort << sb_udpSenderPort;
    lay->addLayout( createLayout( list ) );
    lb_receiverPort = new QLabel( tr("Receiver port: ") );
    sb_udpReceiverPort = new QSpinBox;
    sb_udpReceiverPort->setMaximum(0);
    sb_udpReceiverPort->setMaximum( MAX_PORT );
    list.clear();
    list << lb_receiverPort << sb_udpReceiverPort;
    lay->addLayout( createLayout( list ) );
    lay->addStretch(1);
    udp->setLayout( lay );
    return udp;
}

QHBoxLayout* designerDialog::createLayout( QList<QWidget*> list )
{
    QHBoxLayout *temp = new QHBoxLayout;
    for ( int i = 0; i < list.size(); i++ )
        temp->addWidget( list.at(i) );
    return temp;
}

void designerDialog::apply()
{
    frame Frame;
    Frame.setSender( le_senderMac->text() );
    Frame.setReceiver( le_receiverMac->text() );
    if ( rb_ip->isChecked() ) {
        Frame.setType( frame::ip );
        ipPacket Packet( senderIp->ipText() , receiverIp->ipText() );
        if ( rb_tcp->isChecked() ) {
            tcpPacket tcp;
            tcp.setSender( sb_tcpSenderPort->value() );
            tcp.setReceiver( sb_tcpReceiverPort->value() );
            tcp.setSequence( sb_sequence->value() );
            tcp.setAck( sb_ack->value() );
            quint8 flag;
            if ( cb_ack->isChecked() ) flag |= tcpPacket::ACK;
            if ( cb_fin->isChecked() ) flag |= tcpPacket::FIN;
            if ( cb_reset->isChecked() ) flag |= tcpPacket::RST;
            if ( cb_syn->isChecked() ) flag |= tcpPacket::SYN;
            tcp.setFlag(flag);
            Packet.pack(tcp.toData());
        }
        else {
            udpPacket udp;
            udp.setSender( sb_udpSenderPort->value() );
            udp.setReceiver( sb_udpReceiverPort->value() );
            Packet.pack( udp.toData() );
        }
        Frame.pack(Packet.toData());
    }
    else {
        Frame.setType( frame::arp );
        arpPacket arp;
        arp.setType( ( rb_request->isChecked() ) ? arpPacket::request : arpPacket::response );
        arp.setReceiverIp( arpReceiverIp->ipText() );
        arp.setSenderIp( arpSenderIp->ipText() );
        arp.setReceiverMac( le_arpReceiverMac->text() );
        arp.setSenderMac( le_arpSenderMac->text() );
        Frame.pack(arp.toData());
    }
    myComputer->sendConstructedFrame( combo_interface->currentText(), Frame, sb_count->value() );
    accept();
}

void designerDialog::changeFrameState()
{
    if ( rb_arp->isChecked() ) {
        tw_design->setTabEnabled( ARP_TAB, true );
        tw_design->setTabEnabled( IP_TAB, false );
        tw_design->setTabEnabled( TCP_TAB, false );
        tw_design->setTabEnabled( UDP_TAB, false );
        return;
    }
    tw_design->setTabEnabled( ARP_TAB, false );
    tw_design->setTabEnabled( IP_TAB, true );
    if ( rb_udp->isChecked() ) {
        tw_design->setTabEnabled( UDP_TAB, true );
        tw_design->setTabEnabled( TCP_TAB, false );
        return;
    }
    tw_design->setTabEnabled( TCP_TAB, true );
    tw_design->setTabEnabled( UDP_TAB, false );
}

void designerDialog::changeIpState()
{
    if ( rb_tcp->isChecked() ) {
        tw_design->setTabEnabled( UDP_TAB, false );
        tw_design->setTabEnabled( TCP_TAB, true );
        return;
    }
    tw_design->setTabEnabled( TCP_TAB, false );
    tw_design->setTabEnabled( UDP_TAB, true );
}
