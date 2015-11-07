#pragma once

#include "strary.h"

template<typename K>
class CVoidHashMap
{
public:
    bool _bSorted;
    CVoidHashMap(bool bSorted = FALSE)
    {
        _bSorted = _bSorted;
    }
    virtual ~CVoidHashMap(void)
    {
    }

    struct KV
    {
        K key;
        void *value;
    };

    BOOL Add(K key, void *value)
    {
        KV *pkv;
        if (_kvs.Count() == 0)
        {
            pkv = _kvs.Append(1);
            pkv->key = key;
            pkv->value = value;
            return TRUE;
        }

        int nMin =0;
        int nMax = _kvs.Count() - 1;
        int nInsert = 0;
        while (nMin <= nMax)
        {
            int nMid = (nMin + nMax) / 2;
            pkv = _kvs.GetPtr(nMid);
            if (pkv->key > key)
            {
                nMax = nMid - 1;
                nInsert = nMid;
            } 
            else if (pkv->key < key)
            {
                nMin = nMid + 1;
                nInsert = nMid + 1;
            }
            else 
            {
                if (!_bSorted)
                {
                    return FALSE;
                }
                nInsert = nMid + 1;
                break;
            }
        }

        if (nInsert >= _kvs.Count())
        {
            pkv = _kvs.Append(1);
        }
        else
        {
            _kvs.Insert(nInsert, 1);
            pkv = _kvs.GetPtr(nInsert);
        }
        pkv->key = key;
        pkv->value = value;
        return TRUE;

    }

    void *GetValue(K key)
    {
        if (_kvs.Count() == 0)
        {
            return NULL;
        }

        int nMin =0;
        int nMax = _kvs.Count() - 1;
        while (nMin <= nMax)
        {
            int nMid = (nMin + nMax) / 2;
            KV *pkv = _kvs.GetPtr(nMid);
            if (pkv->key == key)
            {
                return pkv->value;
            }
            if (pkv->key > key)
            {
                nMax = nMid - 1;
            } 
            else // if (pkv->key < key)
            {
                nMin = nMid + 1;
            }
        }
        return NULL;
    }

	BOOL IsKey(K key)
	{
        if (_kvs.Count() == 0)
        {
            return FALSE;
        }

        int nMin =0;
        int nMax = _kvs.Count() - 1;
        while (nMin <= nMax)
        {
            int nMid = (nMin + nMax) / 2;
            KV *pkv = _kvs.GetPtr(nMid);
            if (pkv->key == key)
            {
                return TRUE;
            }
            if (pkv->key > key)
            {
                nMax = nMid - 1;
            } 
            else // if (pkv->key < key)
            {
                nMin = nMid + 1;
            }
        }
        return FALSE;
	}

    void Clear()
    {
        _kvs.Clear();
    }

    void Remove(K key)
    {
        if (_kvs.Count() == 0)
        {
            return;
        }

        int nMin =0;
        int nMax = _kvs.Count() - 1;
        while (nMin <= nMax)
        {
            int nMid = (nMin + nMax) / 2;
            KV *pkv = _kvs.GetPtr(nMid);
            if (pkv->key == key)
            {
                _kvs.Remove(nMid, 1);
                return;
            }
            if (pkv->key > key)
            {
                nMax = nMid - 1;
            } 
            else // if (pkv->key < key)
            {
                nMin = nMid + 1;
            }
        }
        return;
    }

    int GetCount()
    {
        return _kvs.Count();
    }

    void *GetIndexOf(int index, K *pkey)
    {
        KV *pkv = _kvs.GetPtr(index);
        if (pkey != NULL)
        {
            *pkey = pkv->key;
        }
        return pkv->value;
    }
    CStructArray<KV> _kvs;
};

template<typename K, class V>
class CHashMap : public CVoidHashMap<K>
{
public:
    CHashMap() : CVoidHashMap(false) {}
    BOOL Add(K key, V *value) {return CVoidHashMap<K>::Add(key, value); }
    V *GetValue(K key) {return (V *)CVoidHashMap<K>::GetValue(key); }
    V *GetIndexOf(int index, K *pkey) {return (V *)CVoidHashMap<K>::GetIndexOf(index, pkey);}
};

template<typename K, class V>
class CSortedMap : public CVoidHashMap<K>
{
public:
    CSortedMap() : CVoidHashMap(true) {}
    BOOL Add(K key, V *value) {return CVoidHashMap<K>::Add(key, value); }
    V *GetValue(K key) {return (V *)CVoidHashMap<K>::GetValue(key); }
    V *GetIndexOf(int index, K *pkey) {return (V *)CVoidHashMap<K>::GetIndexOf(index, pkey);}
};