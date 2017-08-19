#ifndef DECODEKRC_H
#define DECODEKRC_H

#include <QByteArray>
#include "zlib.h"
const static int Keys[16] =  {64, 71, 97, 119, 94, 50, 116, 71, 81, 54, 49, 45, 206, 210,110, 105};

extern int  ZlibGetDecodeLength(uLong  InRawLength)
{
    return  compressBound(InRawLength);
}
extern int  ZlibGetDecodeLength(QByteArray  InRawData)
{
    return  compressBound(InRawData.length());
}

extern long  ZlibUncompress(QByteArray  &OutDecodeData, QByteArray InEncodeData,
                     int   * nErrorCode = NULL)
{
    Bytef  *DecodeData = NULL; // 解压后的数据缓冲区
    Bytef  *EncodeData_Buffer = NULL;  // 要解压的数据缓冲区, 后面用memcpy放到这
    int     nFuncRet = Z_ERRNO;
    unsigned long  nOutLength = 0;
    nOutLength = ZlibGetDecodeLength(InEncodeData.length());
    nOutLength+=1000000;
    // 为缓冲区申请内存
    DecodeData        =  new Bytef[nOutLength];
    EncodeData_Buffer =  new Bytef[nOutLength];
    if (NULL != DecodeData && NULL != EncodeData_Buffer)
    {
        memcpy(EncodeData_Buffer, InEncodeData.constData(), InEncodeData.size());
        nFuncRet = uncompress(DecodeData, &nOutLength, (Bytef *)EncodeData_Buffer,
                              InEncodeData.length());
        if (Z_OK == nFuncRet)
        {
            OutDecodeData.append((const char *)DecodeData, nOutLength);
        }
        else
        {
            nOutLength = -1;
        }
    }
    else
    {
        nOutLength = -1;
    }
    if (DecodeData)
        delete  DecodeData;
    if (EncodeData_Buffer)
        delete  EncodeData_Buffer;
    if (nErrorCode)
        *nErrorCode = nFuncRet;
    return  nOutLength;
}
extern int  KrcDecode(QByteArray  &KrcData, QByteArray  &LrcData)
{
    int nRet=0;
    QByteArray DecodeData;
    if (!KrcData.isEmpty())
    {
        // 校验开头 4 字符是否为正确
        if (KrcData.left(4) == "krc1")
        {
            KrcData.remove(0, 4);  // 去除文件头标识
            for (int i = 0; i < KrcData.size(); i++)
            {
                DecodeData.append((char)(KrcData[i] ^ Keys[i % 16]));
            }
            ZlibUncompress(LrcData, DecodeData, NULL);
        }
    }
    return nRet;
}
#endif
