#include <iostream>
#include "zsim_hooks.h"

using namespace std;

static void roi_test()
{
  cout << "Start - ROI Test" << endl;
  zsim_roi_begin();
  zsim_heartbeat();
  zsim_roi_end();
  cout << "Stop  - ROI Test" << endl;
}

struct SparsifyOp {
  float* _buffer;
  int _count;
  int _wordSparsity;
  int _cacheLineSparsity;

  SparsifyOp() { SparsifyOp(0, 0, 0);}
  SparsifyOp(const int count, const int wordSparse, const int cacheLineSparse);
  void Init(const float value);
  int WordSparsity();
  int LineSparsity();
};

SparsifyOp::SparsifyOp(const int count, const int wordSparse, const int cacheLineSparse):
  _count(count),
  _wordSparsity(wordSparse),
  _cacheLineSparsity(cacheLineSparse)
{
  _buffer = new float[_count];
}

void SparsifyOp::Init(const float value)
{
  for (size_t i = 0; i < _count; ++i) {
    _buffer[i] = value;
  }
}

int SparsifyOp::WordSparsity()
{
  int sparseCount = 0;
  for (size_t i = 0; i < _count; ++i) {
    if (_buffer[i] == 0.0f) ++sparseCount;
  }
  return 100 * sparseCount / _count;
}

int SparsifyOp::LineSparsity()
{
  int sparseLines = 0;
  const int lineSize = 16;
  const int numLines = _count / lineSize;
  for (size_t line = 0;  line < numLines; ++line) {
    int sparseWords = 0;
    for (size_t word = 0; word < lineSize; ++word) {
      const int index = line * lineSize + word;
      if (_buffer[index] == 0.0f) ++sparseWords;
    }
    if (sparseWords == lineSize) ++sparseLines;
  }
  return 100 * sparseLines / numLines;
}

typedef struct SparsifyOp SparsifyOp;

static void sparsify_test()
{
    printf("Begin - Sparsify Test\n");
    
    for (int b = 0; b < 1; ++b) {
      for (int i = 0; i < 5; ++i) {
	const int wordSparse = 20 + (i * 20);
	const int lineSparse = wordSparse / 2;
	SparsifyOp* op = new SparsifyOp((1024 << b), wordSparse, lineSparse);
	op->Init(1.0f);
	printf("Pre : OpAddr=%p Buffer=%p Buffer[0]=%f WordSparse=%d LineSparse=%d \n", op, op->_buffer, op->_buffer[0], wordSparse, lineSparse);
	zsim_sparsify((uint64_t)op);
	printf("Post: OpAddr=%p Buffer=%p Buffer[0]=%f WordSparse=%d LineSparse=%d \n", op, op->_buffer, op->_buffer[0], op->WordSparsity(), op->LineSparsity());
      }
    }
    printf("End   - Sparsify Test\n");
}

int main() {
    cout << "C++ test" << endl;

    roi_test();
    sparsify_test();
    
    cout << "C++ test done" << endl;
    return 0;
}
