#include "Utils.h"
#include "DataProvider.h"

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
  _activationCacheLineSparsity = DEFAULT_SPARSITY;
  _signalCacheLineSparsity = DEFAULT_SPARSITY;
  _deltaCacheLineSparsity = DEFAULT_SPARSITY;
  _modelType = DEFAULT_MODEL_TYPE;
  _replicatedOutputLayer = true;
  _deltaWeightOpt = true;
  _training = true;
  _affinity = true;
  _sparseKernelVersion = 0;
  _zeroSignalOpt = true;
  EnableAllPasses();
}

void CanonicalConfig::EnableAllPasses()
{
    for (int i = 0; i < DNNPass::NUM_DNN_PASS; i++)
    {
        _enablePass[i] = true;
    }
    _passName = "ALL";
}

void CanonicalConfig::EnablePass(DNNPass pass, const char* name)
{
  MY_ASSERT(pass < DNNPass::NUM_DNN_PASS);
  for (int i = 0; i < DNNPass::NUM_DNN_PASS; i++)
  {
    _enablePass[i] = (i == pass) ? true : false;
  } 
  _passName = name;
}

const char* KernelNames[KernelVersion::KERNEL_VERSION_COUNT] = { "BASELINE", "UNROLL", "SW_SPARSE", "OPT_SW", "HW_SPARSE" };

void CanonicalConfig::Print()
{
  printf(
      "Model: %s \n"
      "Pass: %s \n"
      "WorkerCount: %d \n"
      "ThreadCount: %d \n"
     "SampleCount: %d \n"
     /*     "OutputLayer: %s \n"
     "DeltWeightOpt: %s\n"
     "Momentum: %s \n"
     "StartLayer: %d \n"
     "Task: %s \n"
     "Affinity: %s\n"
     "ZeroSignalOpt: %s\n" */
     "KernelVersion: %s\n"
     "FeedForwardSparsity: %d\n" 
	 "ActivationCacheLineSparsity: %d\n"
	 "BackPropSparsity: %d\n"
     "SignalCacheLineSparsity: %d\n"
     /*     "DeltaComputeSparsity: %d\n"*/
     "WeightUpdateSparsity: %d\n" 
     "DeltaCacheLineSparsity: %d\n"
     ,
     ModelName[_modelType],
     _passName,
     _workerCount,
     _threadCount,
     _sampleCount, 
     /*     (_replicatedOutputLayer ? "Replicated" : "Partitioned"),
     (_deltaWeightOpt ? "enabled" : "disabled"),
     (_deltaWeightOpt ? "enabled" : "disabled"),
     _startLayer,
     (_training ? "Training" : "Classify"),
     (_affinity ? "Enabled" : "Disabled"),
     (_zeroSignalOpt ? "Enabled" : "Disabled"), */
     KernelNames[((int)G_DNN_KERNEL_VERSION)],
     _forwardSparsity,
	 _activationCacheLineSparsity,
     _backwardSparsity,
     _signalCacheLineSparsity,
     /*_deltaComputeSparsity, */
     _weightUpdateSparsity,
     _deltaCacheLineSparsity
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
          _Layers[i].Init(lp[i]._OutputFeature, lp[i]._Input2Height, (replicate) ? lp[i]._Input2Width : lp[i]._Input2Width / nWorkers, lp[i]._FeedForwardSparsity,
                          lp[i]._BackPropSparsity, lp[i]._DeltaComputeSparsity, lp[i]._WeightUpdateSparsity, lp[i]._SignalCacheLineSparsity, lp[i]._DeltaCacheLineSparsity);
          _nThreads[i] = (replicate) ? (int)(ceil((float)G_THREAD_COUNT / nWorkers)) : G_THREAD_COUNT;
      }
      else {
          _Replicated[i] = false;
          _Layers[i].Init(lp[i]._OutputFeature, lp[i]._Input2Height, lp[i]._Input2Width, lp[i]._FeedForwardSparsity,
                          lp[i]._BackPropSparsity, lp[i]._DeltaComputeSparsity, lp[i]._WeightUpdateSparsity, lp[i]._SignalCacheLineSparsity, lp[i]._DeltaCacheLineSparsity);
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
#define OF_NAME "OUTF"
#define I2H_NAME "I2H"
#define I2W_NAME "I2W"
#define D_I2W_NAME "D_I2W"
#define S_I2W_NAME "S_I2W"
#define D_SIG_NAME "D_SGI"
#define S_SIG_NAME "S_SGI"
#define D_DWI_NAME "D_DWI"
#define S_DWI_NAME "S_DWI"
#define D_DSZ_NAME "D_DSZ"

    printf("\n%-40s %-10s \t %-10s \t %-10s \t %-10s \t %-10s \t %-10s \t %-10s \t %-10s \t %-10s \t %-10s \t %-10s\n", "MODEL", 
        OF_NAME, I2H_NAME, I2W_NAME, D_I2W_NAME, S_I2W_NAME, "CONNS", D_SIG_NAME, S_SIG_NAME, D_DWI_NAME, S_DWI_NAME, D_DSZ_NAME);

    for (int i = G_START_LAYER; i < _nLayers; i++)
    {
        printf("%s Config%dW \t Layer%d: \t %10d \t %10d \t %10d \t %10d \t %10d \t %10d \t %10d \t %10d \t %10d \t %10d \t %10d\n", 
            modelName, _nWorkers, i, _Layers[i]._OutputFeature, _Layers[i]._Input2Height, _Layers[i]._Input2Width, _Layers[i]._DenseInput2Width, _Layers[i]._SparseInput2Width, _Layers[i]._Connections, _Layers[i]._minDenseSignalIndex, _Layers[i]._minSparseSignalWordIndex, _Layers[i]._minDenseDeltaWordIndex, _Layers[i]._minSparseDeltaWordIndex, _Layers[i]._DenseDeltaSize);
    }
    printf("\n");
    //for (int i = G_START_LAYER; i < _nLayers; i++)
    //{
    //    printf("%s Archit%dW \t Layer%d: \t %10d \t %10d \t %10d \n", modelName, _nWorkers, i, _Layers[i]._InputSize, _Layers[i]._OutputFeature*_Layers[i]._Input2Width, _Layers[i]._OutputSize);
    //}
    fflush(stdout);
}


void Layer::Init (int of, int i2h, int i2w, int ffs, int bps, int dcs, int wus, int scls, int dcls)
{		
  _OutputFeature = of;
  _Input2Height = i2h;
  _Input2Width = i2w;
  _Weights = new float[CACHELINE_ALIGN(_OutputFeature * i2w)];
  _InputSize = i2w * i2h;
  _OutputSize = of * i2h;
  _WeightSize = _OutputFeature * _Input2Width;
  _Connections = of * i2h * i2w;
  _FeedForwardSparsity = ffs;
  _BackPropSparsity = bps;
  _DeltaComputeSparsity = dcs;
  _WeightUpdateSparsity = wus;

  // Assume sparse signal cache lines come, followed by sparse signal words, and finally dense signal words
  _minDenseSignalIndex = (G_ZERO_SIGNAL_OPT == false) ? 0 : (int)(bps * of * i2h * 0.01); 
  _minSparseSignalWordIndex = (G_ZERO_SIGNAL_OPT == false) ? 0 : (int)(scls * of * i2h * 0.01);
  MY_ASSERT (_minSparseSignalWordIndex <= _minDenseSignalIndex);

  // Assume sparse activations come first, followed by dense activations
  _SparseInput2Width = ffs * i2w * 0.01;
  _DenseInput2Width = i2w - _SparseInput2Width;
  MY_ASSERT((_SparseInput2Width +_DenseInput2Width) == _Input2Width);

  // Assume sparse weight delta cache lines come first, followed by sparse weight delta words, and finally dense weight delta words
  _minDenseDeltaWordIndex = (int)(wus * _WeightSize * 0.01);
  _minSparseDeltaWordIndex = (int)(dcls * _WeightSize * 0.01);
  MY_ASSERT(_minSparseDeltaWordIndex <= _minDenseDeltaWordIndex);
  _DenseDeltaSize = _WeightSize - _minDenseDeltaWordIndex;
}

void ThreadLayerState::Init (const int tNum, const DNN& model)
{
  _threadNum = tNum;
  _LayerState = model._Layers;
  _FLOPTime = new double[model._nLayers];
  _SampleCount = new int[model._nLayers];
  _startLayer = G_START_LAYER;
  _numLayers = model._nLayers;	
  MY_ASSERT(_startLayer < _numLayers);
  ResetStats();
  InitData();
}

void ThreadLayerState::ResetStats()
{
	for (int i = _startLayer; i < _numLayers; i++)
	{
		_FLOPTime[i] = 0;
		_SampleCount[i] = 0;
	}
}

void ThreadLayerState::InitData()
{
	_inputActivation = new float*[_numLayers];
	_outputActivation = new float*[_numLayers];
	_inputError = new float*[_numLayers];
	_outputError = new float*[_numLayers];
	_weightDeltas = new float*[_numLayers];


	for (int i = _startLayer; i < _numLayers; i++)
	{
		int alignedInputSize = CACHELINE_ALIGN(_LayerState[i]._InputSize);
		int alignedOutputSize = CACHELINE_ALIGN(_LayerState[i]._OutputSize);
		int alignedWeightSize = CACHELINE_ALIGN(_LayerState[i]._WeightSize);

		_inputActivation[i] = new float[alignedInputSize];
		_outputActivation[i] = new float[alignedOutputSize];
		_inputError[i] = new float[alignedOutputSize];
		_outputError[i] = new float[alignedInputSize];
		_weightDeltas[i] = new float[alignedWeightSize];

		Sparsify(_inputActivation[i], alignedInputSize, G_FORWARD_SPARSITY, G_ACTIVATION_CACHELINE_SPARSITY);
		Sparsify(_inputError[i], alignedOutputSize, G_BACKPROP_SPARSITY, G_SIGNAL_CACHELINE_SPARSITY);
		Sparsify(_weightDeltas[i], alignedWeightSize, G_WEIGHTUPDATE_SPARSITY, G_DELTA_CACHELINE_SPARSITY);
		Sparsify(_LayerState[i]._Weights, alignedWeightSize, 0, 0);
	}

	std::vector<const float*> activations(_numLayers);
	std::vector<const float*> errors(_numLayers);
	std::vector<const float*> deltas(_numLayers);
	std::vector<int> activationSize(_numLayers);
	std::vector<int> errorSize(_numLayers);
	std::vector<int> deltaSize(_numLayers);
	for (int i = _startLayer; i < _numLayers; i++)
	{
		activations[i] = _inputActivation[i];
		activationSize[i] = CACHELINE_ALIGN(_LayerState[i]._InputSize);
		errors[i] = _inputError[i];
		errorSize[i] = CACHELINE_ALIGN(_LayerState[i]._OutputSize);
		deltas[i] = _weightDeltas[i];
		deltaSize[i] = CACHELINE_ALIGN(_LayerState[i]._WeightSize);
	}
/*
	PrintSparseStatistics("InputActivations", activations, activationSize);
	PrintSparseStatistics("InputErrors", errors, errorSize);
	PrintSparseStatistics("WeightDeltas", deltas, deltaSize);
*/
}

void ThreadLayerState::FiniData()
{
	for (int i = _startLayer; i < _numLayers; i++)
	{
		delete[] _inputActivation[i];
		delete[] _outputActivation[i];

		delete[] _inputError[i];
		delete[] _outputError[i];

		delete[] _weightDeltas[i];
	}

	delete[] _inputActivation;
	delete[] _outputActivation;

	delete[] _inputError;
	delete[] _outputError;

	delete[] _weightDeltas;
}

void ThreadLayerState::Fini (void)
{
  if (_FLOPTime != NULL) 
    {
      delete [] _FLOPTime;
      _FLOPTime = NULL;
    }
  FiniData();
}

