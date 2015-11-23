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
#define G_ZERO_SIGNAL_OPT g_CanonicalConfig._zeroSignalOpt 
#define G_ACTIVATION_CACHELINE_SPARSITY g_CanonicalConfig._activationCacheLineSparsity
#define G_SIGNAL_CACHELINE_SPARSITY g_CanonicalConfig._signalCacheLineSparsity
#define G_DELTA_CACHELINE_SPARSITY g_CanonicalConfig._deltaCacheLineSparsity
#define G_DNN_KERNEL_VERSION g_CanonicalConfig._kernelVersion
#define G_DNN_PASS_ENABLED(p) g_CanonicalConfig._enablePass[p]
#define G_THREAD_MODEL g_CanonicalConfig._threadModel
#define G_OUTPUT_SCALE g_CanonicalConfig._outputScale

typedef struct LayerConfig {
    int _OutputFeature;
    int _Input2Height;
    int _Input2Width;
    int _FeedForwardSparsity;
    int _BackPropSparsity;
    int _DeltaComputeSparsity;
    int _WeightUpdateSparsity;
    int _SignalCacheLineSparsity;
    int _DeltaCacheLineSparsity;

    void InitSparsity(int forward, int backward, int deltaWeight, int weightUpdate, int signalCacheLine, int deltaCacheLine)
    {
        _FeedForwardSparsity = forward;
        _BackPropSparsity = backward;
        _DeltaComputeSparsity = deltaWeight;
        _WeightUpdateSparsity = weightUpdate;
        _SignalCacheLineSparsity = signalCacheLine;
        _DeltaCacheLineSparsity = deltaCacheLine;
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

  int _minDenseSignalIndex;
  int _minSparseSignalWordIndex;
  int _SparseInput2Width;
  int _DenseInput2Width;

  int _minSparseDeltaWordIndex;
  int _minDenseDeltaWordIndex;
  int _DenseDeltaSize;

  void Init (int of, int i2h, int i2w, int ffs, int bps, int dcs, int wus, int scls, int dcls);
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

  float** _inputActivation;
  float** _outputActivation;

  float** _inputError;
  float** _outputError;

  float** _weightDeltas;
  
  void Init(const int, const DNN&);
  void InitData();
  void PrintSparsity();
  void FiniData();
  void ResetStats();
  void Fini(void);
} ThreadLayerState;

typedef void(*FeedForward) (Layer*, const float* input, float* output);
typedef void(*BackPropagate) (Layer*, const float* input, float* output);
typedef void(*ComputeWeightDelta_2D) (Layer*, float* weightDeltas, const float* activation, const float* errorGradients);
typedef void(*ComputeWeightDelta_3D) (Layer*, float* weightDeltas, const float* activation, const float* errorGradients);
typedef void(*WeightUpdate) (Layer*, const float* weightDeltas);

struct DNNKernels {
    FeedForward _feedForward;
    BackPropagate _backPropagate;
    ComputeWeightDelta_2D _computeWeightDelta_2D;
    ComputeWeightDelta_3D _computeWeightDelta_3D;
    WeightUpdate _weightUpdate;
};

typedef enum {
    BASELINE = 0,
    UNROLL,
    SPARSE,
    OPTIMAL_SW,
    HARDWARE,
    KERNEL_VERSION_COUNT
} KernelVersion;

void InitDNNKernels(DNNKernels& dnnKernels, const KernelVersion);

void SetTrainingThreadAffinity(int);
void SetCanonicalConfig(int argc, char *argv[], CanonicalConfig& config);
ModelType ProcessModelParam(const char *modelString);
void ProcessPassParam(const char* passString);
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
#define DEFAULT_AFFINITY true
#define DEFAULT_KERNEL_VERSION (int) KernelVersion::BASELINE
#define DEFAULT_THREAD_MODEL 1
#define DEFAULT_OUTPUT_SCALE 1

struct CanonicalConfig {
    int _threadCount;
    int _sampleCount;
    int _workerCount;
    int _startLayer;
    int _forwardSparsity;
    int _backwardSparsity;
    int _deltaComputeSparsity;
    int _weightUpdateSparsity;
    int _signalCacheLineSparsity;
    int _deltaCacheLineSparsity;
    int _activationCacheLineSparsity;
    ModelType _modelType;
    bool _replicatedOutputLayer;
    bool _deltaWeightOpt;
    bool _training;
    bool _affinity;
    int _sparseKernelVersion;
    bool _zeroSignalOpt;
    KernelVersion _kernelVersion;
    bool _enablePass[DNNPass::NUM_DNN_PASS];
    const char* _passName;
    int _threadModel;
    int _outputScale;

    void Init();
    void Print();
    void EnableAllPasses();
    void EnablePass(DNNPass, const char* name);
};

