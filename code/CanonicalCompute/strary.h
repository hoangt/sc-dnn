//
// strary.h
//
// CStructArray -- growable struct array
//

#pragma once


#include "stdafx.h"
// #include <intsafe.h>

class CVoidStructArray
{
public:
    CVoidStructArray(int iElemSize, int iInitSize = 0)
    {         
        _ASSERT(iElemSize > 0);
        _ASSERT(iInitSize >= 0);
        if (iElemSize <= 0 ||
            iInitSize < 0) {
            return;
        }

        _iElemSize = iElemSize; // Issue: iElemSize should be const in template
        _cElems = 0;

        _iSize = 0;
        _pb = NULL;

        if (iInitSize)
        {
            ULONG ulBufferSize;

            // no intsafe
            // if (FAILED(ULongMult(iInitSize, _iElemSize, &ulBufferSize))) {
            //     return;
            // }
            ulBufferSize = iInitSize * _iElemSize;

            if (_pb = (BYTE *)LocalAlloc(LPTR, ulBufferSize))
                _iSize = iInitSize;
        }
    } 
    virtual ~CVoidStructArray() { LocalFree(_pb); }

    inline void *GetPtr(int iIndex)
    {
        _ASSERT(iIndex >= 0);
        _ASSERT(iIndex <= _cElems); // there's code that uses the first invalid offset for loop termination

        return _pb + (iIndex * _iElemSize);
    }

    BOOL Insert(int iIndex, int cElems);
    void Remove(int iIndex, int cElems);
    int Move(int iIndexNew, int iIndexOld);
    void *Append(int cElems)
    {
        return Insert(Count(), cElems) ? GetPtr(Count()-cElems) : NULL;
    }

    int Count()
    { 
        return _cElems; 
    }

    void Clear()
    { 
        LocalFree(_pb);
        _pb = NULL;
        _cElems = _iSize = 0;
    }

    void Reset(int iMaxSize)
    {
        BYTE *pb;
        ULONG ulBufferSize;

        _cElems = 0;        

        if (_iSize <= iMaxSize)
            return;
        _ASSERT(_pb != NULL); // _iSize should be zero in this case

        if (iMaxSize <=0 || _iElemSize <= 0) {
            return;
        }

        // if (FAILED(ULongMult(iMaxSize, _iElemSize, &ulBufferSize))) {
        //     return;
        // }
        ulBufferSize = iMaxSize * _iElemSize;

        if ((pb = (BYTE *)LocalReAlloc(_pb, ulBufferSize, LMEM_MOVEABLE | LMEM_ZEROINIT))
            == NULL)
        {
            return;
        }

        _pb = pb;
        _iSize = iMaxSize;
    }

protected:
    BYTE *_pb;   // the array
    int _cElems;    // num eles in the array
    int _iElemSize;    // num eles in the array
    int _iSize;     // actual size (in void *'s) of the array
};



//
// typesafe version
//
template<class T>
class CStructArray : public CVoidStructArray
{
public:
    CStructArray(int nInitSize = 0):CVoidStructArray(sizeof(T), nInitSize) {}

    T *GetPtr(int iIndex) { return (T *)CVoidStructArray::GetPtr(iIndex); }
    T *Append(int cElems) { return (T *)CVoidStructArray::Append(cElems); }
    BOOL Append(T &p)
    {
        T* pDest = Append(1);
        if (pDest == NULL)
        {
            return FALSE;
        }
        *pDest = p;
        return TRUE;
    }
    T *AppendClear(int cElems) 
    { 
        T *pv = (T *)CVoidStructArray::Append(cElems); 
        if (pv)
            memset(pv, 0, _iElemSize * cElems);
        return pv;
    }

    CStructArray<T> *Clone()
    {
        CStructArray<T> *pNew = new CStructArray<T>();
        if (!pNew)
            return NULL;

        T *pb = pNew->Append(_cElems);
        if (!pb)
        {
            delete pNew;
            return NULL;
        }

        memcpy(pb, _pb, _iElemSize * _cElems);
        return pNew;
    }
};

//
// GUID version
//
class CGUIDArray : private CVoidStructArray
{
public:
    CGUIDArray():CVoidStructArray(sizeof(GUID)) {}

    int Count() { return _cElems; }

    GUID *GetPtr(int iIndex) { return (GUID *)CVoidStructArray::GetPtr(iIndex); }
    GUID *Append(int cElems) { return (GUID *)CVoidStructArray::Append(cElems); }

    int InsertGuid(const GUID *pguid)
    {
        int nIndex;
        if (Find(pguid, &nIndex) != -1)
            return nIndex;

        nIndex++;

        if (!Insert(nIndex, 1))
            return -1;

        //
        // Insert GUID at last array
        //
        *(((GUID *)_pb) + nIndex) = *pguid;
     
        return nIndex;
    }

    int RemoveGuid(const GUID *pguid)
    {
        int nIndex = Find(pguid, NULL);
        if (nIndex == -1)
            return -1;

        Remove(nIndex, 1);

        return nIndex;
    }

    int Find(const GUID *pguid, int *piOut) 
    {
        int iMatch = -1;
        int iMid = -1;
        int iMin = 0;
        int iMax = _cElems;
        LONG l;

        while(iMin < iMax)
        {
            iMid = (iMin + iMax) / 2;
            l = memcmp(pguid, ((GUID *)_pb) + iMid, sizeof(GUID));

            if (l < 0)
            {
                iMax = iMid;
            }
            else if (l > 0)
            {
                iMin = iMid + 1;
            }
            else 
            {
                iMatch = iMid;
                break;
            }
        }

        if (piOut)
        {
            if ((iMatch == -1) && (iMid >= 0))
            {
                if (memcmp(pguid, ((GUID *)_pb) + iMid, sizeof(GUID)) < 0)
                    iMid--;
            }
            *piOut = iMid;
        }
        return iMatch;
    }

    void Clear()
    {
        CVoidStructArray::Clear();
    }
};

//
// Ref-counted version.
//
// Note: this is limited, because there's no dtor for struct elements.
//
template<class T>
class CSharedStructArray : public CStructArray<T>
{
public:
    CSharedStructArray() : CStructArray<T>()
    {
        _cRef = 1;
    }

    void _AddRef()
    { 
        _cRef++;
    }

    void _Release()
    {   
        _ASSERT(_cRef > 0);

        if (--_cRef == 0)
        {
            delete this;
        }
    }

private:
    LONG _cRef;
};

