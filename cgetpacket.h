#ifndef CGETPACKET_H
#define CGETPACKET_H

#include "MyHeader.h"

class CGetPacket
{
public:
    CGetPacket();
    ~CGetPacket();
    void GetPacket(BYTE* pData,int* plen);//captrue packets from promisc network card

private:
    int m_Socket;
};

#endif // CGETPACKET_H
