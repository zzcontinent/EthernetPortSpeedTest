#pragma once
#include "MyHeader.h"
namespace ProtoType
{
//1.UDP TCP ARP解析
	typedef struct _ethhdr //定义IP首部 
	{
		BYTE desMac[6];
		BYTE srcMac[6];
		USHORT typeOrLen;
	}ETHERNET_HEADER;
	typedef struct _iphdr //定义IP首部 
	{
		unsigned char h_verlen; //4位首部长度+4位IP版本号 
		unsigned char tos; //8位服务类型TOS 
		unsigned short total_len; //16位总长度（字节） 
		unsigned short ident; //16位标识 
		unsigned short frag_and_flags; //3位标志位 
		unsigned char ttl; //8位生存时间 TTL 
		unsigned char proto; //8位协议 (TCP, UDP 或其他) 
		unsigned short checksum; //16位IP首部校验和 
		unsigned int sourceIP; //32位源IP地址 
		unsigned int destIP; //32位目的IP地址 
	}IP_HEADER;
	typedef struct _udphdr //定义UDP首部
	{
		unsigned short uh_sport;    //16位源端口
		unsigned short uh_dport;    //16位目的端口
		unsigned short uh_len;//16位UDP包长度
		unsigned short uh_sum;//16位校验和
	}UDP_HEADER;
	typedef struct _tcphdr //定义TCP首部 
	{
		unsigned short th_sport; //16位源端口 
		unsigned short th_dport; //16位目的端口 
		unsigned int th_seq; //32位序列号 
		unsigned int th_ack; //32位确认号 
		unsigned char th_lenres;//4位首部长度/6位保留字 
		unsigned char th_flag; //6位标志位
		unsigned short th_win; //16位窗口大小
		unsigned short th_sum; //16位校验和
		unsigned short th_urp; //16位紧急数据偏移量
	}TCP_HEADER;
	typedef struct _icmphdr {
		unsigned char  icmp_type;
		unsigned char icmp_code; /* type sub code */
		unsigned short icmp_cksum;
		unsigned short icmp_id;
		unsigned short icmp_seq;
		/* This is not the std header, but we reserve space for time */
		unsigned short icmp_timestamp;
	}ICMP_HEADER;
	

	int GetEtherToUDPLen();
	int GetEtherToTCPLen();
	int GetEtherToIPLen();
	int GetEtherLen();

	bool IsArp(const BYTE* pBuf,UINT len);
    bool IsIP(const BYTE* pBuf,UINT len);
	bool IsICMP(const BYTE* pBuf,UINT len);
	bool IsTCP(const BYTE* pBuf,UINT len);
	bool IsUDP(const BYTE* pBuf,UINT len);
	bool IsFTP(const BYTE* pBuf,UINT len);
	bool IsSSH(const BYTE* pBuf,UINT len);
	bool IsHTTP(const BYTE* pBuf,UINT len);
	
    UINT GetDestIP(const BYTE* pBuf,UINT len);
    UINT GetSrcIP(const BYTE* pBuf,UINT len);
	USHORT GetDestPort_UDP(const BYTE* pBuf,UINT len);
	USHORT GetSrcPort_UDP(const BYTE* pBuf,UINT len);


//2.自定义上位机--下位机通信协议
	typedef struct _UpperLowerCmd
	{
		BYTE DestID[2];    //0x5263-采集装置，0x5665-上位机
		BYTE SrcID[2];
		BYTE   Cmd;		  //信令类型
        BYTE   Data[50];  //信令数据
	}UPPERLOWERCMD;

	//与1不同，pBuf是去掉UDP的数据段数据，而1则是一帧完整数据
    bool IsUpperToLowerCmd(const BYTE* pBuf,UINT len);
	USHORT GetUpperLowerCmd_DestID(const BYTE* pBuf);
	USHORT GetUpperLowerCmd_SrcID(const BYTE* pBuf);
	BYTE GetUpperLowerCmd_Cmd(const BYTE* pBuf);
	void GetUpperLowerCmd_Data(BYTE* pDest,const BYTE* pSrc);
	
    void SetLowerToUpperCmd(UPPERLOWERCMD* pULC,BYTE Cmd,BYTE* pBuf,int len);

//3.报文协议解析

    bool IsBroadCast(const BYTE* pBuf,UINT len,BYTE LastMaskNum);

    bool IsMsg_PreTrans(const BYTE* pBuf,UINT len);//报文预处理，其实就是其他所有数据都判断后，剩下的数据；为了防止访问第三方库速度慢导致采集线程阻塞，降低实时性

    bool IsMsg_PreTrans_Test(const BYTE* pBuf,UINT len);

    bool IsShortPk(unsigned char *pbuf, int nLen);

//4.设备状态信息解析
    bool IsDeviceState(const BYTE* pBuf,UINT len);

//4.音频协议解析
    typedef struct
    {
        UINT local_chn;
        UINT is_caller;
        UINT caller_ip;
        UINT called_ip;
        char caller_num[10];
        char called_num[10];
        UINT start_time;
        UINT end_time;
        char file_name[32];
    }_tVoiceCompleteNotice;

    typedef struct
    {
        UINT Hd;
        USHORT type;
        USHORT id;
        UINT seq;
        UINT lenData;
    }_tVoiceMsgHd;



    //音频车内解码
    bool IsVoice(const BYTE* pBuf,UINT len);

    //音频车际解码
    bool IsVoice716(const BYTE* pBuf,UINT len);

//5.视频协议解析
    bool IsVideo_UDP(const BYTE* pBuf,UINT len);
    bool IsVideo_TCP(const BYTE* pBuf,UINT len);

//7.NTP协议解析
	bool IsNTP(const BYTE* pBuf,UINT len);

//8.电台SNMP协议解析
	bool IsRadio(const BYTE* pBuf,UINT len);

};
