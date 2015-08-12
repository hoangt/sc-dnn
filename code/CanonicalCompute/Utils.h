#ifdef LINUX_BUILD

#include "assert.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "LinuxUtils.h"

#define MY_ASSERT assert
#define DECLARE_TIMER(t)      MyTimer t
#define START_TIMER(t)        (t.StartTimer())
#define STOP_TIMER(t)         (t.StopTimer())
#define ELAPSED_USEC_TIME(t)  (t.ElapsedUsecTime())
#define ATOMIC_INCREMENT64(v) (__atomic_fetch_add(&v, 1, __ATOMIC_SEQ_CST))
#define CASE_INSENSITIVE_STRCMP(a,b) strcasecmp(a,b)

typedef long long INT64;
typedef unsigned int DWORD;

enum WorkerCount {ZERO_WORKER = 0, ONE_WORKER, TWO_WORKER, THREE_WORKER, FOUR_WORKER, NUM_WORKER_COUNT};
enum ModelType {NO_MODEL = 0, MNIST_MODEL = 1, IMAGENET_1K_MODEL, IMAGENET_22K_MODEL, CIFAR_10_MODEL, IMAGENETKZ_1K_MODEL, NUM_MODEL_TYPE};
enum DNNPass {DNN_FORWARD = 0, DNN_BACKWARD, DNN_WEIGHTUPDATE, NUM_DNN_PASS};

#else // WINDOWS_BUILD 

#include "stdafx.h"
#include "paramparser.h"
#include "xstring.h"
#include "HiResTimer.h"

#define MY_ASSERT Assert
#define DECLARE_TIMER(t) CHiResTimer t
#define START_TIMER(t) t.Start()
#define STOP_TIMER(t) t.Stop()
#define ELAPSED_USEC_TIME(t) t.GetElapsedMicroSecs()
#define ATOMIC_INCREMENT64(v) InterlockedIncrement64(&v)
#define CASE_INSENSITIVE_STRCMP(a,b) _strcmpi(a,b)

typedef enum WorkerCount {ZERO_WORKER = 0, ONE_WORKER, TWO_WORKER, THREE_WORKER, FOUR_WORKER, NUM_WORKER_COUNT} WorkerCount;
typedef enum ModelType {NO_MODEL = 0, MNIST_MODEL = 1, IMAGENET_1K_MODEL, IMAGENET_22K_MODEL, CIFAR_10_MODEL, IMAGENETKZ_1K_MODEL, NUM_MODEL_TYPE} ModelType;
typedef enum DNNPass {DNN_FORWARD = 0, DNN_BACKWARD, DNN_WEIGHTUPDATE, NUM_DNN_PASS} DNNPass;

#endif

struct CanonicalConfig;

extern int ModelLayerCount[];
extern const char* ModelName[];
extern CanonicalConfig g_CanonicalConfig;

#define G_THREAD_COUNT g_CanonicalConfig._threadCount
#define G_SAMPLE_COUNT g_CanonicalConfig._sampleCount
#define G_WORKER_COUNT g_CanonicalConfig._workerCount
#define G_START_LAYER g_CanonicalConfig._startLayer
#define G_MODEL_TYPE g_CanonicalConfig._modelType
#define G_REPLICATED_OUTPUT_LAYER g_CanonicalConfig._replicatedOutputLayer
#define G_THREAD_AFFINITY g_CanonicalConfig._affinity
#define G_TRAINING g_CanonicalConfig._training
#define G_DELTA_WEIGHT_OPT g_CanonicalConfig._deltaWeightOpt
#define G_FORWARD_SPARSITY g_CanonicalConfig._forwardSparsity
#define G_BACKPROP_SPARSITY g_CanonicalConfig._backwardSparsity
#define G_DELTACOMPUTE_SPARSITY g_CanonicalConfig._deltaComputeSparsity
#define G_WEIGHTUPDATE_SPARSITY g_CanonicalConfig._weightUpdateSparsity
#define G_SPARSE_KERNEL_VERSION(v) (g_CanonicalConfig._sparseKernelVersion == v)
 
typedef struct LayerConfig {
    int _OutputFeature;
    int _Input2Height;
    int _Input2Width;
    int _FeedForwardSparsity;
    int _BackPropSparsity;
    int _DeltaComputeSparsity;
    int _WeightUpdateSparsity;

	void InitSparsity(int forward, int backward, int deltaWeight, int weightUpdate)
	{
		_FeedForwardSparsity = forward;
		_BackPropSparsity = backward;
		_DeltaComputeSparsity = deltaWeight;
		_WeightUpdateSparsity = weightUpdate;
	}
} LayerConfig;

typedef struct Layer {
  int _OutputFeature;
  int _Input2Height;
  int _Input2Width;
  int _AlignedInput2Width;
  float *_Weights;
  int _InputSize;
  int _OutputSize;
  int _WeightSize;
  int _Connections;
  int _FeedForwardSparsity;
  int _BackPropSparsity;
  int _DeltaComputeSparsity;
  int _WeightUpdateSparsity;

  void Init (int of, int i2h, int i2w, int ffs, int bps, int dcs, int wus);
} Layer;

typedef struct DNN {
  Layer *_Layers;
  int _nLayers;
  int _nWorkers;
  bool *_Replicated;
  int *_nThreads;
  
  void Init (int nLayers, LayerConfig *lp, int nWorkers, bool replicate);
  void Fini(void);
  void Print(const char*);
} DNN;

typedef struct ThreadLayerState {
  Layer *_LayerState;
  double* _FLOPTime;
  int *_SampleCount;
  int _numLayers;
  int _threadNum;
  int _startLayer;
  
  void Init(int, DNN&);
  void Fini(void);
} ThreadLayerState;


void SetTrainingThreadAffinity(int);
void SetCanonicalConfig(int argc, char *argv[], CanonicalConfig& config);
ModelType ProcessModelParam(const char *modelString);
void DoModelCompute(int numThreads, ThreadLayerState *tl, DNNPass dp);

DWORD DNNModelThreadWeightUpdate(ThreadLayerState *tl);
DWORD DNNModelThreadDeltaWeightUpdate(ThreadLayerState *tl);
DWORD DNNModelThreadBackward(ThreadLayerState *tl);
DWORD DNNModelThreadForward(ThreadLayerState *tl);

#define DEFAULT_SAMPLE_COUNT 1000
#define DEFAULT_THREAD_COUNT 1
#define DEFAULT_WORKER_COUNT 1
#define DEFAULT_START_LAYER 0
#define DEFAULT_SPARSITY 0
#define DEFAULT_MODEL_TYPE MNIST_MODEL

struct CanonicalConfig {
  int _threadCount;
  int _sampleCount;
  int _workerCount;
  int _startLayer;
  int _forwardSparsity;
  int _backwardSparsity;
  int _deltaComputeSparsity;
  int _weightUpdateSparsity;
  ModelType _modelType;
  bool _replicatedOutputLayer;
  bool _deltaWeightOpt;
  bool _training;
  bool _affinity;
  int _sparseKernelVersion;
  
  void Init();
  void Print();
};

