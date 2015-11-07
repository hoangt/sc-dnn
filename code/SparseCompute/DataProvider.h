#include <vector>

using namespace std;

void InitDataProvider();

void Sparsify(std::vector<float>& data, const int sparsePercent);

void Sparsify(float* data, const int size, const int sparsePercent);

void GetSparseData(std::vector<std::vector<float>>& sparseData, size_t unitCount, const size_t numElems, const int sparsePercent);

void PrintSparseStatistics(const char* tag, const std::vector<std::vector<float>>& sparseData);
