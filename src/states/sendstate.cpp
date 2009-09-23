#include <QtGui/QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include "sendstate.h"
#include "mycanvas.h"
#include "senddialog.h"
#include "sendellipse.h"
#include "device.h"

sendState::sendState(myCanvas *s) : abstractState(s)
{
    mySendState = noSendItem;
    SendEllipse = new sendEllipse;
    scene->addItem(SendEllipse);
}

sendState::~sendState()
{
    scene->removeItem( SendEllipse);
    delete SendEllipse;
}

void sendState::hideState()
{
    SendEllipse->setPos(-100,-100);
}

void sendState::mouseMove(QGraphicsSceneMouseEvent *event)
{
    if ( SendEllipse ) SendEllipse->moving( event->scenePos() );
}

void sendState::mousePress(QGraphicsSceneMouseEvent*)
{
    if ( !SendEllipse->hasUnderDevice() ) return;
    QGraphicsItem *tempItem = SendEllipse->underDevice();
    device *t = qgraphicsitem_cast<device*>(tempItem);
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
            SendEllipse->chooseOneDevice();
    } else {
        sendDialog *temp = new sendDialog(sendDialog::receiver,t);
        if ( !temp->exec() ) return;
        receiverIp = temp->dest();
        senderDevice->sendMessage(receiverIp,messageSize,protocol);
        emit scene->uncheck();
        goMove();
    }
}
