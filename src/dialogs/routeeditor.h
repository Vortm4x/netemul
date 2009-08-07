#ifndef ROUTEEDITOR_H
#define ROUTEEDITOR_H

#include <QDialog>
#include <QModelIndex>
class QTableView;
class QPushButton;
class QSpinBox;
class QComboBox;

class ipEdit;
class smartDevice;
class routeModel;

/*!
  Реализует отображение таблицы маршрутизации.
*/
class routeEditor : public QDialog
{
    Q_OBJECT
public:
    routeEditor(smartDevice *s);
    ~routeEditor();
private:
    void readSetting();
    void writeSetting() const;
    ipEdit *ip_dest;
    ipEdit *ip_mask;
    ipEdit *ip_gateway;
    QComboBox *cb_out;
    QSpinBox *sp_metr;
    QPushButton *btn_add;
    QPushButton *btn_remove;
    QPushButton *btn_close;
    QTableView *table;
    routeModel *model;
    smartDevice *dev;
public slots:
    void addRecord();
    void deleteRecord();
    void checkSelection(QModelIndex curr);
};
//--------------------------------------------------

#endif // ROUTEEDITOR_H
