
#include "ProtoType.h"
namespace ProtoType
{
	int GetEtherToUDPLen()
	{
		return  sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER) + sizeof(UDP_HEADER);
	}
	int GetEtherToTCPLen()
	{
		return  sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER) + sizeof(TCP_HEADER);
	}
	int GetEtherToIPLen()
	{
		return  sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER);
	}
	int GetEtherLen()
	{
		return  sizeof(ETHERNET_HEADER);
	}

	bool IsArp(const BYTE* pBuf,UINT len)//从以太网帧判断
	{
		if(len < sizeof(ETHERNET_HEADER))
			return 0;
		else
		{
			ETHERNET_HEADER* pEthHeader = (ETHERNET_HEADER*)pBuf;
			if(::ntohs(pEthHeader->typeOrLen) == 0x0806)
				return 1;
			else 
				return 0;
		}
	}

    bool IsIP(const BYTE* pBuf,UINT len)
    {
        if(len < sizeof(ETHERNET_HEADER))
            return 0;
        else
        {
            ETHERNET_HEADER* pEthHeader = (ETHERNET_HEADER*)pBuf;
            if(::ntohs(pEthHeader->typeOrLen) == 0x0800)
                return 1;
            else
                return 0;
        }
    }

    bool IsICMP(const BYTE* pBuf,UINT len)
	{
		return 0;
	}

	bool IsTCP(const BYTE* pBuf,UINT len)
	{
		if(len < sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER))
			return 0;
        if(IsIP(pBuf,len))//非ARP类型
		{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			if(pIpheader->proto == 6)
				return 1;
			else 
				return 0;
		}
		else //返回
			return 0;
	}

	bool IsUDP(const BYTE* pBuf,UINT len)
	{
		if(len < sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER))
			return 0;
        if(IsIP(pBuf,len))//IP类型
		{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			if(pIpheader->proto == 17)
				return 1;
			else 
				return 0;
		}
		else //返回
			return 0;
	}

    //只接收应答报文HTTP协议
    bool IsHTTP(const BYTE* pBuf,UINT len)
    {
        int HdLen = GetEtherToTCPLen();
        string strHttp = (char*)(pBuf+HdLen);
        int pos0 = 0,pos1 = 0;
        if(IsTCP(pBuf,len))
        {
            //"GET /0/C/C/1_freetourw.jpg HTTP/1.1"
            pos1 = strHttp.find(' ',0);
            string str = strHttp.substr(pos0,pos1-pos0);
            if(str == "GET")
            {
                pos0 = strHttp.find(' ',pos1+1);
                pos1 = strHttp.find("\r\n",0);
                str = strHttp.substr(pos0+1,pos1-pos0-1);

                pos1 = str.find('/',0);
                str = str.substr(0,pos1);
                if(str == "HTTP")
                    return 1;
                else
                    return 0;
            }
            else
                return 0;
        }
        return 0;
    }


	bool IsFTP(const BYTE* pBuf,UINT len)
	{
		return 0;
	}
	bool IsSSH(const BYTE* pBuf,UINT len)
	{
		return 0;
	}
    UINT GetDestIP(const BYTE* pBuf,UINT len)
	{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			return pIpheader->destIP;
	}
    UINT GetSrcIP(const BYTE* pBuf,UINT len)
	{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			return pIpheader->sourceIP;
	}
	USHORT GetDestPort_UDP(const BYTE* pBuf,UINT len)
	{
			UDP_HEADER* pHd = (UDP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
			return pHd->uh_dport;
	}
	USHORT GetSrcPort_UDP(const BYTE* pBuf,UINT len)
	{
			UDP_HEADER* pHd = (UDP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
			return pHd->uh_sport;
	}
    USHORT GetDestPort_TCP(const BYTE* pBuf,UINT len)
    {
            TCP_HEADER* pHd = (TCP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
            return pHd->th_dport;
    }
    USHORT GetSrcPort_TCP(const BYTE* pBuf,UINT len)
    {
            TCP_HEADER* pHd = (TCP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
            return pHd->th_sport;
    }


	//<上位机与下位机自定义通信协议解析>
    bool IsUpperToLowerCmd(const BYTE* pBuf,UINT len)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)(pBuf + GetEtherToUDPLen());
		int lenx = GetEtherToUDPLen();
		lenx += sizeof(UPPERLOWERCMD);
		/*
			采集装置端标识：0x5263
			分析管理软件端标识：0x5665
		*/
		if(pHd->DestID[1]==0x52 && pHd->DestID[0]==0x63 && pHd->SrcID[1]==0x56 &&pHd->SrcID[0]==0x65  && len == GetEtherToUDPLen() + sizeof(UPPERLOWERCMD))
				return 1;
			else
				return 0;
	}
	USHORT GetUpperLowerCmd_DestID(const BYTE* pBuf)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pBuf;
		return pHd->DestID[1]*256 + pHd->DestID[0];
	}
	USHORT GetUpperLowerCmd_SrcID(const BYTE* pBuf)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pBuf;
		return pHd->SrcID[1]*256 + pHd->SrcID[0];
	}
	BYTE GetUpperLowerCmd_Cmd(const BYTE* pBuf)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pBuf;
		return pHd->Cmd;
	}
	void GetUpperLowerCmd_Data(BYTE* pDest,const BYTE* pSrc)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pSrc;
        memcpy(pDest,pHd->Data,50);
	}
    void SetLowerToUpperCmd(UPPERLOWERCMD* pULC,BYTE Cmd,BYTE* pBuf,int len)
	{
		memset(pULC,0,sizeof(UPPERLOWERCMD));
		pULC->DestID[1] = 0x56;
		pULC->DestID[0] = 0x65;
		pULC->SrcID[1] = 0x52;
		pULC->SrcID[0] = 0x63;
		pULC->Cmd = Cmd;
		memcpy(pULC->Data,pBuf,len);
	}
	//<!上位机与下位机自定义通信协议解析>
	
	//<报文类型判断>
    bool IsMsg_PreTrans(const BYTE* pBuf,UINT len)
	{
        UINT IP4 = GetSrcIP(pBuf,len);
        BYTE tmpIP[4];
        tmpIP[0] = (IP4>>24)&0xff;
        tmpIP[1] = (IP4>>16)&0xff;
        tmpIP[2] = (IP4>>8)&0xff;
        tmpIP[3] = (IP4)&0xff;

        //过滤广播包地址，过滤方式:IP最后字节，是否等于(255-mask的最后字节)。
        if(IsBroadCast(pBuf,len,240) || IsBroadCast(pBuf,len,0))
            return 0;

        //过滤
        USHORT SrcPort = ntohs(GetSrcPort_UDP(pBuf,len));
        USHORT DestPort = ntohs(GetDestPort_UDP(pBuf,len));
        //if module send a message to me , save them for m_Msg.Processing() to handle with
        if(127 == tmpIP[3] && 0 == tmpIP[2] && 0 == tmpIP[1] && 1 == tmpIP[0])
        {
            if(5000 == SrcPort && 5005 == DestPort)
                return 1;
            else
                return 0;
        }
        else
            return 1;
	}

    typedef struct
    {
        unsigned short type:3;
        unsigned short reply:1;
        unsigned short pt_len:12;
    } stLenAndType;

    //功能： 解掉UDP之后的数据传入此函数
    //返回值：TRUE：是短报文，存入数据库，FALSE：非短报文，直接丢弃
    bool IsShortPk(unsigned char *pbuf, int nLen)
    {
        if (nLen < 19 || NULL == pbuf)
            return false;

        stLenAndType stPk1;
        memcpy((void *)&stPk1, pbuf+4*sizeof(char), 2);
        if (stPk1.pt_len == 13 && stPk1.reply == 0 && stPk1.type == 0) //if (stPk1.pt_len == 13)
        {
//            if (pbuf[8] == 0 && pbuf[9] == 0 && pbuf[10] == 0
//                && pbuf[11] == 0 && pbuf[12] == 0)
            //{
                return true;
           // }
        }

        return false;
    }
    //音频解码模块
    bool IsVoice(const BYTE* pBuf,UINT len)
    {
        UINT IP4;
        BYTE tmpIP[4];
        USHORT* pdatalen = (USHORT*)(pBuf + ProtoType::GetEtherToUDPLen()+3);
        if(len == 370 && pBuf[42] == 0x8b && ::ntohs(*pdatalen) == 323)//转换成主机字节序
        {
            //如果是中心控制盒发出的数据，则过滤掉 10.28.1.8:08011c0a   10.28.2.8:08021c0a;
            IP4 = GetSrcIP(pBuf,len);
            tmpIP[0] = (IP4>>24)&0xff;
            tmpIP[1] = (IP4>>16)&0xff;
            tmpIP[2] = (IP4>>8)&0xff;
            tmpIP[3] = (IP4)&0xff;
            if(10 == tmpIP[3] && 28 == tmpIP[2] && 1 == tmpIP[1] && 8 == tmpIP[0])
                return 0;
            if(10 == tmpIP[3] && 28 == tmpIP[2] && 2 == tmpIP[1] && 8 == tmpIP[0])
                return 0;

            IP4 = GetDestIP(pBuf,len);
            tmpIP[0] = (IP4>>24)&0xff;
            tmpIP[1] = (IP4>>16)&0xff;
            tmpIP[2] = (IP4>>8)&0xff;
            tmpIP[3] = (IP4)&0xff;
            if(10 == tmpIP[3] && 28 == tmpIP[2] && 1 == tmpIP[1] && 8 == tmpIP[0])
                return 0;
            if(10 == tmpIP[3] && 28 == tmpIP[2] && 2 == tmpIP[1] && 8 == tmpIP[0])
                return 0;

            //否则是乘员盒的语音数据
            return 1;
        }
        else
            return 0;
    }


    bool IsDeviceState(const BYTE* pBuf,UINT len)
    {
        UINT UDPLen = ProtoType::GetEtherToUDPLen();
        USHORT dataLen = *(USHORT*)(pBuf+UDPLen+3);

        if( pBuf[UDPLen]==0x92 && len==UDPLen+5+dataLen )
            return 1;
        else
            return 0;
    }
    //<报文类型判断>

    //audio msg from 716 module
    //record msg only--to save to mysql
    bool IsVoice716(const BYTE* pBuf,UINT len)
    {
        if(len - GetEtherToUDPLen() < sizeof(_tVoiceMsgHd))
            return 0;

        UINT IP4 = GetSrcIP(pBuf,len);
        USHORT port = ntohs(GetSrcPort_UDP(pBuf,len));

        BYTE tmpIP[4];
        tmpIP[0] = (IP4>>24)&0xff;
        tmpIP[1] = (IP4>>16)&0xff;
        tmpIP[2] = (IP4>>8)&0xff;
        tmpIP[3] = (IP4)&0xff;

        //过滤回环地址UDP包
        _tVoiceMsgHd* phd = (_tVoiceMsgHd*)(pBuf + GetEtherToUDPLen());
        if(127 == tmpIP[3] && 0 == tmpIP[2] && 0 == tmpIP[1] && 1 == tmpIP[0]  && 7160 == port \
                && ntohl(phd->Hd)==0x0716FFFF)
        {
            return 1;
        }
        else
            return 0;
    }

    //MaskNum -- 255.255.255.240 -- (240)
    bool IsBroadCast(const BYTE* pBuf,UINT len,BYTE LastMaskNum)
    {
        UINT IPDest = ntohl(GetDestIP(pBuf,len));

        if((BYTE)(IPDest & 0x000000ff) == (255-LastMaskNum))
            return 1;
        else
            return 0;
    }



    bool IsVideo_UDP(const BYTE* pBuf,UINT len)
	{
		UINT UDPLen = ProtoType::GetEtherToUDPLen();
		if(len -  UDPLen> 12 && (pBuf[UDPLen]&0xC0) == 0x80 && (pBuf[1+UDPLen]&0x7f)==0x60)//RTP报文长度大于12,版本号是第一个字节的前2bit为2
		{
			return 1;
		}
		return 0;
	}
    bool IsVideo_TCP(const BYTE* pBuf,UINT len)
    {
        UINT TCPLen = ProtoType::GetEtherToTCPLen();
        //if(len -  TCPLen> 12 && (pBuf[TCPLen]&0xC0) == 0x80 && (pBuf[1+TCPLen]&0x7f)==0x60)//RTP报文长度大于12,版本号是第一个字节的前2bit为2
        if(len -  TCPLen > 0)//
        {
            return 1;
        }
        return 0;
    }

	//NTP时间同步类
	bool IsNTP(const BYTE* pBuf,UINT len)
	{
		USHORT SrcPort = ::ntohs(ProtoType::GetSrcPort_UDP(pBuf,len));
		if( SrcPort == 123)
			return 1;
		else
			return 0;
    }

	//电台类--0x30 0x82 0xlen1 0xlen2  端口号为161 or 162  协议类型UDP
	bool IsRadio(const BYTE* pBuf,UINT len)
	{
		UINT UDPLen = ProtoType::GetEtherToUDPLen();
		USHORT SrcPort = ::ntohs(ProtoType::GetSrcPort_UDP(pBuf,len));
        //if(((SrcPort == 161) || (SrcPort == 162)) && (pBuf[UDPLen] == 0x30 && pBuf[UDPLen+1] == 0x82))
        if(((SrcPort == 162)) && len > UDPLen)
        {
			return 1;
		}
		return 0;
	}

	//<!报文类型判断>
}
