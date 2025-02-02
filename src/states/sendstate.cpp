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
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include "sendstate.h"
#include "mycanvas.h"
#include "senddialog.h"
#include "sendellipse.h"
#include "device.h"

SendState::SendState(MyCanvas *s) : AbstractState(s)
{
    mySendState = noSendItem;
    sendEllipse = new SendEllipse;
    scene->addItem(sendEllipse);
}

SendState::~SendState()
{
    scene->removeItem( sendEllipse);
    delete sendEllipse;
}

void SendState::hideState()
{
    sendEllipse->setPos(-100,-100);
}

void SendState::mouseMove(QGraphicsSceneMouseEvent *event)
{
    if ( sendEllipse ) sendEllipse->moving( event->scenePos() );
}

void SendState::mousePress(QGraphicsSceneMouseEvent*)
{
    if ( !sendEllipse->hasUnderDevice() ) return;
    QGraphicsItem *tempItem = sendEllipse->underDevice();
    Device *t = qgraphicsitem_cast<Device*>(tempItem);
    if ( !t ) return;
    if ( !t->isCanSend() ) {
        QMessageBox::warning(0,QObject::tr("Error"),QObject::tr("The device can't transmit data!"), QMessageBox::Ok , QMessageBox::Ok);
        return;
    }

    if ( mySendState == noSendItem ) {
        sendDialog *temp = new sendDialog(sendDialog::sender,t);
        if ( !temp->exec() ) return;
        messageSize = temp->messageSize();
        broadcast = temp->broadcast();
        protocol = temp->protocol();
        senderDevice = t;
        mySendState = oneSendItem;
        sendEllipse->chooseOneDevice();
        delete temp;
    } else {
        sendDialog *temp = new sendDialog(sendDialog::receiver,t);
        if ( !temp->exec() ) return;
        receiverIp = temp->dest();
        senderDevice->sendMessage(receiverIp,messageSize,protocol);
        emit scene->uncheck();
        delete temp;
        goMove();
    }
}
