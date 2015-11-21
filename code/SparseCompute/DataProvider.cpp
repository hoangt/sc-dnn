#include<limits>
#include <vector>
#include <random>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <assert.h>
#include "string.h"
#include "DataProvider.h"

using namespace std;


void InitDataProvider()
{
    srand(static_cast<unsigned> (time(0)));
}

void Sparsify(float* data, const int size, const int sparsePercent, const int sparsecacheLinePercent)
{
    assert(sparsePercent >= sparsecacheLinePercent);
    int numCacheLines = size / DATA_PER_CACHELINE;
    int remainderSparsePercent = 100 * (sparsePercent - sparsecacheLinePercent)/(100 - sparsecacheLinePercent);

    for (size_t i = 0; i < numCacheLines; i++)
    {
        float* cacheLine = data + (i * DATA_PER_CACHELINE);
        int lineProbability = rand() % 100;
        if (lineProbability < sparsecacheLinePercent)
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

void Sparsify(std::vector<float>& data, const int sparsePercent)
{
    for (size_t j = 0; j < data.size(); j++)
    {
        int value = rand() % 100;
        data[j] = (value < sparsePercent) ? 0.0f : static_cast<float>(value);
    }
}
void GetSparseData(std::vector<std::vector<float>>& sparseData,  size_t unitCount, const size_t numElems, const int sparsePercent)
{
    sparseData.resize(numElems);
    for (size_t i = 0; i < numElems; i++)
    {
        sparseData[i].resize(unitCount);
        Sparsify(sparseData[i], sparsePercent);
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
    printf("SparseStatistics: %s Word:%5.2f $Line:%5.2f\n", tag, averageSparsity, averageCacheLineSparsity);
}
