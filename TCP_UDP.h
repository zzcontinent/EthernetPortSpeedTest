#pragma once
#include "MyHeader.h"


//带有接收延迟的服务器socket
class CTCP_UDP
{
public:
	CTCP_UDP(void)//1:UDP,  2:TCP
	{
        InitUDPSocket();
	}
	~CTCP_UDP(void)
	{
        CloseUDPSocket();
	}
    void SendUDP(const char* pAddr,USHORT port,const BYTE* pBuf,int len)
    {
       sockaddr_in address;//处理网络通信的地址
       address.sin_family=AF_INET;
       address.sin_addr.s_addr=inet_addr(pAddr);//这里不一样
       address.sin_port=htons(port);
       sendto(m_UDPSocketServerClient,pBuf,len,0,(struct sockaddr *)&address,sizeof(sockaddr_in));
    }

    int BindUDPServer(USHORT port)
    {
        sockaddr_in address;//处理网络通信的地址
        address.sin_family=AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);//这里不一样
        address.sin_port=htons(port);
        if(-1 == bind(m_UDPSocketServerClient,(sockaddr*)&address,sizeof(sockaddr_in)))
        {
            cout<<"Socket服务器绑定失败!"<<endl;
            return -1;
        }
        else
            return 1;
    }

    int BindUDPServer(const char* pAddr,USHORT port)
    {
        sockaddr_in address;//处理网络通信的地址
        address.sin_family=AF_INET;
        address.sin_addr.s_addr = inet_addr(pAddr);//这里不一样
        address.sin_port=htons(port);
        if(-1 == bind(m_UDPSocketServerClient,(sockaddr*)&address,sizeof(sockaddr_in)))
        {
            cout<<"Socket服务器绑定失败!"<<endl;
            return -1;
        }
        return 1;
    }


    //接收数据、返回接收数据长度
    int RecvUDP(BYTE* pBuf)
    {
        int ret = recvfrom(m_UDPSocketServerClient, pBuf, 2048, 0, NULL, NULL);
        if(-1 == ret)
        {
            //cout<<"..."<<endl;
        }
        return ret;
    }

private:
    void InitUDPSocket(void)
	{
        m_UDPSocketServerClient = socket(AF_INET,SOCK_DGRAM,0);
        if(m_UDPSocketServerClient == -1 )
        {
            cout<<"UDP Socket创建失败!"<<endl;
        }

        struct timeval timeout;
        timeout.tv_sec = 5;//秒
        timeout.tv_usec = 0;//微秒
        if (setsockopt(m_UDPSocketServerClient, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
        {
            cout<<"设置Socket超时失败!"<<endl;
        }
	}


    void CloseUDPSocket()
	{
        shutdown(m_UDPSocketServerClient, SHUT_RDWR);
        close(m_UDPSocketServerClient);
	}

    int m_UDPSocketServerClient;
};

