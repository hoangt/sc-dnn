#include "BasicTypes.h"

#pragma region Critical Section

CCritSec::CCritSec()
{
    InitializeCriticalSection(&_criteSec);
}

CCritSec::~CCritSec()
{
    DeleteCriticalSection(&_criteSec);
}

void CCritSec::Enter()
{
    EnterCriticalSection(&_criteSec);
}

void CCritSec::Leave()
{
    LeaveCriticalSection(&_criteSec);
}

CAutoCritSec::CAutoCritSec(CCritSec* pCritSec) 
    : _pCritSec(pCritSec)
{
    _pCritSec->Enter();
}

CAutoCritSec::~CAutoCritSec()
{
    _pCritSec->Leave();
}

#pragma endregion

#pragma region Ref Counter

void CRefCountable::IncRef()
{
    // Assert the reference was incremented from a valid reference
    Assert(InterlockedIncrement64(&_llRefCount) > 1);
}

void CRefCountable::DecRef()
{
    // Assert we have a valid reference prior to dec ref
    LONG64 llRefCount = InterlockedDecrement64(&_llRefCount);
    Assert(llRefCount >= 0);
    if (llRefCount == 0)
    {
        // Delete self
        Delete();
    }
}

CRefCountable::~CRefCountable()
{
    // If calling the destructor , ref count must be zero
    Assert(_llRefCount == 0);
}

#pragma endregion