#ifndef ZERO_OPT_UTILS_H_
#define ZERO_OPT_UTILS_H_

#include<algorithm>
#include "pin.H"

struct TraceInfo {
  ADDRINT _startAddr;
  ADDRINT _endAddr;
  uint64_t _execCount;
  uint64_t _bblCount;
  uint64_t _instrCount;
  
  TraceInfo(const ADDRINT start, const ADDRINT end, const uint64_t bblCount, const uint64_t instrCount) : 
    _startAddr(start), _endAddr(end),  _execCount(0), _bblCount(bblCount), _instrCount(instrCount) {}
  TraceInfo(const ADDRINT start, const ADDRINT end) {TraceInfo(start, end, 0, 0); }
  TraceInfo() {TraceInfo(0, 0, 0, 0); }

};

typedef std::pair<ADDRINT, TraceInfo> TracePair;

struct SparsifyOp {
  float* _buffer;
  int _count;
  int _wordSparsity;
  int _cacheLineSparsity;

  SparsifyOp(); 
  SparsifyOp(ADDRINT argAddr);
  size_t Execute();
  size_t Execute(const int destOffset, const int repeatCount, const int dataCount);
  void SparsifyData(float* data, const int dataCount);
};

struct ZeroOptUtils {
  
  static void Init();

  static void SparsifyAppMem(ADDRINT argAddr);

  static void CountTraces(THREADID tid, ADDRINT ip);

  static void FiniTraceCount();

  static void AddTraceInfo(const struct TraceInfo& traceInfo);  
};

#endif // ZERO_OPT_UTILS_H_
