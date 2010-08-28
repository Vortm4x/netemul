#ifndef DEVICEIMPL_H
#define DEVICEIMPL_H

#include <QObject>

class DeviceImpl : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool router READ isRouter WRITE setRouter )
public:
    explicit DeviceImpl(QObject *parent = 0);

    bool isRouter() const { return m_router; }
    void setRouter(bool m ) { m_router = m; }

    virtual QString className() const = 0;

signals:

public slots:

private:
    bool m_router;
};

#endif // DEVICEIMPL_H
