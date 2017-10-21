#include "cgetpacket.h"

CGetPacket::CGetPacket()
{
    if((m_Socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP))) < 0)
    {
        fprintf(stdout, "创建采样套接字失败!\n");
        exit(0);
    }
    int RecvBufferlen = 1024*1024;
    if(0 != setsockopt(m_Socket,SOL_SOCKET,SO_RCVBUF,&RecvBufferlen,sizeof(RecvBufferlen)))
    {
        cout<<"采样套接字设置缓冲区失败!"<<endl;
    }
}

CGetPacket::~CGetPacket()
{
    shutdown(m_Socket, SHUT_RDWR);
    close(m_Socket);
}

void CGetPacket::GetPacket(BYTE* pData,int* plen)
{
    *plen = recvfrom(m_Socket, pData, 2048, 0 , NULL, NULL);//2048
}
