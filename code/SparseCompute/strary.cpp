//
// strary.cpp
//
// CStructArray
//

#include "stdafx.h"
#include "strary.h"

#define StrPB(x) (_pb + ((x) * _iElemSize))

//+---------------------------------------------------------------------------
//
// Insert(int iIndex, int cElems)
//     iIndex should be within [0, _cElems]
//
// Grows the array to accomodate cElems at offset iIndex.
//
// The new cells are NOT initialized!
//
//----------------------------------------------------------------------------

BOOL CVoidStructArray::Insert(int iIndex, int cElems)
{
    BYTE *pb;
    int iSizeNew;

    // check int overflow.
    //  
    //  - iIndex must be equal or bigger than 0.
    //  - cElems must be bigger than 0.
    //  - iIndex must be smaller than _cElems. (iIndex is the index to insert elems.)
    //  - _cElems + cElems should not overlow max_int.
    //
    if ((iIndex < 0) ||
        // (cElems < 0) ||   // This condition can be convered by (_cElems > _cElems + cElems).
        (iIndex > _cElems) ||
        (_cElems > _cElems + cElems))
    {
        _ASSERT(0);
        return FALSE;
    }


    if (cElems == 0)
        return TRUE;

    // allocate space if necessary
    if (_iSize < _cElems + cElems)
    {
        // allocate 1.5x what we need to avoid future allocs
        iSizeNew = max(_cElems + cElems, _cElems + _cElems / 2);

        ULONG ulBufferSize;

        // if (FAILED(ULongMult(iSizeNew, _iElemSize, &ulBufferSize))) {
        //     return FALSE;
        // }
        ulBufferSize = iSizeNew * _iElemSize;

        if ((pb = (_pb == NULL) ? 
                   (BYTE *)LocalAlloc(LPTR, ulBufferSize) :
                   (BYTE *)LocalReAlloc(_pb, ulBufferSize, LMEM_MOVEABLE | LMEM_ZEROINIT))
            == NULL)
        {
            return FALSE;
        }

        _pb = pb;
        _iSize = iSizeNew;
    }

    if (iIndex < _cElems)
    {
        // make room for the new addition
        memmove(StrPB(iIndex + cElems), 
                StrPB(iIndex), 
                (_cElems - iIndex)*_iElemSize);
#ifdef DEBUG
        memset(StrPB(iIndex), 0xFE, cElems * _iElemSize);
#endif
    }

    _cElems += cElems;
    _ASSERT(_iSize >= _cElems);

    return TRUE;
}

//+---------------------------------------------------------------------------
//
// Remove(int Index, int cElems)
//
// Removes cElems at offset iIndex.
//
//----------------------------------------------------------------------------

void CVoidStructArray::Remove(int iIndex, int cElems)
{
    BYTE *pb;
    int iSizeNew;

    _ASSERT(cElems > 0);
    _ASSERT(iIndex >= 0);
    _ASSERT(iIndex + cElems <= _cElems);

    if (iIndex + cElems < _cElems)
    {
        // shift following eles left
        memmove(StrPB(iIndex), 
                StrPB(iIndex + cElems), 
                (_cElems - iIndex - cElems) * _iElemSize);
#ifdef DEBUG
        memset(StrPB(_cElems - cElems), 0xFE, cElems * _iElemSize);
#endif
    }

    _cElems -= cElems;

    // free mem when array contents uses less than half alloc'd mem
    iSizeNew = _iSize / 2;
    if (iSizeNew > _cElems)
    {
        if ((pb = (BYTE *)LocalReAlloc(_pb, iSizeNew * _iElemSize, LMEM_MOVEABLE | LMEM_ZEROINIT)) != NULL)
        {
            _pb = pb;
            _iSize = iSizeNew;
        }
    }
}

//+---------------------------------------------------------------------------
//
// Move
//
// Move an entry from one position to another, shifting other entries as
// appropriate to maintain the array size.
//
// The entry currently at iIndexNew will follow the moved entry on return.
//
// Returns the new index, which will be iIndexNew or iIndexNew - 1 if
// iIndexOld < iIndexNew.
//----------------------------------------------------------------------------

int CVoidStructArray::Move(int iIndexNew, int iIndexOld)
{
    int iSrc;
    int iDst;
    int iActualNew;
    void *pv;
    int c;
    int iRet;
    pv  = LocalAlloc(LPTR, _iElemSize); 
    if (!pv)
        return iIndexOld;

    _ASSERT(iIndexOld >= 0);
    _ASSERT(iIndexOld < _cElems);
    _ASSERT(iIndexNew >= 0);

    if (iIndexOld == iIndexNew)
    {
        iRet = iIndexOld;
        goto Exit;
    }

    memcpy(pv, GetPtr(iIndexOld), _iElemSize);

    if (iIndexOld < iIndexNew)
    {
        c = iIndexNew - iIndexOld - 1;
        iSrc = iIndexOld + 1;
        iDst = iIndexOld;
        iActualNew = iIndexNew - 1;
    }
    else
    {
        c = iIndexOld - iIndexNew;
        iSrc = iIndexOld - c;
        iDst = iIndexOld - c + 1;
        iActualNew = iIndexNew;
    }
    _ASSERT(iActualNew >= 0);
    _ASSERT(iActualNew < _cElems);

    memmove(GetPtr(iDst), GetPtr(iSrc), c*_iElemSize);

    memcpy(GetPtr(iActualNew), pv, _iElemSize);

    iRet = iActualNew;
Exit:
    LocalFree(pv);
    return iRet;
}
