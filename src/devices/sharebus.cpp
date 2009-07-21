#include "sharebus.h"

shareBus::shareBus(int c)
{
    Q_UNUSED(c);
    widthDev = defaultWidth;
}

devicePort* shareBus::addInterface(QString str, int t)
{
    Q_UNUSED(str);
    Q_UNUSED(t);
    return NULL;
}

