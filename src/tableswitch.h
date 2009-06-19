#ifndef TABLESWITCH_H
#define TABLESWITCH_H

#include <QDialog>

class switchDevice;
class QLineEdit;
class QComboBox;
class QTableWidget;

class tableSwitch : public QDialog
{
    Q_OBJECT
public:
    tableSwitch();
    void correctSize();
    void updateTable();    
    void setSwitch(switchDevice *d);    
private:
    switchDevice *sw;
    QLineEdit *le_macAddress;
    QComboBox *cb_port;
    QTableWidget *table;
    QPushButton *btn_add;
    QPushButton *btn_del;
    QPushButton *btn_cancel;
public slots:
    void addRecord();
    void deleteRecord();
protected:
    void resizeEvent(QResizeEvent *e) { Q_UNUSED(e); correctSize(); }
};

#endif // TABLESWITCH_H
