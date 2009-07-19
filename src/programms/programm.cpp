#include "programm.h"

programm::programm()
{

}
/*!
  Записывает программу в поток.
  @param stream - поток для записи.
*/
void programm::write(QDataStream &stream) const
{
    stream << mySocket << myEnable;
}
//--------------------------------------------
/*!
  Считывает программу из потока.
  @param stream - поток для чтения.
*/
void programm::read(QDataStream &stream)
{
    stream >> mySocket >> myEnable;
}
//-------------------------------------------
