#ifndef STATICSDIALOG_H
#define STATICSDIALOG_H

#include "ui_staticsdialog.h"

class myCanvas;
class statisticsScene;

class staticsDialog : public QDialog, private Ui::staticsDialog {
    Q_OBJECT
    Q_DISABLE_COPY(staticsDialog)
public:
    staticsDialog(statisticsScene *statistics, QWidget *parent = 0);
protected:
    void changeEvent(QEvent *e);
};

#endif // STATICSDIALOG_H
