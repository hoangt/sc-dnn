#define WINDOWS_BUILD 1
#define USE_SPARSE_KERNELS 1

#ifdef WINDOWS_BUILD

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

#else

#endif

typedef enum WorkerCount {ZERO_WORKER = 0, ONE_WORKER, TWO_WORKER, THREE_WORKER, FOUR_WORKER, NUM_WORKER_COUNT} WorkerCount;
typedef enum ModelType {NO_MODEL = 0, MNIST_MODEL = 1, IMAGENET_1K_MODEL, IMAGENET_22K_MODEL, CIFAR_10_MODEL, IMAGENETKZ_1K_MODEL, NUM_MODEL_TYPE} ModelType;

struct CanonicalConfig;

extern int ModelLayerCount[];
extern const char* ModelName[];
void SetTrainingThreadAffinity(int);
void SetCanonicalConfig(int argc, char *argv[], CanonicalConfig& config);
ModelType ProcessModelParam(const char *modelString);


#define DEFAULT_SAMPLE_COUNT 1000
#define DEFAULT_THREAD_COUNT 1
#define DEFAULT_WORKER_COUNT 1
#define DEFAULT_START_LAYER 0
#define DEFAULT_SPARSITY 0
#define DEFAULT_MODEL_TYPE ModelType::MNIST_MODEL

struct CanonicalConfig {
	int _threadCount;
	int _sampleCount;
	int _workerCount;
	int _startLayer;
	int _feedFowardSparsity;
	int _backPropSparsity;
	int _deltaComputeSparsity;
	int _weightUpdateSparsity;
	ModelType _modelType;
	bool _replicatedOutputLayer;
	bool _deltaWeightOpt;
	bool _training;
	bool _affinity;

	void Init()
	{
		_threadCount = DEFAULT_THREAD_COUNT;
		_sampleCount = DEFAULT_SAMPLE_COUNT;
		_workerCount = DEFAULT_WORKER_COUNT;
		_startLayer = DEFAULT_START_LAYER;
		_feedFowardSparsity = DEFAULT_SPARSITY;
		_backPropSparsity = DEFAULT_SPARSITY;
		_deltaComputeSparsity = DEFAULT_SPARSITY;
		_weightUpdateSparsity = DEFAULT_SPARSITY;
		_modelType = DEFAULT_MODEL_TYPE;
		_replicatedOutputLayer = true;
		_deltaWeightOpt = true;
		_training = true;
		_affinity = true;
	}

	void Print()
	{
		printf("WorkerCount: %d \n"
			   "ThreadCount: %d \n"
			   "Model: %s \n"
			   "SampleCount: %d \n"
			   "OutputLayer: %s \n"
			   "DeltWeightOpt: %s\n"
			   "Momentum: %s \n"
			   "StartLayer: %d \n"
			   "Task: %s \n"
			   "Affinity: %s\n"
			   "FeedForwardSparsity: %d\n" 
			   "BackPropSparsity: %d\n"
			   "DeltaComputeSparsity: %d\n"
			   "WeightUpdateSparsity: %d\n",
			_workerCount,
			_threadCount,
			ModelName[_modelType], 
			_sampleCount, 
			(_replicatedOutputLayer ? "Replicated" : "Partitioned"), 
			(_deltaWeightOpt ? "enabled" : "disabled"),
			(_deltaWeightOpt ? "enabled" : "disabled"),
			_startLayer,
			(_training ? "Training" : "Classify"),
			(_affinity ? "Enabled" : "Disabled"),
			_feedFowardSparsity,
			_backPropSparsity,
			_deltaComputeSparsity,
			_weightUpdateSparsity
			);		
	}
};

