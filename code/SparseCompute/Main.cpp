#include "Utils.h"
#include "DataProvider.h"
#include <iostream>
#include <vector>

using namespace std;

LayerConfig W1_Model_KZ_1K[8] = {{96, 63*63, 11*11*3, 0, 0, 0, 0}, {256, 28*28, 25*96, 0, 0, 0, 0}, {384, 12*12, 9*256, 0, 0, 0, 0}, {384, 100, 9*384, 0, 0, 0, 0}, {256, 64, 9*384, 0, 0, 0, 0}, {4096, 1, 4096, 0, 0, 0, 0}, {4096, 1, 4096, 0, 0, 0, 0}, {4096, 1, 1000, 0, 0, 0, 0}};

LayerConfig W4_Model_1K[7] = {{120, 41*41, 243, 0, 0, 0, 0}, {250, 81, 3000, 0, 0, 0, 0}, {400, 9, 2250, 0, 0, 0, 0}, {600, 1, 3600, 0, 0, 0, 0}, {250, 1, 600, 0, 0, 0, 0}, {250, 1, 250, 0, 0, 0, 0}, {1000, 1, 1000, 0, 0, 0, 0}};
LayerConfig W2_Model_1K[7] = {{120, 85*41, 243, 0, 0, 0, 0}, {250, 20*9, 3000, 0, 0, 0, 0}, {400, 8*3, 2250, 0, 0, 0, 0}, {600, 6, 3600, 0, 0, 0, 0}, {500, 1, 1800, 0, 0, 0, 0}, {500, 1, 500, 0, 0, 0, 0}, {1000, 1, 1000, 0, 0, 0, 0}};
LayerConfig W1_Model_1K[7] = {{120, 85*85, 243, 0, 0, 0, 0}, {250, 20*20, 3000, 0, 0, 0, 0}, {400, 8*8, 2250, 0, 0, 0, 0}, {600, 6*6, 3600, 0, 0, 0, 0}, {1000, 1, 5400, 0, 0, 0, 0}, {1000, 1, 1000, 0, 0, 0, 0}, {1000, 1, 1000, 0, 0, 0, 0}};

LayerConfig W4_Model_22K[8] = {{120, 63*63, 49*3, 0, 0, 0, 0}, {250, 14*14, 25*120, 0, 0, 0, 0}, {400, 25, 9*250, 0, 0, 0, 0}, {400, 9, 9*400, 0, 0, 0, 0}, {600, 1, 9*400, 0, 0, 0, 0}, {750, 1, 600, 0, 0, 0, 0}, {750, 1, 750, 0, 0, 0, 0}, {3000, 1, 22000, 0, 0, 0, 0}};
LayerConfig W2_Model_22K[8] = {{120, 128*63, 49*3, 0, 0, 0, 0}, {250, 30*14, 25*120, 0, 0, 0, 0}, {400, 13*5, 9*250, 0, 0, 0, 0}, {400, 11*3, 9*400, 0, 0, 0, 0}, {600, 9, 9*400, 0, 0, 0, 0}, {1500, 1, 3000, 0, 0, 0, 0}, {1500, 1, 1500, 0, 0, 0, 0}, {3000, 1, 22000, 0, 0, 0, 0}};
LayerConfig W1_Model_22K[8] = {{120, 128*128, 49*3, 0, 0, 0, 0}, {250, 30*30, 25*120, 0, 0, 0, 0}, {400, 13*13, 9*250, 0, 0, 0, 0}, {400, 11*11, 9*400, 0, 0, 0, 0}, {600, 9*9, 9*400, 0, 0, 0, 0}, {3000, 1, 15000, 0, 0, 0, 0}, {3000, 1, 3000, 0, 0, 0, 0}, {3000, 1, 22000, 0, 0, 0, 0}};

LayerConfig W4_Model_MNIST[5] = {{10, 13*13, 25, 0, 0, 0, 0}, {20, 3*3, 25*10, 0, 0, 0, 0}, {80, 1, 100, 0, 0, 0, 0}, {100, 1, 100, 0, 0, 0, 0}, {400, 1, 10, 0, 0, 0, 0}};
LayerConfig W2_Model_MNIST[5] = {{10, 29*13, 25, 0, 0, 0, 0}, {20, 11*3, 25*10, 0, 0, 0, 0}, {240, 1, 200, 0, 0, 0, 0}, {200, 1, 200, 0, 0, 0, 0}, {400, 1, 10, 0, 0, 0, 0}};
LayerConfig W1_Model_MNIST[5] = {{10, 29*29, 25, 0, 0, 0, 0}, {20, 11*11, 25*10, 0, 0, 0, 0}, {720, 1, 400, 0, 0, 0, 0}, {400, 1, 400, 0, 0, 0, 0}, {400, 1, 10, 0, 0, 0, 0}};

LayerConfig W4_Model_CIFAR_10[5] = {{64, 14*14, 25*3, 0, 0, 0, 0}, {64, 3*3, 25*64, 0, 0, 0, 0}, {256, 1, 512, 0, 0, 0, 0}, {512, 1, 512, 0, 0, 0, 0}, {2048, 1, 10, 0, 0, 0, 0}};
LayerConfig W2_Model_CIFAR_10[5] = {{64, 32*14, 25*3, 0, 0, 0, 0}, {64, 12*3, 25*64, 0, 0, 0, 0}, {768, 1, 1024, 0, 0, 0, 0}, {1024, 1, 1024, 0, 0, 0, 0}, {2048, 1, 10, 0, 0, 0, 0}};
LayerConfig W1_Model_CIFAR_10[5] = {{64, 32*32, 25*3, 0, 0, 0, 0}, {64, 12*12, 25*64, 0, 0, 0, 0}, {2034, 1, 2048, 0, 0, 0, 0}, {2048, 1, 2048, 0, 0, 0, 0}, {2048, 1, 10, 0, 0, 0, 0}};

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

long long g_CurrentSamplePos;

CanonicalConfig g_CanonicalConfig;

DNN DNNModel;

DNNKernels g_DNNKernels;

std::vector<std::vector<float>>* g_Activations;
std::vector<std::vector<float>>* g_ErrorGradients;

#define NUM_DNN_UNIT_FLOP 2

const char *DNNPassName[NUM_DNN_PASS] = {"ForwardProp", "BackwardProp", "WeightUpdate"};
DWORD DNNModelThreadForward(ThreadLayerState *tl)
{
    SetTrainingThreadAffinity(tl->_threadNum);
    float **inputActivation = new float *[tl->_numLayers];
    float **outputActivation = new float *[tl->_numLayers];
    for (int i = tl->_startLayer; i < tl->_numLayers; i++)
    {
        inputActivation[i] = new float[tl->_LayerState[i]._InputSize];
        outputActivation[i] = new float[tl->_LayerState[i]._OutputSize];
    }

    while (true)
    {
        INT64 sampleId = ATOMIC_INCREMENT64(g_CurrentSamplePos);
        if (sampleId >= G_SAMPLE_COUNT) break;
        int numLayers = ((sampleId % G_WORKER_COUNT) == 0) ? tl->_numLayers : tl->_numLayers-1;
        for (int l = tl->_startLayer; l < numLayers; l++) {
            
#ifdef PREPARE_COMPUTE_DATA
            std::vector<std::vector<float>>& layerActivations = g_Activations[l];
            int activationId = sampleId % layerActivations.size();
            std::vector<float>& activationVector = layerActivations[activationId];
            Sparsify(layerActivations[activationId], G_FORWARD_SPARSITY);
            const float* inpACT = &activationVector[0];
#else
            float *inpACT = inputActivation[l];
            Sparsify(inpACT, tl->_LayerState[l]._InputSize, G_FORWARD_SPARSITY);
#endif
            float *outACT = outputActivation[l];
            Layer *layer = (tl->_LayerState + l);

            DECLARE_TIMER(timer);
            START_TIMER(timer);
            g_DNNKernels._feedForward(layer, inpACT, outACT);           
            STOP_TIMER(timer);
            tl->_FLOPTime[l] += ELAPSED_USEC_TIME(timer);

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


DWORD DNNModelThreadBackward(ThreadLayerState *tl)
{
    SetTrainingThreadAffinity(tl->_threadNum);

    float **outputError = new float *[tl->_numLayers];
    float **inputError = new float *[tl->_numLayers];
    for (int i = tl->_startLayer; i < tl->_numLayers; i++)
    {
        outputError[i] = new float[tl->_LayerState[i]._InputSize];
        inputError[i] = new float[tl->_LayerState[i]._OutputSize];
    }
    while (true)
    {
        INT64 sampleId = ATOMIC_INCREMENT64(g_CurrentSamplePos);
        if (sampleId >= G_SAMPLE_COUNT) break;
        int numLayers = ((sampleId % G_WORKER_COUNT) == 0) ? tl->_numLayers : tl->_numLayers-1;
        for (int l = tl->_startLayer; l < numLayers; l++)
        {
            Sparsify(inputError[l], tl->_LayerState[l]._OutputSize, G_BACKPROP_SPARSITY);
            Layer *layer = (tl->_LayerState + l);
 
            DECLARE_TIMER(timer);
            START_TIMER(timer);
            g_DNNKernels._backPropagate(layer, inputError[l], outputError[l]);
            STOP_TIMER(timer);
            tl->_FLOPTime[l] += ELAPSED_USEC_TIME(timer);

            tl->_SampleCount[l]++;
        }
    }
    for (int i = tl->_startLayer; i < tl->_numLayers; i++)
    {
        delete [] outputError[i];
        delete [] inputError[i];
    }
    delete []outputError;
    delete []inputError;
    return 0;
}

DWORD DNNModelThreadDeltaWeightUpdate(ThreadLayerState *tl)
{
    SetTrainingThreadAffinity(tl->_threadNum);

    float **inputActivation = new float *[tl->_numLayers];
    float **inputError = new float *[tl->_numLayers];
    float **weightDeltas = new  float *[tl->_numLayers];
    //float **weightMomentum = new  float *[tl->_numLayers];
    
    for (int i = tl->_startLayer; i < tl->_numLayers; i++)
    {
            inputActivation[i] = new float[tl->_LayerState[i]._InputSize];
            inputError[i] = new float[tl->_LayerState[i]._OutputSize];
            weightDeltas[i] = new float[tl->_LayerState[i]._WeightSize];
            //weightMomentum[i] = new float[tl->_LayerState[i]._WeightSize];
    }
    while (true)
    {
            INT64 sampleId = ATOMIC_INCREMENT64(g_CurrentSamplePos);
            if (sampleId >= G_SAMPLE_COUNT) break;
            int numLayers = ((sampleId % G_WORKER_COUNT) == 0) ? tl->_numLayers : tl->_numLayers-1;
            for (int l = tl->_startLayer; l < numLayers; l++) 
            {
                Sparsify(inputActivation[l], tl->_LayerState[l]._InputSize, G_FORWARD_SPARSITY);
                Sparsify(inputError[l], tl->_LayerState[l]._OutputSize, G_BACKPROP_SPARSITY);
                Layer *layer = (tl->_LayerState + l);
                
                // Compute weight deltas
                DECLARE_TIMER(deltaTimer);
                START_TIMER(deltaTimer);
                if (layer->_Input2Height == 1) 
                {
                    g_DNNKernels._computeWeightDelta_2D(layer, weightDeltas[l], inputActivation[l], inputError[l]);
                }
                else 
                {
                    g_DNNKernels._computeWeightDelta_3D(layer, weightDeltas[l], inputActivation[l], inputError[l]);
                }
                STOP_TIMER(deltaTimer);
                tl->_FLOPTime[l] += ELAPSED_USEC_TIME(deltaTimer);

                // Update weights
                Sparsify(weightDeltas[l], tl->_LayerState[l]._WeightSize, G_WEIGHTUPDATE_SPARSITY);
                DECLARE_TIMER(weightTimer);
                START_TIMER(weightTimer);
                g_DNNKernels._weightUpdate(layer, weightDeltas[l]);
                STOP_TIMER(weightTimer);
                tl->_FLOPTime[l] += ELAPSED_USEC_TIME(weightTimer);

                tl->_SampleCount[l]++;
        }
    }

    for (int i = tl->_startLayer; i < tl->_numLayers; i++)
    {
            delete [] inputActivation[i];
            delete [] inputError[i];
            delete [] weightDeltas[i];
            //delete [] weightMomentum[i];
    }
    delete []inputActivation;
    delete []inputError;
    delete [] weightDeltas;
    //delete [] weightMomentum;
    return 0;
}
                    
DWORD DNNModelThreadWeightUpdate(ThreadLayerState *tl)
{
    SetTrainingThreadAffinity(tl->_threadNum);

    float **inputActivation = new float *[tl->_numLayers];
    float **outputActivation = new float *[tl->_numLayers];
    for (int i = tl->_startLayer; i < tl->_numLayers; i++)
    {
        inputActivation[i] = new float[tl->_LayerState[i]._InputSize];
        outputActivation[i] = new float[tl->_LayerState[i]._OutputSize];
    }
    while (true)
    {
        INT64 sampleId = ATOMIC_INCREMENT64(g_CurrentSamplePos);
        if (sampleId >= G_SAMPLE_COUNT) break;
        int numLayers = ((sampleId % G_WORKER_COUNT) == 0) ? tl->_numLayers : tl->_numLayers-1;
        for (int l = tl->_startLayer; l < numLayers; l++)
        {
            float *inpACT = inputActivation[l];
            float *outACT = outputActivation[l];
            Layer *layer = (tl->_LayerState + l);
            DECLARE_TIMER(timer);
            START_TIMER(timer);
            if (layer->_Input2Height == 1)
            {
                for (int i = 0; i < layer->_OutputFeature; i++)
                {
                }
            }
            else 
            {
                for (int i = 0; i < layer->_OutputFeature; i++)
                {
                    for (int j = 0; j < layer->_Input2Height; j++)
                    {
                    }
                }
            }
            STOP_TIMER(timer);
            tl->_FLOPTime[l] += ELAPSED_USEC_TIME(timer);
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

void PrintComputeStats(int numThreads, ThreadLayerState *tl, DNNPass dp)
{
    int *sampleCount = new int[DNNModel._nLayers];
    double *averageSampleTime = new double[DNNModel._nLayers];
    double totalPassTime = 0.0f;
    for (int i = G_START_LAYER; i < DNNModel._nLayers; i++)
    {
        averageSampleTime[i] = 0;
        sampleCount[i] = 0;
        for (int j = 0; j < numThreads; j++)
        {
            averageSampleTime[i] += (INT64)(tl[j]._FLOPTime[i]);
            sampleCount[i] += tl[j]._SampleCount[i];
        }
        averageSampleTime[i] /=  (DNNModel._Replicated[i] == true) ? (DNNModel._nWorkers  * DNNModel._nThreads[i] * sampleCount[i]) : (DNNModel._nThreads[i] * sampleCount[i]);
        totalPassTime += averageSampleTime[i];
    }

    printf("%s: %5.2f msecs\n", DNNPassName[dp], totalPassTime/(1E3));
    for (int i = G_START_LAYER; i < DNNModel._nLayers; i++)
    {		
        INT64 nTotalFlops = (DNNModel._Replicated[i] == true) ? ((INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP) :((INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP * DNNModel._nWorkers);
        double avgGFLOPs = (averageSampleTime[i] > 0.0f) ? nTotalFlops/(averageSampleTime[i] * 1e3) : 0.0f;
        printf("%10d %10d %10.2f %10.6f %lld\n", i, DNNModel._nThreads[i], avgGFLOPs, averageSampleTime[i]/(1E3), nTotalFlops);
    }

    fflush(stdout);
    delete [] sampleCount;
    delete [] averageSampleTime;
}

double runDNNModelThreads (int numThreads, DNNPass dp)
{
    ThreadLayerState *tl = new ThreadLayerState[numThreads];
    g_CurrentSamplePos = -1;
    for (int i = 0; i < numThreads; i++)
    {

        tl[i].Init(i, DNNModel);
    }

    DECLARE_TIMER(computeTimer);
    START_TIMER(computeTimer);
    DoModelCompute(numThreads, tl, dp);
    STOP_TIMER(computeTimer);

    PrintComputeStats(numThreads, tl, dp);

    for (int i = 0; i < numThreads; i++)
    {
        tl[i].Fini();
    }
    delete [] tl;

    double elapsedTime = ELAPSED_USEC_TIME(computeTimer);
    std::cout<<"Pass_run_time: " << elapsedTime/(1E06) << "secs" << endl;
    return elapsedTime;
}

double runDNNModel(void)
{
    printf("%-10s %-10s %-10s %-10s %-10s\n", "Layers", "Threads", "GFLOP/s", "MSec", "FLOPs");
    double elapsedTime = 0;
   
    if (G_DNN_PASS_ENABLED(DNN_FORWARD) && g_DNNKernels._feedForward != nullptr)
    {
        elapsedTime = runDNNModelThreads(G_THREAD_COUNT, DNN_FORWARD);
    }

    if (G_DNN_PASS_ENABLED(DNN_BACKWARD) && g_DNNKernels._backPropagate != nullptr)
    {
        elapsedTime += runDNNModelThreads(G_THREAD_COUNT, DNN_BACKWARD);
    }
        
    if (G_DNN_PASS_ENABLED(DNN_WEIGHTUPDATE) && g_DNNKernels._computeWeightDelta_2D != nullptr && g_DNNKernels._computeWeightDelta_3D != nullptr)
    {
        elapsedTime += runDNNModelThreads(G_THREAD_COUNT, DNN_WEIGHTUPDATE);
    }
 
    return elapsedTime;
}

ModelType ProcessModelParam(const char *modelString)
{
    for (int i = 0; i < NUM_MODEL_TYPE; i++)
    {
        if (CASE_INSENSITIVE_STRCMP(modelString, ModelName[i]) == 0) 
        {
            return (ModelType)i;
        }
    }
    return NO_MODEL;
}

void ProcessPassParam(const char* passString)
{
    for (int i = 0; i < NUM_DNN_PASS; i++)
    {
        if (CASE_INSENSITIVE_STRCMP(passString, DNNPassName[i]) == 0)
        {
  	    g_CanonicalConfig.EnablePass(static_cast<DNNPass>(i), DNNPassName[i]);
            break;
        }
    }

}

void PrepareComputeData(DNN& model)
{
    InitDataProvider();
    
    g_Activations = new std::vector<std::vector<float>> [model._nLayers];
    g_ErrorGradients = new std::vector<std::vector<float>>[model._nLayers];
    const int numElems = 100;
    INT64 memorySize = 0;
    for (size_t i = 0; i < model._nLayers; i++)
    {
        memorySize += model._Layers[i]._InputSize + model._Layers[i]._OutputSize;
    }

    memorySize *= numElems * sizeof(float);
    printf("Total Memory size = %5.2f GB\n", memorySize * 1.0f / (1 << 30)); fflush(stdout);

    for (size_t i = 0; i < model._nLayers; i++)
    {
        GetSparseData(g_Activations[i], model._Layers[i]._InputSize, numElems, G_FORWARD_SPARSITY);
        GetSparseData(g_ErrorGradients[i], model._Layers[i]._OutputSize, numElems, G_BACKPROP_SPARSITY);
    }

    //for (size_t i = 0; i < model._nLayers; i++)
    //{
    //    PrintSparseStatistics("Activations", g_Activations[i]);
    //    PrintSparseStatistics("Error Gradients", g_ErrorGradients[i]);
    //}
}

void ReleaseComputeData()
{
    delete[] g_Activations;
    delete[] g_ErrorGradients;
}

int main(int argc, char *argv[])
{
    g_CanonicalConfig.Init();

    SetCanonicalConfig(argc, argv, g_CanonicalConfig);
    g_CanonicalConfig.Print();
    LayerConfig* lc = ModelConfig[g_CanonicalConfig._modelType][g_CanonicalConfig._workerCount];
    
    if (lc == NULL) return 0;

    for (int i = 0; i < ModelLayerCount[(int)G_MODEL_TYPE]; i++)
    {
        lc[i].InitSparsity(G_FORWARD_SPARSITY, G_BACKPROP_SPARSITY, G_DELTACOMPUTE_SPARSITY, G_WEIGHTUPDATE_SPARSITY, G_SIGNAL_CACHELINE_SPARSITY, G_DELTA_CACHELINE_SPARSITY);
    }


    DNNModel.Init(ModelLayerCount[(int)G_MODEL_TYPE], lc, G_WORKER_COUNT, G_REPLICATED_OUTPUT_LAYER);
//    DNNModel.Print( ModelName[g_CanonicalConfig._modelType]);

#ifdef PREPARE_COMPUTE_DATA
    PrepareComputeData(DNNModel);
#endif

    InitDNNKernels(g_DNNKernels, G_DNN_KERNEL_VERSION);

    double runTime = runDNNModel();
    std::cout<<"Total_run_time: " << runTime/(1E06) << "secs" << endl;

    DNNModel.Fini();

#ifdef PREPARE_COMPUTE_DATA
    ReleaseComputeData();
#endif
    return 0;
}
