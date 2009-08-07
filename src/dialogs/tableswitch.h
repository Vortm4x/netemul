#ifndef TABLESWITCH_H
#define TABLESWITCH_H

#include "ui_tableswitch.h"

class switchTableSetting;
class switchModel;

class tableSwitch : public QDialog , private Ui::tableSwitch
{
    Q_OBJECT
    Q_DISABLE_COPY(tableSwitch)
public:
    explicit tableSwitch(switchTableSetting *s);
private:
    switchTableSetting *sw;
    switchModel *t_sw;
private slots:
    void addRecord();
    void deleteRecord();
    void selectionChanged();
    void resetSelection();
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // TABLESWITCH_H
