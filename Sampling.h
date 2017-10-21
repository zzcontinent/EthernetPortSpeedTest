#pragma once

#include "MyHeader.h"
#include "ProtoType.h"     //协议头解析类
//#include "Video.h"         //视频类
//#include "Voice.h"         //语音类
//#include "Message.h"       //报文类
//#include "UpperLowerCom.h" //上位机与下位机通信
//#include "Strategy.h"	   //策略类
#include "cgetpacket.h"    //获取网卡报文包
//#include "NTPTime.h"       //获取NTP服务器时间同步

//#include "DataCo.h"        //硬件配置类--LJL
//#include "cradio.h"        //电台类--FYB


//采集类完成所有工作
class CSampling
{
//1.采样缓冲区创建-------------------------------------------------------
public:
	CSampling()
	{
		m_StopSamp = 0;
		m_isSampThreadEnd = 0;

        m_isProcessThread1End = 0;
        m_isProcessThread2End = 0;
        m_isProcessThread3End = 0;
        m_isProcessThread4End = 0;
        m_isProcessThread5End = 0;
        m_isProcessThread6End = 0;
        m_isMonitorThreadEnd = 0;
		m_DataRecvCnt=0;//监视采集到的数据数量、速度
	}
	
	~CSampling() 
	{
		m_StopSamp = 1;
		StopSampling();//阻塞式函数，等待线程退出完毕后，返回
	}
	
	void StartSampling();//开始采样，创建采样线程、创建视频解码与保存线程
	void StopSampling();

//采样线程、视频解码与保存线程------------------------------------------------
private:
    static void* SamplingThread(void* lpParam);

    //处理线程:视频线程
    static void* ProcessingThread1(void* lpParam);
    //处理线程:音频线程
    static void* ProcessingThread2(void* lpParam);
    //处理线程:报文解码
    static void* ProcessingThread3(void* lpParam);
    //处理线程:电台类、NTP类
    static void* ProcessingThread4(void* lpParam);
    //处理线程:上位机与下位机通信模块
    static void* ProcessingThread5(void* lpParam);

    //SNMP service
    static void* ProcessingThread6(void* lpParam);


    //监听缓冲区内存是否满
    static void* BufferMonitorThread(void* lpParam);


private:
	bool m_StopSamp;//用于结束采样工作
	bool m_isSampThreadEnd;//线程退出后，置位
    bool m_isProcessThread1End;//用于线程退出后，置位
    bool m_isProcessThread2End;//用于线程退出后，置位
    bool m_isProcessThread3End;//用于线程退出后，置位
    bool m_isProcessThread4End;//用于线程退出后，置位
    bool m_isProcessThread5End;
    bool m_isProcessThread6End;
    bool m_isMonitorThreadEnd;
    bool m_isTimerThreadEnd;


	UINT m_DataRecvCnt;//监视采集到的数据数量、速度


private:
//    CVoice m_Voice;//语音类
//    CVideo m_Video;//视频类
//    CMessage m_Msg;//报文类
//    CUpperLowerCom m_UpperLowerCom;//上位机通信类
//    CNTPTime m_NTP;//NTP协议类
//    CRadio m_Radio;//电台类报文--SNMP

private:
    CGetPacket m_GetPacket;
};








