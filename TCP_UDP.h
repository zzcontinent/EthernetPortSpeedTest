#pragma once
#include "MyHeader.h"


//带有接收延迟的服务器socket
class CTCP_UDP
{
public:
    CTCP_UDP(void):m_IsTcpClientConnected(false)//1:UDP,  2:TCP
    {
        InitUDPSocket();
        InitTCPSocket();
	}
	~CTCP_UDP(void)
	{
        CloseUDPSocket();
        CloseTCPSocket();
	}
    int SendUDP(const char* pAddr,USHORT port,const BYTE* pBuf,int len)
    {
       sockaddr_in address;//处理网络通信的地址
       address.sin_family=AF_INET;
       address.sin_addr.s_addr=inet_addr(pAddr);//这里不一样
       address.sin_port=htons(port);
       return sendto(m_UDPSocketServerClient,pBuf,len,0,(struct sockaddr *)&address,sizeof(sockaddr_in));
    }
    //接收数据、返回接收数据长度
    int RecvUDP(BYTE* pBuf)
    {
        int addr_len = sizeof(sockaddr_in);
        int ret = recvfrom(m_UDPSocketServerClient, pBuf, 2048, 0, (struct sockaddr*)&m_UDPServerRecvFromAddress, (socklen_t*)&addr_len);
        if(-1 == ret)
        {
            //cout<<"..."<<endl;
        }
        return ret;
    }
    int BindServerUDP(USHORT port)
    {
        sockaddr_in address;//处理网络通信的地址
        address.sin_family=AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);//这里不一样
        address.sin_port=htons(port);
        if(-1 == bind(m_UDPSocketServerClient,(sockaddr*)&address,sizeof(sockaddr_in)))
        {
            cout<<"Socket Udp服务器绑定失败!"<<endl;
            return -1;
        }
        else
            return 1;
    }

    int BindServerUDP(const char* pAddr,USHORT port)
    {
        sockaddr_in address;//处理网络通信的地址
        address.sin_family=AF_INET;
        address.sin_addr.s_addr = inet_addr(pAddr);//这里不一样
        address.sin_port=htons(port);
        if(-1 == bind(m_UDPSocketServerClient,(sockaddr*)&address,sizeof(sockaddr_in)))
        {
            cout<<"Socket Udp服务器绑定失败!"<<endl;
            return -1;
        }
        return 1;
    }


    int BindServerTCP(USHORT port)
    {
        sockaddr_in address;//处理网络通信的地址
        address.sin_family=AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);//这里不一样
        address.sin_port=htons(port);
        if(-1 == bind(m_TCPSocketServerClient,(sockaddr*)&address,sizeof(sockaddr_in)))
        {
            cout<<"Socket Tcp服务器绑定失败!"<<endl;
            return -1;
        }
        else
            return 1;
    }

    int BindServerTCP(const char* pAddr,USHORT port)
    {
        sockaddr_in address;//处理网络通信的地址
        address.sin_family=AF_INET;
        address.sin_addr.s_addr = inet_addr(pAddr);//这里不一样
        address.sin_port=htons(port);
        if(-1 == bind(m_TCPSocketServerClient,(sockaddr*)&address,sizeof(sockaddr_in)))
        {
            cout<<"Socket Tcp服务器绑定失败!"<<endl;
            return -1;
        }
        return 1;
    }


    int ListenTCP(void)
    {
        if(listen(m_TCPSocketServerClient,10) == -1) //在端口sock_descriptor监听
        {
            perror("call to listen error ");
            return -1;
        }
        cout<<"listen one tcp!"<<endl;
        return 1;
    }

    int AcceptTCP(void)
    {
       //用来监听的端口sock_descriptor
       socklen_t address_size = sizeof(sockaddr_in);//处理网络通信的地址
       m_ConnectTCPSocketServerClient = accept(m_TCPSocketServerClient,(struct sockaddr *)&m_TCPServerAcceptAddress, &address_size);
       if(m_ConnectTCPSocketServerClient== -1)
       {
          perror("call to accept error ");
          return -1;
       }
       printf("accept client ip:%s\n port:%d\n",inet_ntoa(m_TCPServerAcceptAddress.sin_addr),ntohs(m_TCPServerAcceptAddress.sin_port));
       return 1;
    }

    int RecvTCP(BYTE* pBuf)
    {
        int ret = recv(m_ConnectTCPSocketServerClient, pBuf, 2048,0);
        if(-1 == ret)
        {
            //cout<<"recv error"<<endl;
            return -1;
        }
        else
            return ret;
    }

    int ConnectTCP(const char* pAddr,USHORT port)
    {
        sockaddr_in address;//处理网络通信的地址
        address.sin_family=AF_INET;
        address.sin_addr.s_addr=inet_addr(pAddr);//这里不一样
        address.sin_port=htons(port);
        if(connect(m_TCPSocketServerClient,(sockaddr*)&address,sizeof(address))==-1)
        {
            cout<<"connect failed"<<endl;
            return -1;
        }
        return 1;
    }

    int SendTCP(const BYTE* pBuf,int len)
    {
       int ret;
       ret = send(m_TCPSocketServerClient,pBuf,len,0);
       if(-1 == ret)
       {
           cout<<"*";
       }
       return ret;
    }

    bool CheckTcpConnection()
    {
        m_IsTcpClientConnected = GetSocketConnectedTCP(m_ConnectTCPSocketServerClient);
        return m_IsTcpClientConnected;
    }
    //firstly,call CheckTcpConnection() to update the connection state;
    bool GetIsTcpConnected()
    {
        return m_IsTcpClientConnected;
    }


    void GetClientAddressPort_TCP(char* pAddr,ushort* port)
    {
        strcpy(pAddr,inet_ntoa(m_TCPServerAcceptAddress.sin_addr));
        *port = ntohs(m_TCPServerAcceptAddress.sin_port);
    }

    void GetClientAddressPort_UDP(char* pAddr,ushort* port)
    {
        strcpy(pAddr,inet_ntoa(m_UDPServerRecvFromAddress.sin_addr));
        *port = ntohs(m_UDPServerRecvFromAddress.sin_port);
    }

private:

    int GetSocketConnectedTCP(int sock)
    {
        if (sock <= 0)
            return 0;
        struct tcp_info info;
        int len = sizeof(info);
        getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *) & len);
        //if ((info.tcpi_state == TCP_ESTABLISHED)) {
        if ((info.tcpi_state == 1)) {
            //myprintf("socket connected\n");
            return 1;
        } else {
            //myprintf("socket disconnected\n");
            return 0;
        }
    }

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

    void InitTCPSocket(void)
    {
        m_TCPSocketServerClient = socket(AF_INET,SOCK_STREAM,0);
        if(m_TCPSocketServerClient == -1 )
        {
            cout<<"TCP Socket创建失败!"<<endl;
        }
    }
    void CloseTCPSocket(void)
    {
        shutdown(m_TCPSocketServerClient, SHUT_RDWR);
        close(m_TCPSocketServerClient);
        shutdown(m_ConnectTCPSocketServerClient, SHUT_RDWR);
        close(m_ConnectTCPSocketServerClient);
    }


    void CloseUDPSocket()
	{
        shutdown(m_UDPSocketServerClient, SHUT_RDWR);
        close(m_UDPSocketServerClient);
	}
    bool m_IsTcpClientConnected;
    int m_UDPSocketServerClient;
    int m_TCPSocketServerClient;
    int m_ConnectTCPSocketServerClient;
    sockaddr_in m_TCPServerAcceptAddress;//处理网络通信的地址
    sockaddr_in m_UDPServerRecvFromAddress;//处理网络通信的地址
};

