#include "logbuffer.h"

LogBuffer::LogBuffer(int nSingleBufLen)
{
    try {
        m_pBuffer = new char[nSingleBufLen];
        m_nUsedLen = 0;
        m_nTotalLen = nSingleBufLen;
        prev = nullptr;
        next = nullptr;
        m_nType = Buffer_Valiable;
    }
    catch (std::exception e)
    {
        exit(1);
    }
}

LogBuffer::~LogBuffer()
{
    delete m_pBuffer;
}

bool LogBuffer::isEmpty()
{
    return (m_nUsedLen == 0);
}

uint32_t LogBuffer::getFreeLen()
{
    return m_nTotalLen - m_nUsedLen;
}

bool LogBuffer::addLog(const char* msg, uint32_t msgLen)
{
    if (msgLen <= 0) return false;
    if (m_nTotalLen - m_nUsedLen <= msgLen) return false;
    memcpy(m_pBuffer + m_nUsedLen, msg, msgLen);
    m_nUsedLen += msgLen;
    return true;
}

bool LogBuffer::writeToFile(FILE* fp)
{
    uint32_t writeLen = fwrite(m_pBuffer, 1, m_nUsedLen, fp);
    return (writeLen == m_nUsedLen);
}

void LogBuffer::clear()
{
    m_nUsedLen = 0;
    m_nType = Buffer_Valiable;
}

