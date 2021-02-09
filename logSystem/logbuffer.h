#ifndef LOGBUFFER_H
#define LOGBUFFER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <exception>

class LogBuffer
{
public:

    enum LogBufferType {
        Buffer_FULL,
        Buffer_Valiable,
        Buffer_Type
    };

    LogBuffer(int nSingleBufLen);
    virtual ~LogBuffer();

    bool isEmpty();
    uint32_t getFreeLen();
    bool addLog(const char* pMsg, uint32_t nMsgLen);
    bool writeToFile(FILE* fp);
    void clear();

    LogBuffer *prev, *next;

public:
    char *m_pBuffer;
    uint32_t m_nTotalLen, m_nUsedLen;
    LogBufferType m_nType;
};

#endif // LOGBUFFER_H
