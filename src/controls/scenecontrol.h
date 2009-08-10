#ifndef SCENECONTROL_H
#define SCENECONTROL_H

#include <QtCore/QObject>
#include <QtGui/QIcon>

class myCanvas;

class sceneControl : public QObject
{
    Q_OBJECT
public:
    sceneControl(QObject *parent,myCanvas *s);
    bool isSelect() const;
    QString tableName() const;
    QIcon tableIcon() const;
private:
    myCanvas *scene;
public slots:
    void observeSelection();
    void propertyDialog() const;
    void tableDialog() const;
    void adapterDialog() const;
    void programmsDialog() const;
    void arpDialog() const;
signals:
    void selectOneDevice(bool);
    void selectTableDevice(bool);
    void selectSmartDevice(bool);
};

#endif // SCENECONTROL_H
