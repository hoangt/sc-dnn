#pragma once
#include "crc32.h"
#include "hashmap.h"

//--------------------------------------------------------------
//
// TransNum
//
//--------------------------------------------------------------

DWORD TransNum(LPCTSTR lpsz);

class CParamParser
{
public:
	CParamParser(int argc, TCHAR* argv[])
	{
        TCHAR szKey[32];
		for (int i = 1; i < argc; i ++)
		{
            int nIndex = 0;
			TCHAR *pcur = argv[i];
			if (*pcur == TEXT('-') || *pcur == TEXT('/') || *pcur == TEXT('\x2013')) // 2013 is EN-DASH
			{
				pcur++;
                while (*pcur != TEXT(':') && *pcur != TEXT(' ') && *pcur != TEXT('\0'))
                {
				    szKey[nIndex++] = *pcur;
				    pcur++;
                }
                szKey[nIndex++] = TEXT('\0');
				TCHAR *value = NULL;
				if (*pcur == TEXT(':'))
				{
					pcur++;
					value = pcur;
				}
				_hashMap.Add(GenerateCrc32String(szKey, lstrlen(szKey)), value);
			}
		}
	}

	BOOL IsParamExist(LPCTSTR pszKey)
	{
        UINT32 key = GenerateCrc32String(pszKey, lstrlen(pszKey));
		return _hashMap.IsKey(key);
	}

	LPCTSTR ParamString(LPCTSTR pszKey)
	{
        UINT32 key = GenerateCrc32String(pszKey, lstrlen(pszKey));
		return _hashMap.GetValue(key);
	}

	TCHAR ParamChar(LPCTSTR pszKey)
	{
        UINT32 key = GenerateCrc32String(pszKey, lstrlen(pszKey));
		if (!IsParamExist(pszKey))
		{
			return TEXT('\0');
		}
		return *_hashMap.GetValue(key);
	}

	int ParamInt(LPCTSTR pszKey)
	{
        UINT32 key = GenerateCrc32String(pszKey, lstrlen(pszKey));
		if (!IsParamExist(pszKey))
		{
			return 0;
		}
		return _ttoi(_hashMap.GetValue(key));
	}

	DWORD ParamHexDWORD(LPCTSTR pszKey)
	{
        UINT32 key = GenerateCrc32String(pszKey, lstrlen(pszKey));
		if (!IsParamExist(pszKey))
		{
			return 0;
		}
		return TransNum(_hashMap.GetValue(key));
	}

    INT64 ParamInt64(LPCTSTR pszKey)
	{
        UINT32 key = GenerateCrc32String(pszKey, lstrlen(pszKey));
		if (!IsParamExist(pszKey))
		{
			return 0;
		}
		return _wtoi64(_hashMap.GetValue(key));
	}

	float ParamFloat(LPCTSTR pszKey)
	{
		UINT32 key = GenerateCrc32String(pszKey, lstrlen(pszKey));
		if (!IsParamExist(pszKey))
		{
			return 0;
		}
		return (float)_wtof(_hashMap.GetValue(key));
	}

private:
	CHashMap<UINT32, TCHAR> _hashMap;
};