#include "Utils.h"
#include "ThreadAffinity.h"
#include <iostream>
#include <vector>

using namespace std;

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
    config._affinity = pparser.IsParamExist(TEXT("affinity")) ? true : DEFAULT_AFFINITY;
    config._forwardSparsity = pparser.IsParamExist(TEXT("forwardsparsity")) ? pparser.ParamInt(TEXT("forwardsparsity")) : DEFAULT_SPARSITY;
    config._backwardSparsity = pparser.IsParamExist(TEXT("backwardsparsity")) ? pparser.ParamInt(TEXT("backwardsparsity")) : DEFAULT_SPARSITY;
    config._deltaComputeSparsity = pparser.IsParamExist(TEXT("deltacomputesparsity")) ? pparser.ParamInt(TEXT("deltacomputesparsity")) : DEFAULT_SPARSITY;
    config._weightUpdateSparsity = pparser.IsParamExist(TEXT("weightupdatesparsity")) ? pparser.ParamInt(TEXT("weightupdatesparsity")) : DEFAULT_SPARSITY;
    config._sparseKernelVersion = pparser.IsParamExist(TEXT("sparsekernels")) ? pparser.ParamInt(TEXT("sparsekernels")) : 0;
    config._zeroSignalOpt = pparser.IsParamExist(TEXT("zerosignalopt")) ? true : false;
    config._signalCacheLineSparsity = pparser.IsParamExist(TEXT("signalcachelinesparsity")) ? pparser.ParamInt(TEXT("signalcachelinesparsity")) : DEFAULT_SPARSITY;
    config._deltaCacheLineSparsity = pparser.IsParamExist(TEXT("deltacachelinesparsity")) ? pparser.ParamInt(TEXT("deltacachelinesparsity")) : DEFAULT_SPARSITY;
    config._activationCacheLineSparsity = pparser.IsParamExist(TEXT("activationcachelinesparsity")) ? pparser.ParamInt(TEXT("activationcachelinesparsity")) : DEFAULT_SPARSITY;
    config._threadModel = pparser.IsParamExist(TEXT("threadmodel")) ? pparser.ParamInt(TEXT("threadmodel")) : DEFAULT_THREAD_MODEL;
    config._outputScale = pparser.IsParamExist(TEXT("outputscale")) ? pparser.ParamInt(TEXT("outputscale")) : DEFAULT_OUTPUT_SCALE;
    int kernelVersion = pparser.IsParamExist(TEXT("kernel")) ? pparser.ParamInt(TEXT("kernel")) : DEFAULT_KERNEL_VERSION;
    assert(kernelVersion < (int)KernelVersion::KERNEL_VERSION_COUNT);
    config._kernelVersion = static_cast<KernelVersion>(kernelVersion);
    
    if (pparser.IsParamExist(TEXT("pass")))
    {
        ProcessPassParam((char *)WtoA(pparser.ParamString(TEXT("pass"))));
    }
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
