#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

class device;

#include "ui_connectdialog.h"

class connectDialog : public QDialog , private Ui::connectDilog
{
    Q_OBJECT
    Q_DISABLE_COPY(connectDialog)
public:
    explicit connectDialog(device *start,device *end);
public slots:
    void changeSelect();
    QString getStart() { return startList->currentItem()->text(); }
    QString getEnd() { return  endList->currentItem()->text(); }
private:
    QIcon connectIcon(bool b) const;
    device *start;
    device *end;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // CONNECTDIALOG_H
