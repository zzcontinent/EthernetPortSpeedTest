#include <iostream>
#include "MyHeader.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>


#include "TCP_UDP.h"
#include "memregisters.h"
#include "memutils.h"


#include "algorithm"


using namespace std;

static void* ProcessingThread1(void* lpParam);
uint32_t recvCnt=0;
uint32_t sendCnt=0;

timeval GetLocalTime(void)
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);
    return tv;
}


mem_dev regs;
void *mappedCmd;

void InitOLED()
{

    regs.base_addr = XPAR_MEMORY_MAPPER_0_S00_AXI_BASEADDR;
    regs.high_addr = XPAR_MEMORY_MAPPER_0_S00_AXI_HIGHADDR;
    regs.dev_id = 0;
    regs.n_regs = 20;
    regs.reg_offset = 4;
    mappedCmd = MemoryMapping(regs.base_addr,regs.high_addr);
    printf("Mapping completed...\n");

    //OLED initialization
    mWriteReg((u32)mappedCmd,64,1);
}
void UnmapOLED()
{
    munmap((void*)mappedCmd, regs.high_addr-regs.base_addr);
    printf("Unmapped...\n");
}

/*
write 16Bytes(4 registers) per line
input:
pStr: string
len: string length

output:
n>0: register writed number
*/

int WriteReg_OLED(const char* pStr, int len)
{
    if(len == 0)
    {
        return 0;
    }
    else if(len > 64)
        len = 64;

    int len4 = len - len%4;

    u32 val = 0;
    for(int i=0;i<len4;i++)
    {
        val |= pStr[i]<<((i%4)<<3);// x<<0-8-16-24

        if((i+1)%4 == 0 && i>0)
        {
            mWriteReg((u32)mappedCmd,i-3,val);
            val = 0;
        }
    }

    if(len%4)
    {
        char len4bottom[4] = {0};
        int lenLeft = len -len4;
        for(int j=0;j<lenLeft;j++)
            len4bottom[j] = pStr[len4+j];

        u32 val = 0;
        val |= len4bottom[0];
        val |= len4bottom[1]<<8;
        val |= len4bottom[2]<<16;
        val |= len4bottom[3]<<24;
        mWriteReg((u32)mappedCmd,len4,val);
    }

    return len4/4+1;
}

void readReg_OLED(char* pStrOut,int RegCnt)
{
    uint32_t ret[8];
    for(int i=0;i<RegCnt;i++)
    {
        ret[i] = mReadReg((u32)mappedCmd, i*4);
        pStrOut[i*4+0] = ret[i]&0xff;
        pStrOut[i*4+1] = (ret[i]>>8)&0xff;
        pStrOut[i*4+2] = (ret[i]>>16)&0xff;
        pStrOut[i*4+3] = (ret[i]>>24)&0xff;
    }
}




int main()
{
    //1 设置printf缓冲区为0
    setvbuf(stdout,(char*)NULL,_IONBF,0);

    CTCP_UDP tcp_udp;
    cout << "input <1 port>                   : udp server" << endl;
    cout << "input <2 address port MB>        : udp client" << endl;

    cout << "input <3 port>                   : tcp server" << endl;
    cout << "input <4 address port MB>        : tcp client" << endl;


    cout << "input <5 string times>           : OLED test" << endl;

    cout << "input <6 port>                   : tcp_axi server" << endl;
    cout << "input <7 address port MB>        : tcp_axi client" << endl;

    cout << "input <10>                       : axi_interrupt test" << endl;

    cout<<endl;

    int i=0,port=0,MB=0;
    string straddr;

    struct timeval tv1,tv2;

    cin>>i;
    BYTE recvBuf[4096];
    BYTE sendBuf[4096];
    memset(recvBuf,0,4096);
    memset(sendBuf,0xff,4096);


    //create monitor thread
    pthread_t ProcessID1;

    //udp Server------------------------------------------------------------------
    if(1 == i)
    {

            cin>>port;
            tcp_udp.BindServerUDP(port);
            pthread_create(&ProcessID1,NULL,ProcessingThread1,&tcp_udp);//monitor thread
            while(1)
            {
                recvCnt += tcp_udp.RecvUDP(recvBuf);
            }
    }

    //udp client------------------------------------------------------------------
    else if(2 == i)
    {
            cin>>straddr;
            cin>>port;
            cin>>MB;
            uint64_t sendCnt = 0;
            int j=MB*500;

            tv1 = GetLocalTime();
            while(j--)
            {
                sendCnt += tcp_udp.SendUDP(straddr.c_str(),port,sendBuf,2000);
            }
            tv2 = GetLocalTime();

            float timediff = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)*1.0/1000000;
            cout<<endl;
            cout<<"Send Size(MB/MB):"<<sendCnt/1000000<<"/"<< MB <<endl;
            cout<<"Send Time(s):"<<timediff<<endl;
            cout<<"Send Speed(MB/s):"<<MB*1.0/timediff<<endl;
    }

    //tcp server------------------------------------------------------------------
    else if(3 == i)
    {
            cin>>port;
            pthread_create(&ProcessID1,NULL,ProcessingThread1,&tcp_udp);//monitor thread
            //tcp_udp.BindServerTCP("127.0.0.1",port);
            tcp_udp.BindServerTCP(port);
            if(-1 == tcp_udp.ListenTCP())
                return 0;

            int tmpRecvCnt=0;

            while(1)
            {
                if(!tcp_udp.GetIsTcpConnected())
                    if(-1 == tcp_udp.AcceptTCP())
                        continue;

                tmpRecvCnt = tcp_udp.RecvTCP(recvBuf);
                if(-1 != tmpRecvCnt || 0 != tmpRecvCnt)
                    recvCnt += tmpRecvCnt;
            }
     }

     //tcp client------------------------------------------------------------------
    else if(4 == i)
    {
        cin>>straddr;
        cin>>port;
        cin>>MB;
        int j=MB*500;

        uint64_t sendCnt = 0;
        if(-1 == tcp_udp.ConnectTCP(straddr.c_str(),port))
        {
            cout<<"connect failed";
            return 0;
        }
        //time delay is neccessary!!!
        sleep(1);
        tv1 = GetLocalTime();
        while(j--)
        {
            sendCnt += tcp_udp.SendTCP(sendBuf,2000);
        }
        tv2 = GetLocalTime();

        float timediff = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)*1.0/1000000;
        cout<<endl;
        cout<<"Send Size(MB/MB):"<<sendCnt/1000000<<"/"<< MB <<endl;
        cout<<"Send Time(s):"<<timediff<<endl;
        cout<<"Send Speed(MB/s):"<<MB*1.0/timediff<<endl;
    }


    //oled test------------------------------------------------------------------
    else if(5 == i)
    {
        InitOLED();
        cin>>straddr;
        int cnt;
        cin>>cnt;
        for(int i=0;i<cnt;i++)
            WriteReg_OLED(straddr.c_str(),strlen(straddr.c_str()));
        UnmapOLED();
    }


    //tcp_axi server ------------------------------------------------------------------
    else if(6 == i)
    {
        cin>>port;
        pthread_create(&ProcessID1,NULL,ProcessingThread1,&tcp_udp);//monitor thread

        tcp_udp.BindServerTCP(port);
        if(-1 == tcp_udp.ListenTCP())
            return 0;

        int tmpRecvCnt=0;

        while(1)
        {
            if(!tcp_udp.GetIsTcpConnected())
                if(-1 == tcp_udp.AcceptTCP())
                    continue;

            tmpRecvCnt = tcp_udp.RecvTCP(recvBuf);
            if(-1 != tmpRecvCnt || 0 != tmpRecvCnt)
            {
                recvCnt += tmpRecvCnt;
                //write 32 bytes to oled register

            }
        }
    }

    //tcp_axi client------------------------------------------------------------------
    else if(7 == i)
    {
        cin>>straddr;
        cin>>port;
        cin>>MB;
        int j=MB*500;

        uint64_t sendCnt = 0;
        if(-1 == tcp_udp.ConnectTCP(straddr.c_str(),port))
        {
            cout<<"connect failed";
            return 0;
        }
        //time delay is neccessary!!!
        sleep(1);
        tv1 = GetLocalTime();
        while(j--)
        {
            sendCnt += tcp_udp.SendTCP(sendBuf,2000);
        }
        tv2 = GetLocalTime();

        float timediff = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)*1.0/1000000;
        cout<<endl;
        cout<<"Send Size(MB/MB):"<<sendCnt/1000000<<"/"<< MB <<endl;
        cout<<"Send Time(s):"<<timediff<<endl;
        cout<<"Send Speed(MB/s):"<<MB*1.0/timediff<<endl;
    }



    return 0;
}



void* ProcessingThread1(void* lpParam)//
{
    uint i=0;
    CTCP_UDP* ptcp_udp = (CTCP_UDP*)lpParam;
    while(1)
    {

        //check tcp connect state per 20ms
        if(1 == (i)%5)
        {
            if(0 == ptcp_udp->CheckTcpConnection())
            {
                cout<<".";
            }
        }
        // print the receive speed per 1s
        if(0 == (i)%10)
        {
            cout<<"\r";
            cout<<"receive speed:"<<recvCnt*1.0/1000000<<"MB/s                           ";
            recvCnt = 0;
        }
        ++i;
        usleep(100000);
    }
    return 0;
}

