#include "programmrep.h"
#include <QtDebug>

programmRep::programmRep()
{
}

programmRep::~programmRep()
{

}

/*!
  Записывает программу в поток.
  @param stream - поток для записи.
*/
void programmRep::write(QDataStream &stream) const
{
    stream << myEnable;
}
//--------------------------------------------
/*!
  Считывает программу из потока.
  @param stream - поток для чтения.
*/
void programmRep::read(QDataStream &stream)
{
    stream >> myEnable;
}
//-------------------------------------------
