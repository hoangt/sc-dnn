//
// xstring.h
//
// Unicode/ansi conversion.
//
#pragma once
#include "crtdbg.h"
#include <strsafe.h>
#include <objidl.h>

class WtoA {
public:
    WtoA(LPCWSTR str)
    {
        _cch = 0;
        int cch = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
        _pch = new char[cch + 1];
        if (_pch)
        {
            ZeroMemory(_pch, cch + 1);
            WideCharToMultiByte(CP_ACP, 0, str, -1, _pch, cch, NULL, NULL);
            _cch = cch;
        }
    }

    WtoA(__in_ecount(cch) const WCHAR* str, ULONG cch)
    {
        _cch = 0;
        int cchA = WideCharToMultiByte(CP_ACP, 0, str, cch, NULL, 0, NULL, NULL);
        _pch = new char[cchA + 1];
        if (_pch)
        {
            ZeroMemory(_pch, cchA + 1);
            WideCharToMultiByte(CP_ACP, 0, str, cch, _pch, cchA, NULL, NULL);
            _cch = cchA;
        }
    }
    ~WtoA()
    {
        delete _pch;
    }

    operator char*()
    {
        if (_pch)
            return _pch;

        _ASSERT(0);
        return "\0";
    }

    int Length()
    {
        return _cch;
    }

    static int SafeCopy(char *pszOut, ULONG cchOut, const WCHAR *pszIn, ULONG cchIn)
    {
        _ASSERT(cchOut > 0);
        int cch = WideCharToMultiByte(CP_ACP, 0, pszIn, cchIn, pszOut, cchOut, NULL, NULL);
        pszOut[cch] = L'\0';
        return cch;
    }

protected:
    char* _pch;
    int _cch;
};

class AtoW {
public:
    AtoW(LPCSTR str)
    {
        _cch = 0;
        int cch = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
        _pch = new WCHAR[cch + 1];
        if (_pch)
        {
            ZeroMemory(_pch, (cch + 1) * sizeof(WCHAR));
            MultiByteToWideChar(CP_ACP, 0, str, -1, _pch, cch + 1);
            _cch = cch;
        }
    }

    AtoW(__in_ecount(cch) const char* str, ULONG cch)
    {
        _cch = 0;
        int cchW = MultiByteToWideChar(CP_ACP, 0, str, cch, NULL, 0);
        _pch = new WCHAR[cchW + 1];
        if (_pch)
        {
            ZeroMemory(_pch, (cchW + 1) * sizeof(WCHAR));
            MultiByteToWideChar(CP_ACP, 0, str, cch, _pch, cchW + 1);
            _cch = cch;
        }
    }

    ~AtoW()
    {
        delete _pch;
    }

    operator WCHAR*()
    {
        if (_pch)
            return _pch;

        _ASSERT(0);
        return L"\0";
    }

    int Length()
    {
        return _cch;
    }

    static int SafeCopy(WCHAR *pwszOut, ULONG cchOut, const char *pszIn, ULONG cchIn)
    {
        _ASSERT(cchOut > 0);
        int cch = MultiByteToWideChar(CP_ACP, 0, pszIn, cchIn, pwszOut, cchOut - 1);
        pwszOut[cch] = L'\0';
        return cch;
    }

protected:
    WCHAR* _pch;
    int _cch;
};

class WtoUTF8 {
public:
    WtoUTF8(LPCWSTR str)
    {
        _cch = 0;
        int cch = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
        _pch = new char[cch + 1];
        if (_pch)
        {
            ZeroMemory(_pch, cch + 1);
            WideCharToMultiByte(CP_UTF8, 0, str, -1, _pch, cch, NULL, NULL);
            _cch = cch;
        }
    }

    WtoUTF8(__in_ecount(cch) const WCHAR* str, ULONG cch)
    {
        _cch = 0;
        int cchA = WideCharToMultiByte(CP_UTF8, 0, str, cch, NULL, 0, NULL, NULL);
        _pch = new char[cchA + 1];
        if (_pch)
        {
            ZeroMemory(_pch, cchA + 1);
            WideCharToMultiByte(CP_UTF8, 0, str, cch, _pch, cchA, NULL, NULL);
            _cch = cchA;
        }
    }
    ~WtoUTF8()
    {
        delete _pch;
    }

    operator char*()
    {
        if (_pch)
            return _pch;

        _ASSERT(0);
        return "\0";
    }

    int Length()
    {
        return _cch;
    }

protected:
    char* _pch;
    int _cch;
};

class UTF8toW {
public:
    UTF8toW(LPCSTR str)
    {
        _cch = 0;
        int cch = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
        _pch = new WCHAR[cch + 1];
        if (_pch)
        {
            ZeroMemory(_pch, (cch + 1) * sizeof(WCHAR));
            MultiByteToWideChar(CP_UTF8, 0, str, -1, _pch, cch + 1);
            _cch = cch;
        }
    }

    UTF8toW(__in_ecount(cch) const char* str, ULONG cch)
    {
        _cch = 0;
        int cchW = MultiByteToWideChar(CP_UTF8, 0, str, cch, NULL, 0);
        _pch = new WCHAR[cchW + 1];
        if (_pch)
        {
            ZeroMemory(_pch, (cchW + 1) * sizeof(WCHAR));
            MultiByteToWideChar(CP_UTF8, 0, str, cch, _pch, cchW + 1);
            _cch = cchW;
        }
    }

    ~UTF8toW()
    {
        delete _pch;
    }

    operator WCHAR*()
    {
        if (_pch)
            return _pch;

        _ASSERT(0);
        return L"\0";
    }

    int Length()
    {
        return _cch;
    }

protected:
    WCHAR* _pch;
    int _cch;
};

class CXStringA
{
public:
    CXStringA(LPCSTR psz)
    {
        _psz = NULL;
        _nLen = 0;
        Init(psz);
    }

    CXStringA()
    {
        _psz = NULL;
        _nLen = 0;
    }

    ~CXStringA()
    {
        Clear();
    }

    BOOL Init(LPCSTR psz, int nLen = -1)
    {
        Clear();

        if (psz)
        {
            if (nLen == -1)
                _nLen = lstrlenA(psz);
            else
                _nLen = nLen;
            _psz = (LPSTR)LocalAlloc(LPTR, (_nLen + 1) * sizeof(char));
            if (_psz)
            {
                StringCchCopyNA(_psz, _nLen + 1, psz, _nLen);
            }
        }
        return TRUE;
    }

    void Clear()
    {
        if (_psz)
        {
            LocalFree(_psz);
            _psz = NULL;
            _nLen = 0;
        }
    }

    operator LPSTR()
    {
        return _psz;
    }

    LPSTR _psz;
    int _nLen;
};

class CXStringW
{
public:
    CXStringW(LPCWSTR psz)
    {
        _psz = NULL;
        _nLen = 0;
        Init(psz);
    }

    CXStringW()
    {
        _psz = NULL;
        _nLen = 0;
    }

    ~CXStringW()
    {
        Clear();
    }

    BOOL Init(LPCWSTR psz, int nLen = -1)
    {
        Clear();

        if (psz)
        {
            if (nLen == -1)
                _nLen = lstrlen(psz);
            else
                _nLen = nLen;
            _psz = (LPWSTR)LocalAlloc(LPTR, (_nLen + 1) * sizeof(WCHAR));
            if (_psz)
            {
                StringCchCopyN(_psz, _nLen + 1, psz, _nLen);
            }
        }
        return TRUE;
    }

    void Clear()
    {
        if (_psz)
        {
            LocalFree(_psz);
            _psz = NULL;
            _nLen = 0;
        }
    }

    operator LPWSTR()
    {
        return _psz;
    }

    LPWSTR _psz;
    int _nLen;
};

class CBstr
{
public:
    CBstr(BSTR bstr)
    {
       _bstr = bstr;
    }

    CBstr(LPCWSTR psz)
    {
       _bstr = SysAllocString(psz);
    }

    CBstr()
    {
       _bstr = NULL;
    }

    ~CBstr()
    {
       Free();
    }

    void Init(BSTR bstr)
    {
        Free();
        _bstr = bstr;
    }

    void Free()
    {
        if (_bstr)
        {
           SysFreeString(_bstr);
           _bstr = NULL;
        }
    }

    ULONG GetLength()
    {
       if (!_bstr)
           return 0;
       return SysStringLen(_bstr);
    }

    operator BSTR*()
    {
        return &_bstr;
    }

    operator BSTR()
    {
        return _bstr;
    }

    operator LPCWSTR()
    {
        return _bstr;
    }

private:
    BSTR _bstr;
};


_inline BOOL HexStringToDword(LPCWSTR lpsz, DWORD &Value, int cDigits, WCHAR chDelim)
{
    int Count;

    Value = 0;
    for (Count = 0; Count < cDigits; Count++, lpsz++)
    {
        if (*lpsz >= '0' && *lpsz <= '9')
            Value = (Value << 4) + *lpsz - '0';
        else if (*lpsz >= 'A' && *lpsz <= 'F')
            Value = (Value << 4) + *lpsz - 'A' + 10;
        else if (*lpsz >= 'a' && *lpsz <= 'f')
            Value = (Value << 4) + *lpsz - 'a' + 10;
        else
            return(FALSE);
    }

    if (chDelim != 0)
        return *lpsz++ == chDelim;
    else
        return TRUE;
}
