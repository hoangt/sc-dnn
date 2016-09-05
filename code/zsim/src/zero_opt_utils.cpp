#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "log.h"
#include "mtrand.h"
#include "zero_opt_utils.h"

static std::unordered_map<ADDRINT, TraceInfo> traceMap;
static uint64_t traceCount = 0;
static MTRand wordSparsityRNG;
static MTRand lineSparsityRNG;
const int c_DataPerCacheLine = 16;
const int c_MaxDataCount = 1 << 20;


// Sort traces in descending order of execution counts
static bool TraceCompare(const TracePair lhs, const TracePair rhs) {return lhs.second._execCount > rhs.second._execCount; }

void ZeroOptUtils::AddTraceInfo(const struct TraceInfo& traceInfo) {
  traceMap[traceInfo._startAddr] = traceInfo;
}

void ZeroOptUtils::CountTraces(THREADID tid, ADDRINT ip)
{
  if (traceMap.count(ip) > 0) {
    ++traceMap[ip]._execCount;
  }
  ++traceCount;
}

void ZeroOptUtils::FiniTraceCount()
{
  info("TraceCount Static %lu Execution %lu", traceMap.size(), traceCount);

  // Print top-10 traces
  info("%-30s %-12s %-12s %-12s %-12s", "Trace", "Count", "BBL", "INSTR", "PERC");
  std::vector<TracePair> traceVector(traceMap.begin(), traceMap.end()); 

  const double execPercBase = 0.01f * traceCount; 

  std::sort(traceVector.begin(), traceVector.end(), TraceCompare);
  for (size_t i = 0; i < std::min(traceVector.size(), 10ul); ++i) {
    const TraceInfo& tmp = traceVector[i].second;
    const double execPerc = tmp._execCount / execPercBase;
    info("[0x%lx--0X%lx] %12lu %12lu %12lu %8.2f", tmp._startAddr,tmp._endAddr, tmp._execCount, tmp._bblCount, tmp._instrCount, execPerc);
  }
}

SparsifyOp::SparsifyOp()
{
  _buffer = nullptr;
  _count = 0;
  _wordSparsity = 0;
  _cacheLineSparsity = 0;
}

void SparsifyOp::SparsifyData(float* data, const int dataCount)
{
  int numCacheLines = dataCount/c_DataPerCacheLine;
  int remainderSparsePercent = (100 == _cacheLineSparsity) ? 0 : 100 * (_wordSparsity - _cacheLineSparsity)/(100 - _cacheLineSparsity);
  
  for (int i = 0; i < numCacheLines; i++) {
    float* cacheLine = data + (i * c_DataPerCacheLine);
    int lineProbability =  lineSparsityRNG.randInt(99);
    if (lineProbability < _cacheLineSparsity) {
      memset(cacheLine, 0, sizeof(float) * c_DataPerCacheLine);
    }
    else {
      for (int j = 0; j < c_DataPerCacheLine; j++) {
	int wordProbability = wordSparsityRNG.randInt(99);
	cacheLine[j] = (wordProbability < remainderSparsePercent) ? 0.0f : static_cast<float>(wordProbability);
      }
    }
  }    
}

size_t SparsifyOp::Execute(const int destOffset, const int repeatCount, const int dataCount)
{
  size_t copySize = 0;
  const float* baseDestAddr = _buffer + destOffset;

  float* sparseData = new float[dataCount];
  SparsifyData(sparseData, dataCount);
  for (int i = 0; i < repeatCount; ++i) {
    VOID* dest = (VOID*)(baseDestAddr + (i * dataCount));
    copySize += PIN_SafeCopy(dest, sparseData, dataCount * sizeof(float));
  }
  delete(sparseData);

  return copySize;
}

size_t SparsifyOp::Execute()
{
  size_t sparseCopyBytes = 0;

  const int copyCount = _count % c_MaxDataCount;
  if (copyCount != 0) {
    sparseCopyBytes += Execute(0, 1, copyCount);
  }

  if (_count >= c_MaxDataCount) {
    const int repeatCount = _count / c_MaxDataCount;
    sparseCopyBytes += Execute(copyCount, repeatCount, c_MaxDataCount);
  }

  return sparseCopyBytes;
}

SparsifyOp::SparsifyOp(ADDRINT argAddr)
{  
#ifdef DEBUG_SPARSIFY_OP
  const size_t argCopyBytes = PIN_SafeCopy(this, (VOID *)argAddr, sizeof(SparsifyOp));
  const size_t bufferBytes = _count * sizeof(float);
  info("SparsifyOp: ArgCopyBytes=%lu Buffer=%p BufferBytes=%lu Count=%d WordSparsity=%d $LineSparsity=%d", 
       argCopyBytes, _buffer, bufferBytes, _count, _wordSparsity, _cacheLineSparsity);

   const size_t sparseCopyBytes = Execute();
   info("SparsifyOp: SparseCopyBytes=%lu", sparseCopyBytes);
#else
   PIN_SafeCopy(this, (VOID *)argAddr, sizeof(SparsifyOp));
   Execute();
#endif
}

void ZeroOptUtils::Init()
{
  wordSparsityRNG.seed();
  lineSparsityRNG.seed();
}

void ZeroOptUtils::SparsifyAppMem(ADDRINT argsAddr)
{
  SparsifyOp op(argsAddr);
}
