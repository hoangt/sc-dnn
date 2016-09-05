#include<limits>
#include <vector>
#include <random>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <assert.h>
#include "string.h"
#include "DataProvider.h"

#ifdef ZSIM_BUILD
#include "zsim_hooks.h"
#endif 

using namespace std;


void InitDataProvider()
{
    srand(static_cast<unsigned> (time(0)));
}

static void CPU_Sparsify(float* data, const int size, const int wordSparsity, const int cacheLineSparsity)
{
    int numCacheLines = size / DATA_PER_CACHELINE;
    int remainderSparsePercent = (100 == cacheLineSparsity) ? 0 : 100 * (wordSparsity - cacheLineSparsity)/(100 - cacheLineSparsity);

    for (size_t i = 0; i < numCacheLines; i++)
    {
        float* cacheLine = data + (i * DATA_PER_CACHELINE);
        int lineProbability = rand() % 100;
        if (lineProbability < cacheLineSparsity)
        {
            memset(cacheLine, 0, sizeof(float)* DATA_PER_CACHELINE);
        }
        else
        {
            for (size_t j = 0; j < DATA_PER_CACHELINE; j++)
            {
                int wordProbability = rand() % 100;
                cacheLine[j] = (wordProbability < remainderSparsePercent) ? 0.0f : static_cast<float>(wordProbability);
            }
        }
    }  
}

#ifdef ZSIM_BUILD 
struct SparsifyOp {
  float* _buffer;
  int _count;
  int _wordSparsity;
  int _cacheLineSparsity;
  
  SparsifyOp(float* buffer, const int count, const int wordSparse, const int lineSparse):
    _buffer(buffer),
    _count(count),
    _wordSparsity(wordSparse),
    _cacheLineSparsity(lineSparse)
  {}  
  };

static void ZSIM_Sparsify(float* data, const int count, const int wordSparsity, const int cacheLineSparsity)
{
  SparsifyOp* op = new SparsifyOp(data, count, wordSparsity, cacheLineSparsity);
  zsim_sparsify((uint64_t)op);
  delete op;
}
#endif // ZSIM_BUILD 

void Sparsify(float* data, const int size, const int wordSparsity, const int cacheLineSparsity)
{
    assert(wordSparsity >= cacheLineSparsity);
#ifdef ZSIM_BUILD
    ZSIM_Sparsify(data, size, wordSparsity, cacheLineSparsity);
#else
    CPU_Sparsify(data, size, wordSparsity, cacheLineSparsity);
#endif
}

void Sparsify(std::vector<float>& data, const int wordSparsity)
{
    for (size_t j = 0; j < data.size(); j++)
    {
        int value = rand() % 100;
        data[j] = (value < wordSparsity) ? 0.0f : static_cast<float>(value);
    }
}
void GetSparseData(std::vector<std::vector<float>>& sparseData,  size_t unitCount, const size_t numElems, const int wordSparsity)
{
    sparseData.resize(numElems);
    for (size_t i = 0; i < numElems; i++)
    {
        sparseData[i].resize(unitCount);
        Sparsify(sparseData[i], wordSparsity);
    }
}

void PrintSparseStatistics(const char* tag, const std::vector<std::vector<float>>& sparseData)
{
    assert(sparseData.size() > 0);
    int minCount = std::numeric_limits<int>::max();
    int maxCount = std::numeric_limits<int>::min();
    int totalSparseCount = 0;
    for (size_t i = 0; i < sparseData.size(); i++)
    {
        int count = 0;
        for (size_t j = 0; j < sparseData[i].size(); j++)
        {
            if (sparseData[i][j] == 0.0f) count++;
        }
        minCount = std::min(minCount, count);
        maxCount = std::max(maxCount, count);
        totalSparseCount += count;
    }

    int unitCount = sparseData[0].size();
    float minSparsity = static_cast<float>(minCount) / static_cast<float>(unitCount);
    float maxSparsity = static_cast<float>(maxCount) / static_cast<float>(unitCount);
    float averageSparsity = static_cast<float>(totalSparseCount) / static_cast<float>(unitCount * sparseData.size());
    printf("SparseStatistics: %s Min:%5.2f Max:%5.2f Avg:%5.2f\n", tag, minSparsity, maxSparsity, averageSparsity);
}

void PrintSparseStatistics(const char* tag, const std::vector<const float*>& sparseData, const std::vector<int>& dataSize)
{
    assert(sparseData.size() > 0);
    int totalSparseCount = 0;
    int totalCacheLineSparseCount = 0;
    int dataCount = 0;
    int cacheLineDataCount = 0;

    for (size_t i = 0; i < sparseData.size(); i++)
    {
        int layerCount = 0;
        int numCacheLines = dataSize[i] / DATA_PER_CACHELINE;
        for (size_t j = 0; j < numCacheLines; j++)
        {
            const float* cacheLine = sparseData[i] + (j * DATA_PER_CACHELINE);
            int count = 0;
            for (size_t k = 0; k < DATA_PER_CACHELINE; k++)
            {
                if (cacheLine[k] == 0.0f) count++;
            }
            layerCount += count;
            if (count == DATA_PER_CACHELINE) totalCacheLineSparseCount++;
        }

        totalSparseCount += layerCount;
        dataCount += dataSize[i];
        cacheLineDataCount += numCacheLines;
    }

    float averageSparsity = static_cast<float>(totalSparseCount) / static_cast<float>(dataCount);
    float averageCacheLineSparsity = static_cast<float>(totalCacheLineSparseCount) / static_cast<float> (cacheLineDataCount);
#ifdef M5_BUILD 
    int percentSparsity = static_cast<int>(averageSparsity * 100);
    int percentCacheLineSparsity = static_cast<int>(averageCacheLineSparsity * 100);
    printf("SparseStatistics: %s Word:%d%% Line:%d%%\n", tag, percentSparsity, percentCacheLineSparsity);
#else
    printf("SparseStatistics: %s Word:%5.2f $Line:%5.2f\n", tag, averageSparsity, averageCacheLineSparsity);
#endif
}
