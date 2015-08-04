#include "stdafx.h"
#include "paramparser.h"
#include "xstring.h"
#include "HiResTimer.h"
#include <vector>

using namespace std;

extern "C"
{
	extern float avx2_mulsum_2_mem(const float *pf0, const float *pf1, INT64 count);
	extern void avx2_mulsum_3_mem(const float *pf0, const float *pf1, float f2, INT64 count);
	extern void avx2_fmemcpy(const float *pfSrc, float *pDst,  INT64 length);

	extern float mulsum2_base(const float *pf0, const float *pf1, INT64 count);
	extern float mulsum2_opt1_75_25(const float *pf0, const float *pf1, INT64 count);
	extern float mulsum2_opt1_50_50(const float *pf0, const float *pf1, INT64 count);
	extern float mulsum2_opt1_25_75(const float *pf0, const float *pf1, INT64 count);
	extern float mulsum2_opt1_0_100(const float *pf0, const float *pf1, INT64 count);

}
    struct ThreadAffinityConfig {
        BOOL _enabled;
        BOOL _hyperThreadingEnabled;
        DWORD _logicalProcessorCount;
        DWORD _physicalProcessorCount;
        std::vector<DWORD> _affinityMask;
        std::vector<DWORD> _logicalProcessor;

        ThreadAffinityConfig()
            : _enabled(false),
            _hyperThreadingEnabled(false),
            _logicalProcessorCount(0),
            _physicalProcessorCount(0)
        {
        }

        const char* ToString(const BOOL value) const
        {
            return value ? "Enabled" : "Disabled";
        }

        void Print(void) const
        {
            printf("ThreadAffinityConfig(%s): HyperThreading:%s LogicalProcessor:%u PhysicalProcessor:%u\n", ToString(_enabled), ToString(_hyperThreadingEnabled), _logicalProcessorCount, _physicalProcessorCount);
        }

        void Configure(const uint32 threadCount)
        {
            Assert(_enabled == TRUE);

            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            _logicalProcessorCount = sysinfo.dwNumberOfProcessors;

            _logicalProcessor.resize(threadCount);
            _affinityMask.resize(threadCount);

            DWORD bufferLength = 0;
            BOOL logicalProcessorInfo = GetLogicalProcessorInformationEx(LOGICAL_PROCESSOR_RELATIONSHIP::RelationProcessorCore, nullptr, &bufferLength);
            if ((logicalProcessorInfo == false) && (GetLastError() == ERROR_INSUFFICIENT_BUFFER) && (bufferLength > 0))
            {
                PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)malloc(bufferLength);
                logicalProcessorInfo = GetLogicalProcessorInformationEx(LOGICAL_PROCESSOR_RELATIONSHIP::RelationProcessorCore, buffer, &bufferLength);
                if (logicalProcessorInfo == TRUE)
                {
                    DWORD byteOffset = 0;
                    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX ptr = buffer;
                    while ((byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)) <= bufferLength)
                    {
                        if (ptr == nullptr) break;
                        if ((ptr->Relationship == LOGICAL_PROCESSOR_RELATIONSHIP::RelationProcessorCore) && (ptr->Processor.Flags == LTP_PC_SMT))
                        {
                            _hyperThreadingEnabled = TRUE;

                            break;
                        }
                        ptr++;
                        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
                    }
                }
            }
            _physicalProcessorCount = (_hyperThreadingEnabled) ? _logicalProcessorCount / 2 : _logicalProcessorCount;

            // Compute logical processor and affinity mask assignments of threads
            if (_hyperThreadingEnabled)
            {
                // The assignment goals for hyperthreading is to
                // (1) minimize physical core sharing to avoid L1 sharing
                // (2) maximize physical core utilization per socket to avoid sharing data through L3
                for (size_t t = 0; t < threadCount; t++)
                {
                    const DWORD physicalCoreId = (t % _physicalProcessorCount);
                    const DWORD virtualCoreId = ((t / _physicalProcessorCount) % 2);
                    _logicalProcessor[t] = ((physicalCoreId * 2) + virtualCoreId);
                    _affinityMask[t] = (1 << _logicalProcessor[t]);
                }
            }
            else
            {
                for (size_t t = 0; t < threadCount; t++)
                {
                    _logicalProcessor[t] = (t % _logicalProcessorCount);
                    _affinityMask[t] = (1 << _logicalProcessor[t]);
                }
            }
        }

        DWORD LogicalProcessor(const uint32 threadId)
        {
            Assert(_enabled && threadId < _logicalProcessor.size());
            return _logicalProcessor[threadId];
        }

        DWORD AffinityMask(const uint32 threadId)
        {
            Assert(_enabled && threadId < _affinityMask.size());
            return _affinityMask[threadId];
        }
    };

    ThreadAffinityConfig g_trainingThreadAffinity;

typedef struct Layer {
	int _OutputFeature;
	int _Input2Height;
	int _Input2Width;
	float *_Weights;
	int _InputSize;
	int _OutputSize;
	int _WeightSize;
	int _Connections;
        int _FeedForwardSparsity;
        void Init (int of, int i2h, int i2w, int ffs)
	{		
		_OutputFeature = of;
		_Input2Height = i2h;
		_Input2Width = i2w;
		_Weights = new float[_OutputFeature * _Input2Width];
		_InputSize = i2w * i2h;
		_OutputSize = of * i2h;
		_WeightSize = _OutputFeature * _Input2Width;
		_Connections = of * i2h * i2w;
                _FeedForwardSparsity = ffs;
	}
} Layer;
typedef struct LayerConfig {
	int _OutputFeature;
	int _Input2Height;
	int _Input2Width;
    int _FeedForwardSparsity;
} LayerConfig;

LayerConfig W1_Model_KZ_1K[8] = {{96, 63*63, 11*11*3, 0}, {256, 28*28, 25*96, 0}, {384, 12*12, 9*256, 0}, {384, 100, 9*384, 0}, {256, 64, 9*384, 0}, {4096, 1, 4096, 0}, {4096, 1, 4096, 0}, {4096, 1, 1000, 0}};

LayerConfig W4_Model_1K[7] = {{120, 41*41, 243, 0}, {250, 81, 3000, 0}, {400, 9, 2250, 0}, {600, 1, 3600, 0}, {250, 1, 600, 0}, {250, 1, 250, 0}, {1000, 1, 1000, 0}};
LayerConfig W2_Model_1K[7] = {{120, 85*41, 243, 0}, {250, 20*9, 3000, 0}, {400, 8*3, 2250, 0}, {600, 6, 3600, 0}, {500, 1, 1800, 0}, {500, 1, 500, 0}, {1000, 1, 1000, 0}};
LayerConfig W1_Model_1K[7] = {{120, 85*85, 243, 0}, {250, 20*20, 3000, 0}, {400, 8*8, 2250, 0}, {600, 6*6, 3600, 0}, {1000, 1, 5400, 0}, {1000, 1, 1000, 0}, {1000, 1, 1000, 0}};

LayerConfig W4_Model_22K[8] = {{120, 63*63, 49*3, 0}, {250, 14*14, 25*120, 0}, {400, 25, 9*250, 0}, {400, 9, 9*400, 0}, {600, 1, 9*400, 0}, {750, 1, 600, 0}, {750, 1, 750, 0}, {3000, 1, 22000, 0}};
LayerConfig W2_Model_22K[8] = {{120, 128*63, 49*3, 0}, {250, 30*14, 25*120, 0}, {400, 13*5, 9*250, 0}, {400, 11*3, 9*400, 0}, {600, 9, 9*400, 0}, {1500, 1, 3000, 0}, {1500, 1, 1500, 0}, {3000, 1, 22000, 0}};
LayerConfig W1_Model_22K[8] = {{120, 128*128, 49*3, 0}, {250, 30*30, 25*120, 0}, {400, 13*13, 9*250, 0}, {400, 11*11, 9*400, 0}, {600, 9*9, 9*400, 0}, {3000, 1, 15000, 0}, {3000, 1, 3000, 0}, {3000, 1, 22000, 0}};

LayerConfig W4_Model_MNIST[5] = {{10, 13*13, 25, 0}, {20, 3*3, 25*10, 0}, {80, 1, 100, 0}, {100, 1, 100, 0}, {400, 1, 10, 0}};
LayerConfig W2_Model_MNIST[5] = {{10, 29*13, 25, 0}, {20, 11*3, 25*10, 0}, {240, 1, 200, 0}, {200, 1, 200, 0}, {400, 1, 10, 0}};
LayerConfig W1_Model_MNIST[5] = {{10, 29*29, 25, 0}, {20, 11*11, 25*10, 0}, {720, 1, 400, 0}, {400, 1, 400, 0}, {400, 1, 10, 0}};

LayerConfig W4_Model_CIFAR_10[5] = {{64, 14*14, 25*3, 0}, {64, 3*3, 25*64, 0}, {256, 1, 512, 0}, {512, 1, 512, 0}, {2048, 1, 10, 0}};
LayerConfig W2_Model_CIFAR_10[5] = {{64, 32*14, 25*3, 0}, {64, 12*3, 25*64, 0}, {768, 1, 1024, 0}, {1024, 1, 1024, 0}, {2048, 1, 10, 0}};
LayerConfig W1_Model_CIFAR_10[5] = {{64, 32*32, 25*3, 0}, {64, 12*12, 25*64, 0}, {2034, 1, 2048, 0}, {2048, 1, 2048, 0}, {2048, 1, 10, 0}};

typedef enum WorkerCount {ZERO_WORKER = 0, ONE_WORKER, TWO_WORKER, THREE_WORKER, FOUR_WORKER, NUM_WORKER_COUNT} WorkerCount;
typedef enum ModelType {NO_MODEL = 0, MNIST_MODEL = 1, IMAGENET_1K_MODEL, IMAGENET_22K_MODEL, CIFAR_10_MODEL, IMAGENETKZ_1K_MODEL, NUM_MODEL_TYPE} ModelType;
LayerConfig *ModelConfig[NUM_MODEL_TYPE][NUM_WORKER_COUNT] = {
	NULL, NULL, NULL, NULL, NULL, 
	NULL, W1_Model_MNIST, W2_Model_MNIST, NULL, W4_Model_MNIST,
	NULL, W1_Model_1K, W2_Model_1K, NULL, W4_Model_1K,
	NULL, W1_Model_22K, W2_Model_22K, NULL, W4_Model_22K,
	NULL, W1_Model_CIFAR_10, W2_Model_CIFAR_10, NULL, W4_Model_CIFAR_10,
	NULL, W1_Model_KZ_1K, NULL, NULL, NULL,
};

int ModelLayerCount[NUM_MODEL_TYPE] = {0, 5, 7, 8, 5, 8};

const char *ModelName[NUM_MODEL_TYPE] = {"NO MODEL", "MNIST", "IMG1K", "IMG22K", "CIFAR10", "IMGKZ1K"};

#define DEFAULT_SAMPLE_COUNT 1000
#define DEFAULT_THREAD_COUNT 6
#define DEFAULT_WORKER_COUNT 1
#define DEFAULT_MODEL_TYPE ModelType::MNIST_MODEL

int g_ThreadCount;
INT64 g_SampleCount;
LONG64 g_CurrentSamplePos;
int g_WorkerCount;
ModelType g_ModelType;
bool g_ReplicateOutputLayer = true;
bool g_DeltaWeightOpt = true;
int g_StartLayer = 0;
bool g_Training = true;
int g_Sparsity = 0;

typedef struct DNN {
	Layer *_Layers;
	int _nLayers;
	int _nWorkers;
	bool *_Replicated;
	int *_nThreads;
	void Init (int nLayers, LayerConfig *lp, int nWorkers, bool replicate)
	{
		_nLayers = nLayers;
		_Layers = new Layer[nLayers];
		_Replicated = new bool[nLayers];
		_nWorkers = nWorkers;
		_nThreads = new int[nLayers];
		for (int i = g_StartLayer; i < nLayers; i++)
		{
			if (i == (nLayers - 1))
			{
				_Replicated[i] = replicate;
				_Layers[i].Init(lp[i]._OutputFeature, lp[i]._Input2Height, (replicate) ? lp[i]._Input2Width : lp[i]._Input2Width/nWorkers, lp[i]._FeedForwardSparsity);
				_nThreads[i] = (replicate) ? ceil((float)g_ThreadCount/nWorkers) : g_ThreadCount;
			}
			else {
				_Replicated[i] = false;
				_Layers[i].Init(lp[i]._OutputFeature, lp[i]._Input2Height, lp[i]._Input2Width, lp[i]._FeedForwardSparsity);
				_nThreads[i] = g_ThreadCount;
			}
		}		
	}
	void Fini(void)
	{
		for (int i = g_StartLayer; i < _nLayers; i++)
		{
			delete[] _Layers[i]._Weights;
			_Layers[i]._Weights = NULL;
		}
		_nLayers = 0;		
		delete [] _Layers;
		_Layers = NULL;
	}
	void Print(const char *modelName)
	{
		for (int i = g_StartLayer; i < _nLayers; i++)
		{
			printf("%s Config%dW \t Layer%d: \t %10d \t %10d \t %10d \t %10d\n", modelName, _nWorkers, i, _Layers[i]._OutputFeature, _Layers[i]._Input2Height, _Layers[i]._Input2Width, _Layers[i]._Connections);
			printf("%s Archit%dW \t Layer%d: \t %10d \t %10d \t %10d \n", modelName, _nWorkers, i, _Layers[i]._InputSize, _Layers[i]._OutputFeature*_Layers[i]._Input2Width, _Layers[i]._OutputSize);
		}
		fflush(stdout);
	}
} DNN;
DNN DNNModel;
typedef struct ThreadLayerState {
	Layer *_LayerState;
	double* _FLOPTime;
	int *_SampleCount;
	int _numLayers;
	int _threadNum;
	int _startLayer;
	void Init (int tNum, DNN& model)
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
		_startLayer = g_StartLayer;
		_numLayers = model._nLayers;	
		Assert(_startLayer < _numLayers);
	}
	void Fini (void)
	{
		if (_FLOPTime != NULL) 
		{
			delete [] _FLOPTime;
			_FLOPTime = NULL;
		}
	}
} ThreadLayerState;

double mulsum2_wrapper(Layer *layer, float *inpACT, float* outACT) {
    CHiResTimer timer;
    int sparsity = layer->_FeedForwardSparsity;
    if (sparsity == 0) {
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++) {
            for (int j = 0; j < layer->_Input2Height; j++) {
				outACT[i*layer->_Input2Height + j] = mulsum2_base(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width);
			}
		}
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity < 25) {
        INT64 second = layer->_Input2Width * (sparsity / 25);
        INT64 first = layer->_Input2Width - second;
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++) {
                 outACT[i*layer->_Input2Height + j] = mulsum2_base(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), first);
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_75_25(inpACT+(j*layer->_Input2Width) + first, layer->_Weights+(i*layer->_Input2Width) + first, second);
            }
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity == 25) {
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++)
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_75_25(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width);
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity < 50) {
        sparsity -= 25;
        INT64 second = layer->_Input2Width * (sparsity / 25);
        INT64 first = layer->_Input2Width - second;
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++) {
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_75_25(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), first);
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_50_50(inpACT+(j*layer->_Input2Width) + first, layer->_Weights+(i*layer->_Input2Width) + first, second);
            }
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity == 50) {
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++)
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_50_50(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width);
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity < 75) {
        sparsity -= 50;
        INT64 second = layer->_Input2Width * (sparsity / 25);
        INT64 first = layer->_Input2Width - second;
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++) {
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_50_50(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), first);
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_25_75(inpACT+(j*layer->_Input2Width) + first, layer->_Weights+(i*layer->_Input2Width) + first, second);
            }
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity == 75) {
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++)
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_25_75(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width);
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity < 100) {
        sparsity -= 75;
        INT64 second = layer->_Input2Width * (sparsity / 25);
        INT64 first = layer->_Input2Width - second;
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++) {
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_25_75(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), first);
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_0_100(inpACT+(j*layer->_Input2Width) + first, layer->_Weights+(i*layer->_Input2Width) + first, second);
            }
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
    else if (sparsity == 100) {
        timer.Start();
        for (int i = 0; i < layer->_OutputFeature; i++)
            for (int j = 0; j < layer->_Input2Height; j++)
                 outACT[i*layer->_Input2Height + j] = mulsum2_opt1_0_100(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width);
        timer.Stop();
        return timer.GetElapsedMicroSecs();
    }
	return 0.0f;
}


#define NUM_DNN_UNIT_FLOP 2
typedef enum DNNPass {DNN_FORWARD = 0, DNN_BACKWARD, DNN_WEIGHTUPDATE, NUM_DNN_PASS} DNNPass;
const char *DNNPassName[NUM_DNN_PASS] = {"ForwardProp", "BackwardProp", "WeightUpdate"};
DWORD DNNModelThreadForward(ThreadLayerState *tl)
{	
	if (g_trainingThreadAffinity._enabled)
    {
		const DWORD logicalProcessor = g_trainingThreadAffinity.LogicalProcessor(tl->_threadNum);
        const DWORD affinityMask = g_trainingThreadAffinity.AffinityMask(tl->_threadNum);
        SetThreadAffinityMask(GetCurrentThread(), affinityMask);
	}
	printf("ThreadForward: TID %d\n", tl->_threadNum);
	float **inputActivation = new float *[tl->_numLayers];
	float **outputActivation = new float *[tl->_numLayers];
	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		inputActivation[i] = new float[tl->_LayerState[i]._InputSize];
		outputActivation[i] = new float[tl->_LayerState[i]._OutputSize];
	}
	while (true)
	{
		INT64 sampleId = InterlockedIncrement64(&g_CurrentSamplePos);
		if (sampleId >= g_SampleCount) break;
		int numLayers = ((sampleId % g_WorkerCount) == 0) ? tl->_numLayers : tl->_numLayers-1;
		for (int l = tl->_startLayer; l < numLayers; l++)
		{
			float *inpACT = inputActivation[l];
			float *outACT = outputActivation[l];
			Layer *layer = (tl->_LayerState + l);
            double elapsedTime;
#if 0
            elapsedTime = mulsum2_wrapper(layer, outACT, inpACT);
#else                   
			CHiResTimer timer;
			timer.Start();
			for (int i = 0; i < layer->_OutputFeature; i++)
			{
				for (int j = 0; j < layer->_Input2Height; j++)
				{
					outACT[i*layer->_Input2Height + j] = avx2_mulsum_2_mem(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width);
				}
			}
			timer.Stop();
			elapsedTime = timer.GetElapsedMicroSecs();
			elapsedTime = mulsum2_wrapper(layer, inpACT, outACT);

#endif
			tl->_FLOPTime[l] += elapsedTime; // 
			tl->_SampleCount[l]++;
		}		
	}
	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		delete [] inputActivation[i];
		delete [] outputActivation[i];
	}
	delete []inputActivation;
	delete []outputActivation;
	return 0;
}
DWORD WINAPI s_DNNModelThreadForward(LPVOID lp)
{
	return DNNModelThreadForward((ThreadLayerState *)lp);
}

DWORD DNNModelThreadBackward(ThreadLayerState *tl)
{
	if (g_trainingThreadAffinity._enabled)
    {
		const DWORD logicalProcessor = g_trainingThreadAffinity.LogicalProcessor(tl->_threadNum);
        const DWORD affinityMask = g_trainingThreadAffinity.AffinityMask(tl->_threadNum);
        SetThreadAffinityMask(GetCurrentThread(), affinityMask);
	}

	float **inputActivation = new float *[tl->_numLayers];
	float **outputActivation = new float *[tl->_numLayers];
	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		inputActivation[i] = new float[tl->_LayerState[i]._InputSize];
		outputActivation[i] = new float[tl->_LayerState[i]._OutputSize];
	}
	while (true)
	{
		INT64 sampleId = InterlockedIncrement64(&g_CurrentSamplePos);
		if (sampleId >= g_SampleCount) break;
		int numLayers = ((sampleId % g_WorkerCount) == 0) ? tl->_numLayers : tl->_numLayers-1;
		for (int l = tl->_startLayer; l < numLayers; l++)
		{
			float *inpACT = inputActivation[l];
			float *outACT = outputActivation[l];
			Layer *layer = (tl->_LayerState + l);
			CHiResTimer timer;
			timer.Start();
			for (int i = 0; i < layer->_OutputFeature; i++)
			{
				for (int j = 0; j < layer->_Input2Height; j++)
				{
					avx2_mulsum_3_mem(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), outACT[i*layer->_Input2Height + j], layer->_Input2Width); 
				}
			}
			timer.Stop();
			tl->_FLOPTime[l] += timer.GetElapsedMicroSecs();
			tl->_SampleCount[l]++;
		}
	}
	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		delete [] inputActivation[i];
		delete [] outputActivation[i];
	}
	delete []inputActivation;
	delete []outputActivation;
	return 0;
}
DWORD WINAPI s_DNNModelThreadBackward(LPVOID lp)
{
	return DNNModelThreadBackward((ThreadLayerState *)lp);
}
DWORD DNNModelThreadDeltaWeightUpdate(ThreadLayerState *tl)
{
	if (g_trainingThreadAffinity._enabled)
    {
		const DWORD logicalProcessor = g_trainingThreadAffinity.LogicalProcessor(tl->_threadNum);
        const DWORD affinityMask = g_trainingThreadAffinity.AffinityMask(tl->_threadNum);
        SetThreadAffinityMask(GetCurrentThread(), affinityMask);
	}

	float **inputActivation = new float *[tl->_numLayers];
	float **outputActivation = new float *[tl->_numLayers];
	float **deltaWeights = new  float *[tl->_numLayers];
	float **weightMomentum = new  float *[tl->_numLayers];

	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		inputActivation[i] = new float[tl->_LayerState[i]._InputSize];
		outputActivation[i] = new float[tl->_LayerState[i]._OutputSize];
		deltaWeights[i] = new float[tl->_LayerState[i]._WeightSize];
		weightMomentum[i] = new float[tl->_LayerState[i]._WeightSize];
	}
	while (true)
	{
		INT64 sampleId = InterlockedIncrement64(&g_CurrentSamplePos);
		if (sampleId >= g_SampleCount) break;
		int numLayers = ((sampleId % g_WorkerCount) == 0) ? tl->_numLayers : tl->_numLayers-1;
		for (int l = tl->_startLayer; l < numLayers; l++)
		{
			float *inpACT = inputActivation[l];
			float *outACT = outputActivation[l];
			Layer *layer = (tl->_LayerState + l);
			CHiResTimer timer;
			timer.Start();
			avx2_mulsum_3_mem(deltaWeights[l], weightMomentum[l], 1.0f, tl->_LayerState[l]._WeightSize);
			if (layer->_Input2Height == 1)
			{
				for (int i = 0; i < layer->_OutputFeature; i++)
				{
					avx2_mulsum_3_mem(deltaWeights[l]+(i*layer->_Input2Width), inpACT, outACT[i], layer->_Input2Width); 
				}
			}
			else 
			{
				for (int i = 0; i < layer->_OutputFeature; i++)
				{
					for (int j = 0; j < layer->_Input2Height; j++)
					{									
						avx2_mulsum_3_mem(deltaWeights[l]+(i*layer->_Input2Width), inpACT+(j*layer->_Input2Width), outACT[i*layer->_Input2Height + j], layer->_Input2Width); 
					}
				}
            avx2_mulsum_3_mem(layer->_Weights, deltaWeights[l], 1.0f, layer->_OutputFeature * layer->_Input2Width);
			}
			avx2_fmemcpy(weightMomentum[l], deltaWeights[l], tl->_LayerState[l]._WeightSize);
			timer.Stop();
			tl->_FLOPTime[l] += timer.GetElapsedMicroSecs();
			tl->_SampleCount[l]++;
		}
	}
	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		delete [] inputActivation[i];
		delete [] outputActivation[i];
		delete [] deltaWeights[i];
	}
	delete []inputActivation;
	delete []outputActivation;
	delete [] deltaWeights;
	delete [] weightMomentum;
	return 0;
}

DWORD DNNModelThreadWeightUpdate(ThreadLayerState *tl)
{
	if (g_trainingThreadAffinity._enabled)
    {
		const DWORD logicalProcessor = g_trainingThreadAffinity.LogicalProcessor(tl->_threadNum);
        const DWORD affinityMask = g_trainingThreadAffinity.AffinityMask(tl->_threadNum);
        SetThreadAffinityMask(GetCurrentThread(), affinityMask);
	}

	float **inputActivation = new float *[tl->_numLayers];
	float **outputActivation = new float *[tl->_numLayers];
	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		inputActivation[i] = new float[tl->_LayerState[i]._InputSize];
		outputActivation[i] = new float[tl->_LayerState[i]._OutputSize];
	}
	while (true)
	{
		INT64 sampleId = InterlockedIncrement64(&g_CurrentSamplePos);
		if (sampleId >= g_SampleCount) break;
		int numLayers = ((sampleId % g_WorkerCount) == 0) ? tl->_numLayers : tl->_numLayers-1;
		for (int l = tl->_startLayer; l < numLayers; l++)
		{
			float *inpACT = inputActivation[l];
			float *outACT = outputActivation[l];
			Layer *layer = (tl->_LayerState + l);
			CHiResTimer timer;
			timer.Start();
			if (layer->_Input2Height == 1)
			{
				for (int i = 0; i < layer->_OutputFeature; i++)
				{
					avx2_mulsum_3_mem(layer->_Weights+(i*layer->_Input2Width), inpACT, outACT[i], layer->_Input2Width); 
				}
			}
			else 
			{
				for (int i = 0; i < layer->_OutputFeature; i++)
				{
					for (int j = 0; j < layer->_Input2Height; j++)
					{									
						avx2_mulsum_3_mem(layer->_Weights+(i*layer->_Input2Width), inpACT+(j*layer->_Input2Width), outACT[i*layer->_Input2Height + j], layer->_Input2Width); 
					}
				}
			}
			timer.Stop();
			tl->_FLOPTime[l] += timer.GetElapsedMicroSecs();
			tl->_SampleCount[l]++;
		}
	}
	for (int i = tl->_startLayer; i < tl->_numLayers; i++)
	{
		delete [] inputActivation[i];
		delete [] outputActivation[i];
	}
	delete []inputActivation;
	delete []outputActivation;
	return 0;
}
DWORD WINAPI s_DNNModelThreadWeightUpdate(LPVOID lp)
{
	if (g_DeltaWeightOpt)
	{
		return  DNNModelThreadDeltaWeightUpdate((ThreadLayerState *)lp);
	}
	return DNNModelThreadWeightUpdate((ThreadLayerState *)lp);
}

void runDNNModelThreads (int numThreads, DNNPass dp)
{
	HANDLE *helperThreads = new HANDLE[numThreads];
	ThreadLayerState *tl = new ThreadLayerState[numThreads];
	g_CurrentSamplePos = -1;
	for (int i = 0; i < numThreads; i++)
	{

		tl[i].Init(i, DNNModel);
	}
	for (int i = 0; i < numThreads; i++)
	{
		if (dp == DNN_FORWARD)
			helperThreads[i] = CreateThread(NULL, 0, s_DNNModelThreadForward, (LPVOID)(tl+i), 0, NULL);
		else if (dp == DNN_BACKWARD)
			helperThreads[i] = CreateThread(NULL, 0, s_DNNModelThreadBackward, (LPVOID)(tl+i), 0, NULL);
		else 
			helperThreads[i] = CreateThread(NULL, 0, s_DNNModelThreadWeightUpdate, (LPVOID)(tl+i), 0, NULL);
	}
	WaitForMultipleObjects(numThreads, helperThreads, TRUE, INFINITE);
	
	for (int i = 0; i < numThreads; i++)
	{
		CloseHandle(helperThreads[i]);
	}
	int *sampleCount = new int[DNNModel._nLayers];
	double *averageSampleTime = new double[DNNModel._nLayers];
	for (int i = g_StartLayer; i < DNNModel._nLayers; i++)
	{
		averageSampleTime[i] = 0;
		sampleCount[i] = 0;
		for (int j = 0; j < numThreads; j++)
		{
			averageSampleTime[i] += (INT64)(tl[j]._FLOPTime[i]);
			sampleCount[i] += tl[j]._SampleCount[i];
			//printf("Layer%d Thread%d FlopTime %I64d Count %d\n", i, j, (INT64)(tl[j]._FLOPTime[i]), tl[j]._SampleCount[i]);
			//fflush(stdout);
		}
		//printf("Layer%d Threads %d Count %d Time %10.2f\n", i, DNNModel._nThreads[i], sampleCount[i], averageSampleTime[i]);
		//averageSampleTime[i] /= (DNNModel._nThreads[i] * sampleCount[i]);
		//averageSampleTime[i] /= (DNNModel._nWorkers  * DNNModel._nThreads[i] * sampleCount[i]);
		averageSampleTime[i] /=  (DNNModel._Replicated[i] == true) ? (DNNModel._nWorkers  * DNNModel._nThreads[i] * sampleCount[i]) : (DNNModel._nThreads[i] * sampleCount[i]);
	}
	printf("%s\n", DNNPassName[dp]);
	for (int i = g_StartLayer; i < DNNModel._nLayers; i++)
	{		
		INT64 nTotalFlops = (DNNModel._Replicated[i] == true) ? ((INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP) :((INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP * DNNModel._nWorkers);
		//INT64 nTotalFlops = (INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP;
		double avgGFLOPs = (averageSampleTime[i] > 0.0f) ? nTotalFlops/(averageSampleTime[i] * 1e3) : 0.0f;
		printf("%10d %10d %10.2f %10.6f %I64d\n", i, DNNModel._nThreads[i], avgGFLOPs, averageSampleTime[i]/(1E3), nTotalFlops);
	}
	fflush(stdout);
	for (int i = 0; i < numThreads; i++)
	{
		tl[i].Fini();
	}
	delete [] sampleCount;
	delete [] averageSampleTime;
	delete [] helperThreads;
	delete [] tl;
}

void runDNNModel(void)
{
	printf("%-10s %-10s %-10s %-10s %-10s\n", "Layers", "Threads", "GFLOP/s", "MSec", "FLOPs");
	runDNNModelThreads(g_ThreadCount, DNN_FORWARD);
	if (g_Training)
	{
		runDNNModelThreads(g_ThreadCount, DNN_BACKWARD);
		runDNNModelThreads(g_ThreadCount, DNN_WEIGHTUPDATE);
	}
}

ModelType ProcessModelParam(const char *modelString)
{
	for (int i = 0; i < ModelType::NUM_MODEL_TYPE; i++)
	{
		if (_strcmpi(modelString, ModelName[i]) == 0) 
		{
			return (ModelType)i;
		}
	}
	return ModelType::NO_MODEL;
}
LayerConfig* ProcessParams(CParamParser& pparser)
{
	g_SampleCount = pparser.IsParamExist(TEXT("samples")) ? pparser.ParamInt64(TEXT("samples")) : DEFAULT_SAMPLE_COUNT;
	g_ThreadCount = pparser.IsParamExist(TEXT("threads")) ? pparser.ParamInt(TEXT("threads")) : DEFAULT_THREAD_COUNT;
	g_WorkerCount = pparser.IsParamExist(TEXT("workers")) ? pparser.ParamInt(TEXT("workers")) : DEFAULT_WORKER_COUNT;
	g_ModelType = pparser.IsParamExist(TEXT("model")) ? ProcessModelParam((char *)WtoA(pparser.ParamString(TEXT("model")))) : DEFAULT_MODEL_TYPE;
	g_ReplicateOutputLayer = pparser.IsParamExist(TEXT("partition")) ? false : true;
	g_DeltaWeightOpt = pparser.IsParamExist(TEXT("nodeltaweight")) ? false : true;
	int startLayer = pparser.IsParamExist(TEXT("startlayer")) ? pparser.ParamInt(TEXT("startlayer")) : 0;
	g_StartLayer = (startLayer < ModelLayerCount[(int)g_ModelType]) ? startLayer : 0;
	g_Training = pparser.IsParamExist(TEXT("classify")) ? false : true;
	g_trainingThreadAffinity._enabled = pparser.IsParamExist(TEXT("affinity")) ? true : false;
	g_Sparsity = pparser.IsParamExist(TEXT("sparsity")) ? pparser.ParamInt(TEXT("sparsity")) : 0;

	printf("WorkerCount: %d \n" 
		   "ThreadCount: %d \n"
		   "Model: %s \n"
		   "SampleCount: %I64d \n"
		   "OutputLayer: %s \n"
		   "DeltWeightOpt: %s\n"
		   "Momentum: %s \n"
		   "StartLayer: %d \n"
		   "Task: %s \n"
		   "Affinity: %s\n"
		   "Sparsity: %d\n", 
		g_WorkerCount, g_ThreadCount, ModelName[g_ModelType], g_SampleCount, 
		(g_ReplicateOutputLayer ? "Replicated" : "Partitioned"), 
		(g_DeltaWeightOpt ? "enabled" : "disabled"),
		(g_DeltaWeightOpt ? "enabled" : "disabled"),
		g_StartLayer,
		(g_Training ? "Training" : "Classify"),
		(g_trainingThreadAffinity._enabled ? "Enabled" : "Disabled"),
		g_Sparsity);

	return ModelConfig[g_ModelType][g_WorkerCount];
}

int _tmain(int argc, TCHAR *argv[])
{	
	CParamParser pparser(argc, argv);
	LayerConfig *lc = ProcessParams(pparser);

	if (lc == NULL) return 0;
	
	if (g_trainingThreadAffinity._enabled)
	{
		g_trainingThreadAffinity.Configure(g_ThreadCount);
		g_trainingThreadAffinity.Print();
	}
	lc->_FeedForwardSparsity = g_Sparsity;
	DNNModel.Init(ModelLayerCount[(int)g_ModelType], lc, g_WorkerCount, g_ReplicateOutputLayer);
	DNNModel.Print(ModelName[(int)g_ModelType]);

	runDNNModel();

	DNNModel.Fini();
	return 0;
}
