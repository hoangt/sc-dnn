#include <vector>

using namespace std;

#define DATA_PER_CACHELINE 16

#define CACHELINE_ALIGN(N) ((N + (DATA_PER_CACHELINE - 1)) & ~(DATA_PER_CACHELINE - 1))

void InitDataProvider();

void Sparsify(std::vector<float>& data, const int sparsePercent);

void Sparsify(float* data, const int size, const int sparsePercent, const int sparseCacheLinePercent);

void GetSparseData(std::vector<std::vector<float>>& sparseData, size_t unitCount, const size_t numElems, const int sparsePercent);

void PrintSparseStatistics(const char* tag, const std::vector<std::vector<float>>& sparseData);

void PrintSparseStatistics(const char* tag, const std::vector<const float*>& sparseData, const std::vector<int>& dataSize);
