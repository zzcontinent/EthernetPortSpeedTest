#include <iostream>
#include "MyHeader.h"
#include "TCP_UDP.h"
using namespace std;

static void* ProcessingThread1(void* lpParam);
int recvCnt=0;
int sendCnt=0;

timeval GetLocalTime(void)
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);
    return tv;
}

int main()
{
    //1 设置printf缓冲区为0
    setvbuf(stdout,(char*)NULL,_IONBF,0);

    CTCP_UDP udp;
    cout << "input <1 port>: udp server" << endl;
    cout << "input <2 address port MB>: udp client" << endl;
    int i=0,port=0,MB=0;
    string straddr;

    struct timeval tv1,tv2;

    cin>>i;

    BYTE recvBuf[2048];
    BYTE sendBuf[4096];
    memset(recvBuf,0,2048);
    memset(sendBuf,0xff,4096);


    //create monitor thread
    pthread_t ProcessID1;
    if(1==i)
    {
        cin>>port;
        udp.BindUDPServer(port);
        pthread_create(&ProcessID1,NULL,ProcessingThread1,NULL);//monitor thread
        while(1)
        {
            recvCnt += udp.RecvUDP(recvBuf);
        }
    }
    else if(2==i)
    {
        cin>>straddr;
        cin>>port;
        cin>>MB;
        int j=MB*500;

        tv1 = GetLocalTime();
        while(j--)
        {
            udp.SendUDP(straddr.c_str(),port,sendBuf,2000);
        }
        tv2 = GetLocalTime();

        float timediff = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)*1.0/1000000;
        cout<<"Send Size(MB):"<< MB <<endl;
        cout<<"Send Time(s):"<<timediff<<endl;
        cout<<"Send Speed(MB/s):"<<MB*1.0/timediff<<endl;
    }
    return 0;
}



void* ProcessingThread1(void* lpParam)//从缓存中获取数据，解码并分类保存
{
    while(1)
    {
        cout<<"\r";
        cout<<"receive speed:"<<recvCnt*1.0/1000000<<"MB/s                           ";
        recvCnt = 0;
        sleep(1);
    }
    return 0;
}

