#pragma once
#include "MyHeader.h"
//环形队列缓冲区

template<class T>
class CMBuffer
{
public:
    CMBuffer(UINT len):m_Pos0(0),m_Pos1(0),m_DataLen(0)
    {
        m_BufferLen = len;
        m_pBuffer = new T[len];
        if(pthread_mutex_init(&m_Mutex, NULL) < 0)
               exit(0);
    }
    virtual ~CMBuffer()
    {
        delete []m_pBuffer;
        m_pBuffer = NULL;
        pthread_mutex_destroy(&m_Mutex);
    }

    int Add(const T* pSrc,UINT len)//返回实际添加的数据长度
    {
        UINT i = 0;
        pthread_mutex_lock(&m_Mutex);
        while (i < len)
        {
            if (-1 == Add(pSrc[i]))//添加失败，缓冲区满
            {
                break;
            }
            else
            {
                ++i;
            }
        }
        pthread_mutex_unlock(&m_Mutex);
        return i;
    }

    int Read(T* pDest, UINT len)
    {
        UINT i = 0;
        while (i < len)
        {
            if (-1 == Read(pDest++))
            {
                break;
            }
            else
            {
                ++i;
            }
        }
        return i;
    }


    int IsEmpty()
    {
        return ( 0 == m_DataLen);
    }
    int IsFull()
    {
        return (m_DataLen == m_BufferLen);
    }

    UINT GetDataLen()
    {
        return m_DataLen;
    }
    UINT GetCapacity()
    {
        return m_BufferLen;
    }

private:
    int Add(T data) //如果缓冲区满，返回-1，否则返回1
    {
        if (m_DataLen == m_BufferLen)//缓冲区已满
            return -1;
        else
        {
            m_pBuffer[m_Pos1++] = data;
            m_Pos1 = m_Pos1 % m_BufferLen;
            ++m_DataLen;
            return 1;
        }
    }
    int Read(T* data)
    {
        if (m_DataLen == 0)
            return -1;
        else
        {
            pthread_mutex_lock(&m_Mutex);
            *data = m_pBuffer[m_Pos0++];
            m_Pos0 = m_Pos0 % m_BufferLen;
            --m_DataLen;
            pthread_mutex_unlock(&m_Mutex);
            return 1;
        }
    }

private:
    T* m_pBuffer;
    UINT m_Pos0;
    UINT m_Pos1;
    UINT m_DataLen;
    UINT m_BufferLen;
    pthread_mutex_t m_Mutex;
};

