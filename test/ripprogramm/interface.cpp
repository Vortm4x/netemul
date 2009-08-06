#include "interface.h"

interface::interface()
{

}

void interface::sendPacket(ipPacket p)
{
    udpPacket u( p.unpack() );
    buf = u.unpack();
}
