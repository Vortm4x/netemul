#include "dialogtemplate.h"
#include <QApplication>
#include <QStyle>

dialogTemplate::dialogTemplate()
{    
    lay = new QHBoxLayout;
    btn_cancel = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogCancelButton)),
                                 tr("Cancel"));
    connect( btn_cancel , SIGNAL(clicked()) , SLOT(reject()) );
    btn_ok = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogOkButton)),
                             tr("Ok"));
    connect( btn_ok , SIGNAL(clicked()) , SLOT(apply()) );
    btn_apply = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogApplyButton)),
                                tr("Apply"));
    connect(btn_apply, SIGNAL(clicked()) , SLOT(applyDisable()));

    lay->addWidget(btn_apply);
    lay->addWidget(btn_ok);
    lay->addWidget(btn_cancel);
    lay->setAlignment(Qt::AlignRight);
}

void dialogTemplate::applyEnable()
{
    btn_apply->setEnabled(true);
}

void dialogTemplate::applyDisable()
{
    btn_apply->setEnabled(false);
    apply();
}
