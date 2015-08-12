#include "Utils.h"
#include <iostream>
#include <vector>

using namespace std;

struct ThreadAffinityConfig {
	bool _enabled;
	bool _hyperThreadingEnabled;
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
		_enabled = true;

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

void SetTrainingThreadAffinity(int threadNum)
{
	if (g_trainingThreadAffinity._enabled) {
	  const DWORD logicalProcessor = g_trainingThreadAffinity.LogicalProcessor(threadNum);
	  const DWORD affinityMask = g_trainingThreadAffinity.AffinityMask(threadNum);
	  SetThreadAffinityMask(GetCurrentThread(), affinityMask);
	}
}

void ProcessParams(CParamParser& pparser, CanonicalConfig& config)
{
	config._sampleCount = pparser.IsParamExist(TEXT("samples")) ? pparser.ParamInt(TEXT("samples")) : DEFAULT_SAMPLE_COUNT;
	config._threadCount = pparser.IsParamExist(TEXT("threads")) ? pparser.ParamInt(TEXT("threads")) : DEFAULT_THREAD_COUNT;
	config._workerCount = pparser.IsParamExist(TEXT("workers")) ? pparser.ParamInt(TEXT("workers")) : DEFAULT_WORKER_COUNT;
	config._modelType = pparser.IsParamExist(TEXT("model")) ? ProcessModelParam((char *)WtoA(pparser.ParamString(TEXT("model")))) : DEFAULT_MODEL_TYPE;
	config._replicatedOutputLayer = pparser.IsParamExist(TEXT("partition")) ? false : true;
	config._deltaWeightOpt = pparser.IsParamExist(TEXT("nodeltaweight")) ? false : true;
	int startLayer = pparser.IsParamExist(TEXT("startlayer")) ? pparser.ParamInt(TEXT("startlayer")) : DEFAULT_START_LAYER;
	config._startLayer = (startLayer < ModelLayerCount[(int)config._modelType]) ? startLayer : DEFAULT_START_LAYER;
	config._training = pparser.IsParamExist(TEXT("classify")) ? false : true;
	config._affinity = pparser.IsParamExist(TEXT("affinity")) ? true : false;
	config._forwardSparsity = pparser.IsParamExist(TEXT("forwardsparsity")) ? pparser.ParamInt(TEXT("forwardsparsity")) : DEFAULT_SPARSITY;
	config._backwardSparsity = pparser.IsParamExist(TEXT("backpropsparsity")) ? pparser.ParamInt(TEXT("backpropsparsity")) : DEFAULT_SPARSITY;
	config._deltaComputeSparsity = pparser.IsParamExist(TEXT("deltacomputesparsity")) ? pparser.ParamInt(TEXT("deltacomputesparsity")) : DEFAULT_SPARSITY;
	config._weightUpdateSparsity = pparser.IsParamExist(TEXT("weightupdatesparsity")) ? pparser.ParamInt(TEXT("weightupdatesparsity")) : DEFAULT_SPARSITY;
	config._sparseKernelVersion = pparser.IsParamExist(TEXT("sparsekernels")) ? pparser.ParamInt(TEXT("sparsekernels")) : 0;
}

void SetCanonicalConfig(int argc, char* argv[], CanonicalConfig& config)
{
	int tArgLen = argc;
	TCHAR** tArgv = new TCHAR*[tArgLen];
	for (int i = 0; i < tArgLen; i++)
	{
		size_t size = strlen(argv[i]) + 1;
		tArgv[i] = new TCHAR[size];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, tArgv[i], size, argv[i], _TRUNCATE);
	}

	CParamParser pparser (tArgLen, tArgv);
	ProcessParams(pparser, config);
	if (config._affinity)
	{
		g_trainingThreadAffinity.Configure(config._threadCount);
		g_trainingThreadAffinity.Print();
	}
	for (int i = 0; i < tArgLen; i++)
	{
		delete tArgv[i];
	}
	delete [] tArgv;
}

DWORD WINAPI s_DNNModelThreadForward(LPVOID lp)
{
	return DNNModelThreadForward((ThreadLayerState *)lp);
}

DWORD WINAPI s_DNNModelThreadBackward(LPVOID lp)
{
	return DNNModelThreadBackward((ThreadLayerState *)lp);
}

DWORD WINAPI s_DNNModelThreadWeightUpdate(LPVOID lp)
{
	if (G_DELTA_WEIGHT_OPT)
	{
		return  DNNModelThreadDeltaWeightUpdate((ThreadLayerState *)lp);
	}
	return DNNModelThreadWeightUpdate((ThreadLayerState *)lp);
}

void DoModelCompute(int numThreads, ThreadLayerState *tl, DNNPass dp)
{
  HANDLE *helperThreads = new HANDLE[numThreads];

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
	delete [] helperThreads;
}
