#include "dialogtemplate.h"
#include <QApplication>
#include <QStyle>

dialogTemplate::dialogTemplate()
{    
    lay = new QHBoxLayout;
    btn_cancel = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogCancelButton)),
                                 trUtf8("Cancel"));
    connect( btn_cancel , SIGNAL(clicked()) , SLOT(reject()) );
    btn_ok = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogOkButton)),
                             trUtf8("Ok"));
    connect( btn_ok , SIGNAL(clicked()) , SLOT(apply()) );
    btn_apply = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogApplyButton)),
                                trUtf8("Apply"));
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
