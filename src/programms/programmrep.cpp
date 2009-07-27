#include "programmrep.h"
#include <QtDebug>

programmRep::programmRep()
{
}

/*!
  Записывает программу в поток.
  @param stream - поток для записи.
*/
void programmRep::write(QDataStream &stream) const
{
    stream << mySocket << myEnable;
    qDebug() << "1";
}
//--------------------------------------------
/*!
  Считывает программу из потока.
  @param stream - поток для чтения.
*/
void programmRep::read(QDataStream &stream)
{
    stream >> mySocket >> myEnable;
    qDebug() << "2";
}
//-------------------------------------------
