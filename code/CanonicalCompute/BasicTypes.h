#pragma once

#pragma region Includes

#include <SDKDDKVer.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <MSWSock.h>
#include <windows.h>
#include <new.h>
#include <stdio.h>
#include <emmintrin.h>
#include <mmintrin.h>
#include <typeinfo>

#pragma endregion

#pragma region Logging/Assertion

#define Assert(condition)													                                            \
    do {                                                                                                                \
    if (!(condition)) {																				                    \
    printf("##FATALERROR## Assert Failed! File: %s, Line: %d, Condition: %s\n", __FILE__, __LINE__, #condition);        \
    fflush(stdout);                                                                                                     \
    DebugBreak();                                                                                                       \
    }                                                                                                                   \
    } while (0)

#pragma endregion

#pragma region Typedefs

typedef char int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;
typedef unsigned char uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#pragma endregion

#pragma region Macros

#define ALIGN_8 __declspec(align(8))
#define ALIGN_16 __declspec(align(16))
#define ALIGN_32 __declspec(align(32))
#define ALIGN_64 __declspec(align(64))

#pragma endregion

#pragma region Critical Section

// Critical section wrapper
class CCritSec
{
public:

    // Construct critical section
    CCritSec();

    // Destroy critical section
    ~CCritSec();

    // Enter critical section
    void Enter();

    // Leave critical section
    void Leave();

private:

    // Underlying critical section
    CRITICAL_SECTION _criteSec;

    // Friends
    friend class CAutoCritSec;
};

// Scoped critical section
class CAutoCritSec
{
public:

    // Create scoped critical section
    CAutoCritSec(CCritSec* pCritSec);

    // Destroy scoped critical section
    ~CAutoCritSec();

private:

    // Underlying critical section
    CCritSec* _pCritSec;
};

#pragma endregion

#pragma region Non-copyable

// Extend this class privately to prevent the target class from being copyable
class CNonCopyable
{   
protected:

    // Construct 
    CNonCopyable() {}

    // Destroy
    ~CNonCopyable() {}
private:  

    // Emphasize the following members are private
    CNonCopyable( const CNonCopyable& );
    const CNonCopyable& operator=( const CNonCopyable& );
};

#pragma endregion

#pragma region Ref Counter

// Ref counter interface
class CRefCountable : CNonCopyable
{
public:

    // Increment reference
    void IncRef();

    // Decrement reference
    void DecRef();

    // Destructor
    virtual ~CRefCountable();

    // Get ref count
    LONG64 GetRefCount() const
    {
        return _llRefCount;
    }

protected:

    // Constructor
    CRefCountable() 
        : _llRefCount(1) {}

    // Deleter
    virtual void Delete() = 0;

private:
    ALIGN_8 volatile LONG64 _llRefCount;
};

// Smart ref counter holder
template <class T> 
class CRef
{

public:

    // Construct ref counter holder
    CRef()
    {
        _p = NULL;
    }

    // Construct with a ref counter
    CRef(T* lp)
    {
        _p = lp;

        if (_p != NULL) {
            _RemovePtrConst(_p)->IncRef();
        }
    }

    // Copy constructor
    explicit CRef(const CRef<T>& ref)
    {
        _p = ref.p;

        if (_p != NULL) {
            _RemovePtrConst(_p)->IncRef();
        }
    }

    // Destroy ref counter holder
    ~CRef()
    {
        if (_p != NULL) {
            _RemovePtrConst(_p)->DecRef();
            _p = NULL;   // Make sure we AV in case someone is using CsRef after DecRef
        }
    }

    // Get underlying ref counter pointer
    operator T*() const
    {
        return _p;
    }

    // Get reference to held ref counter
    T& operator*() const
    {
        return *_p;
    }

    // Dereference the held ref counter
    T* operator->() const
    {
        return _p;
    }

    // Null checker
    bool operator!() const
    {
        return (_p == NULL);
    }

    // Less than
    bool operator<(T* pT) const
    {
        return (_p < pT);
    }

    // Greater than 
    bool operator>(T* pT) const
    {
        return (_p > pT);
    }

    // Less than or equal to
    bool operator<=(T* pT) const
    {
        return (_p <= pT);
    }

    // Greater than or equal to
    bool operator>=(T* pT) const
    {
        return (_p >= pT);
    }

    // Compare with another ref counter for equality
    bool operator==(T* pT) const
    {
        return (_p == pT);
    }

    // Compare with another ref counter for inequality 
    bool operator!=(T* pT) const
    {
        return (_p != pT);
    }

    // Set underlying ref counter
    CRef& Set(T* lp)
    {
        if (_p != lp) {
            if (lp != NULL) {
                _RemovePtrConst(lp)->IncRef();
            }

            if (_p != NULL) {
                _RemovePtrConst(_p)->DecRef();
            }

            _p = lp;
        }

        return *this;
    }

    // Assign underlying ref counter
    CRef& operator=(T* lp)
    {
        return Set(lp);
    }

    // Assign from another ref counter holder
    CRef& operator=(const CRef<T>& ref)
    {
        return Set(ref._p);
    }

    // Release the interface and set to NULL
    void Release()
    {
        T* pTemp = _p;
        if (pTemp != NULL) {
            _p = NULL;
            _RemovePtrConst(pTemp)->DecRef();
        }
    }

    // Attach to an existing interface (does not IncRef)
    void Attach(T* p2)
    {
        if (_p != NULL) {
            _RemovePtrConst(_p)->DecRef();
        }
        _p = p2;
    }

    // Detach the interface (does not DecRef)
    T* Detach()
    {
        T* pt = _p;
        _p = NULL;
        return pt;
    }

    // Transfer from another ref counter holder without incrementing ref count
    void TransferFrom(CRef<T>& source)
    {
        Attach(source.Detach());
    }

    // Transfer from another ref counter polymorphic
    template<class T2> void TransferFrom(CRef<T2>& source)
    {
        T2 *p2 = source.Detach();
        if (_p != NULL) {
            _RemovePtrConst(_p)->DecRef();
        }
        if (p2== NULL) {
            _p = NULL;
        } else {
            _p = dynamic_cast<T *>(p2);
            Assert(_p != NULL);
        }
    }

    // Get underlying ref counter
    T* Ptr() const
    {
        return _p;
    }

    // Check if NULL
    bool IsNull()
    {
        return (_p == NULL);
    }

private:

    // Underlying ref counter
    T* _p;

    // Remove const cast
    static __forceinline T* _RemovePtrConst(const T *pX)
    {
        return const_cast<T*>(pX);
    }
};

#pragma endregion



