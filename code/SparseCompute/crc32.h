#pragma once
#include "stdafx.h"

extern const UINT32 s_CrcTable[256];

inline UINT32 UPDC32(BYTE octet, UINT32 crc)
{
    return (s_CrcTable[((crc) ^ ((BYTE)octet)) & 0xff] ^ ((crc) >> 8));
}

//
// GenerateCrc32String() is matched with managed function Crc32.CRC32String() in Crc32.cs
//
inline UINT32 GenerateCrc32String(LPCWSTR psz, UINT32 nLength)
{
    UINT32 oldcrc32;
    oldcrc32 = 0xFFFFFFFF;
    int len = nLength;
    WCHAR wc;
    BYTE lowByte;
    BYTE hiByte;
    for (int i = 0; len > 0; i++)
    {
        --len;
        wc = psz[len];
        lowByte = (BYTE)(wc & 0xffff);
        hiByte = (BYTE)(wc >> 8);
        oldcrc32 = UPDC32(hiByte, oldcrc32);
        oldcrc32 = UPDC32(lowByte, oldcrc32);
    }
    return ~oldcrc32;
}

inline UINT32 GenerateCrc32(PVOID pvBuffer, UINT32 cbBuffer)
{
    UINT32 dwResult = 0xFFFFFFFF;
    PBYTE pbBuffer = reinterpret_cast<PBYTE>(pvBuffer);
    if (pbBuffer)
    {
        for (UINT32 i=0;i<cbBuffer;i++)
        {
            dwResult = (dwResult >> 8) ^ s_CrcTable[static_cast<BYTE>(dwResult ^ static_cast<UINT32>(pbBuffer[i]))];
        }
    }
    return dwResult ^ 0xFFFFFFFF;
}

void CRC32Update(UINT32 *pCRC32,void *p,unsigned long cb);