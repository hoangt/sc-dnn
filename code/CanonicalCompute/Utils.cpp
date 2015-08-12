#include "Utils.h"

void CanonicalConfig::Init()
{
  _threadCount = DEFAULT_THREAD_COUNT;
  _sampleCount = DEFAULT_SAMPLE_COUNT;
  _workerCount = DEFAULT_WORKER_COUNT;
  _startLayer = DEFAULT_START_LAYER;
  _forwardSparsity = DEFAULT_SPARSITY;
  _backwardSparsity = DEFAULT_SPARSITY;
  _deltaComputeSparsity = DEFAULT_SPARSITY;
  _weightUpdateSparsity = DEFAULT_SPARSITY;
  _modelType = DEFAULT_MODEL_TYPE;
  _replicatedOutputLayer = true;
  _deltaWeightOpt = true;
  _training = true;
  _affinity = true;
  _sparseKernelVersion = 0;
}

void CanonicalConfig::Print()
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
	 "SparseKernelsVersion: %d\n"
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
	 (_sparseKernelVersion),
	 _forwardSparsity,
	 _backwardSparsity,
	 _deltaComputeSparsity,
	 _weightUpdateSparsity
	 );
	fflush(stdout);
}

void DNN::Init (int nLayers, LayerConfig *lp, int nWorkers, bool replicate)
{
  _nLayers = nLayers;
  _Layers = new Layer[nLayers];
  _Replicated = new bool[nLayers];
  _nWorkers = nWorkers;
  _nThreads = new int[nLayers];
  for (int i = g_CanonicalConfig._startLayer; i < nLayers; i++)
    {
      if (i == (nLayers - 1))
	{
	  _Replicated[i] = replicate;
	  _Layers[i].Init(lp[i]._OutputFeature, lp[i]._Input2Height, (replicate) ? lp[i]._Input2Width : lp[i]._Input2Width/nWorkers, lp[0]._FeedForwardSparsity,
			  lp[i]._BackPropSparsity, lp[i]._DeltaComputeSparsity, lp[i]._WeightUpdateSparsity);
	  _nThreads[i] = (replicate) ? (int)(ceil((float)G_THREAD_COUNT/nWorkers)) : G_THREAD_COUNT;
	}
      else {
	_Replicated[i] = false;
	_Layers[i].Init(lp[i]._OutputFeature, lp[i]._Input2Height, lp[i]._Input2Width, lp[i]._FeedForwardSparsity,
			lp[i]._BackPropSparsity, lp[i]._DeltaComputeSparsity, lp[i]._WeightUpdateSparsity);
	_nThreads[i] = G_THREAD_COUNT;
      }
    }		
}

void DNN::Fini(void)
{
  for (int i = G_START_LAYER; i < _nLayers; i++)
    {
      delete[] _Layers[i]._Weights;
      _Layers[i]._Weights = NULL;
    }
  _nLayers = 0;		
  delete [] _Layers;
  _Layers = NULL;
}

void DNN::Print(const char *modelName)
{
  for (int i = G_START_LAYER; i < _nLayers; i++)
    {
      printf("%s Config%dW \t Layer%d: \t %10d \t %10d \t %10d \t %10d\n", modelName, _nWorkers, i, _Layers[i]._OutputFeature, _Layers[i]._Input2Height, _Layers[i]._Input2Width, _Layers[i]._Connections);
      printf("%s Archit%dW \t Layer%d: \t %10d \t %10d \t %10d \n", modelName, _nWorkers, i, _Layers[i]._InputSize, _Layers[i]._OutputFeature*_Layers[i]._Input2Width, _Layers[i]._OutputSize);
    }
  fflush(stdout);
}

void Layer::Init (int of, int i2h, int i2w, int ffs, int bps, int dcs, int wus)
{		
  _OutputFeature = of;
  _Input2Height = i2h;
  _Input2Width = i2w;
  _Weights = new float[_OutputFeature * i2w];
  _InputSize = i2w * i2h;
  _OutputSize = of * i2h;
  _WeightSize = _OutputFeature * _Input2Width;
  _Connections = of * i2h * i2w;
  _FeedForwardSparsity = ffs;
  _BackPropSparsity = bps;
  _DeltaComputeSparsity = dcs;
  _WeightUpdateSparsity = wus;
}

void ThreadLayerState::Init (int tNum, DNN& model)
{
  _threadNum = tNum;
  _LayerState = model._Layers;
  _FLOPTime = new double[model._nLayers];
  _SampleCount = new int[model._nLayers];
  for (int i = 0; i < model._nLayers; i++)
    {
      _FLOPTime[i] = 0;
      _SampleCount[i] = 0;
    }
  _startLayer = G_START_LAYER;
  _numLayers = model._nLayers;	
  MY_ASSERT(_startLayer < _numLayers);
}

void ThreadLayerState::Fini (void)
{
  if (_FLOPTime != NULL) 
    {
      delete [] _FLOPTime;
      _FLOPTime = NULL;
    }
}
