#include<limits>
#include <vector>
#include <random>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <assert.h>
#include "DataProvider.h"

using namespace std;

void InitDataProvider()
{
    srand(static_cast<unsigned> (time(0)));
}

void Sparsify(float* data, const int size, const int sparsePercent)
{
    for (size_t j = 0; j < size; j++)
    {
        int value = rand() % 100;
        data[j] = (value < sparsePercent) ? 0.0f : static_cast<float>(value);
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