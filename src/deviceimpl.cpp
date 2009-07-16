#include "deviceimpl.h"

deviceImpl::deviceImpl()
{

}

/*!
  Выделеят кабель на порту с указанным именем.
  @param s - имя порта, если порта с таким иенем нет, то сбрасывает все выделения.
*/
void deviceImpl::setCheckedSocket(const QString &s)
{
    foreach ( devicePort *i , mySockets )
        if (i->isConnect()) i->setChecked( i->name() == s );
}
//----------------------------------------------------------
/*!
  Находит порт с указанным именем.
  @param name - имя искомого порта.
  @return указатель на порт или NULL если такого порта нет.
*/
devicePort* device::socket(const QString name)
{
    foreach ( devicePort *i, mySockets) {
        if ( i->name() == name )
            return i;
    }
    return 0;
}
