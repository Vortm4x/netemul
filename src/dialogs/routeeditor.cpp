#include "routeeditor.h"
#include "ipedit.h"
#include "smartdevice.h"
#include "routemodel.h"
#include <QPushButton>
#include <QTableView>
#include <QSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSettings>
#include <QHeaderView>

routeEditor::routeEditor(smartDevice *s)
{
    dev = s;    
    QVBoxLayout *all = new QVBoxLayout;
    table = new QTableView;
    model = dev->routeTable();
    table->setModel(model);
    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    all->addWidget(table);
    ip_dest = new ipEdit(trUtf8("Адрес назначения: "));
    all->addWidget(ip_dest);
    ip_mask = new ipEdit(trUtf8("Маска: "));
    all->addWidget(ip_mask);
    ip_gateway = new ipEdit(trUtf8("Шлюз: "));
    all->addWidget(ip_gateway);
    QHBoxLayout *temp = new QHBoxLayout;
    temp->addWidget( new QLabel(trUtf8("Интерфейс: ")) );
    cb_out = new QComboBox;
    cb_out->setFixedWidth(250);
    cb_out->addItems( dev->interfacesIp() );
    temp->addWidget(cb_out);
    temp->addStretch(1);
    all->addLayout(temp);
    QHBoxLayout *lay = new QHBoxLayout;
    QLabel *lb = new QLabel(trUtf8("Метрика: "));
    lay->addWidget(lb);
    sp_metr = new QSpinBox;
    sp_metr->setMinimum(0);
    sp_metr->setMaximum(100);
    lay->addWidget(sp_metr);
    lay->addStretch(1);
    all->addLayout(lay);

    lay = new QHBoxLayout;
    btn_add = new QPushButton( QIcon(":/im/images/edit_add.png"), trUtf8("Добавить"));
    btn_add->setFixedWidth(150);
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    lay->addWidget( btn_add, 1, Qt::AlignLeft);
    btn_remove = new QPushButton(QIcon(":/im/images/edit_remove.png"),trUtf8("Удалить"));
    btn_remove->setFixedWidth(150);
    btn_remove->setEnabled(false);
    connect( btn_remove , SIGNAL(clicked()) ,SLOT(deleteRecord()) );
    lay->addWidget(btn_remove, 1, Qt::AlignLeft);
    lay->addStretch(2);
    btn_close = new QPushButton(QIcon(":/im/images/not.png"), trUtf8("Закрыть"));
    btn_close->setFixedWidth(150);
    lay->addWidget(btn_close, 1, Qt::AlignRight);
    all->addLayout(lay);

    connect(btn_close, SIGNAL(clicked()), SLOT(reject()));
    connect( table->selectionModel() , SIGNAL(currentRowChanged(QModelIndex,QModelIndex)) , SLOT(checkSelection(QModelIndex)) );
    readSetting();
    setLayout(all);
}

routeEditor::~routeEditor()
{
    writeSetting();
}

void routeEditor::addRecord()
{
    model->addToTable( ip_dest->ipText() , ip_mask->ipText() , ip_gateway->ipText() , cb_out->currentText() , sp_metr->value(),
                     routeModel::staticMode );
    ip_dest->clear();
    ip_mask->clear();
    ip_gateway->clear();
    cb_out->setCurrentIndex(0);
    sp_metr->setValue(0);
    btn_remove->setEnabled(false);

}

void routeEditor::deleteRecord()
{
    if ( !table->selectionModel()->hasSelection() ) return;
    model->deleteFromTable( table->currentIndex().row() );
    btn_remove->setEnabled(false);
}

/*!
  Проверяет выделенное и при необходимости делает недоступной кнопку удалить.
*/
void routeEditor::checkSelection(QModelIndex curr)
{
    if ( curr.isValid() && model->isConnectedMode(curr) ) {
        btn_remove->setEnabled(true);
        return;
    }
    btn_remove->setEnabled(false);
}
//------------------------------------------
/*!
  Читает размеры окна из файла.
*/
void routeEditor::readSetting()
{
    QSettings s("FROST","netemul");
    s.beginGroup("routeeditor");
    resize( s.value( "width", 800 ).toInt() ,
            s.value( "height", 600 ).toInt() );
    move( s.value( "left" , 100 ).toInt() ,
          s.value( "top" , 100 ).toInt() );
    s.endGroup();
}
//--------------------------------------------
/*!
  Записывает размеры окна в файл.
*/
void routeEditor::writeSetting() const
{
    QSettings s("FROST","netemul");
    s.beginGroup("routeeditor");
    s.setValue("width", width() );
    s.setValue("height", height() );
    s.setValue("left" , pos().x() );
    s.setValue("top" , pos().y() );
    s.endGroup();
}
//--------------------------------------------

void routeEditor::updateTable()
{

}

