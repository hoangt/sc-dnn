#include "Utils.h"

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

	extern float mulsum3_base(const float *pf0, const float *pf1, float f2, INT64 count);
	extern float mulsum3_opt1_75_25(const float *pf0, const float *pf1, float f2, INT64 count);
	extern float mulsum3_opt1_50_50(const float *pf0, const float *pf1, float f2, INT64 count);
	extern float mulsum3_opt1_25_75(const float *pf0, const float *pf1, float f2, INT64 count);
	extern float mulsum3_opt1_0_100(const float *pf0, const float *pf1, float f2, INT64 count);

	extern float mulsum2_opt2_75_25(const float *pf0, const float *pf1, INT64 count);
	extern float mulsum2_opt2_50_50(const float *pf0, const float *pf1, INT64 count);
	extern float mulsum2_opt2_25_75(const float *pf0, const float *pf1, INT64 count);
	extern float mulsum2_opt2_0_100(const float *pf0, const float *pf1, INT64 count);

	extern float mulsum3_opt2_75_25(const float *pf0, const float *pf1, float f2, INT64 count);
	extern float mulsum3_opt2_50_50(const float *pf0, const float *pf1, float f2, INT64 count);
	extern float mulsum3_opt2_25_75(const float *pf0, const float *pf1, float f2, INT64 count);
	extern float mulsum3_opt2_0_100(const float *pf0, const float *pf1, float f2, INT64 count);

}

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


#define FEED_FORWARD_CALL(mulsum_func) \
    START_TIMER(timer);                                                      \
    for (int i = 0; i < layer->_OutputFeature; i++)                     \
        for (int j = 0; j < layer->_Input2Height; j++)                  \
            outACT[i*layer->_Input2Height + j] = mulsum_func(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width); \
    STOP_TIMER(timer);

#define FEED_FORWARD_CALL_2(m2func1, m2func2) \
    INT64 second = (INT64)((layer->_Input2Width * sparsity) / 25.0);    \
    INT64 first = layer->_Input2Width - second;                         \
    START_TIMER(timer);                                                      \
    for (int i = 0; i < layer->_OutputFeature; i++)                     \
        for (int j = 0; j < layer->_Input2Height; j++) {                \
            outACT[i*layer->_Input2Height + j] = m2func1(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), first); \
            outACT[i*layer->_Input2Height + j] = m2func2(inpACT+(j*layer->_Input2Width) + first, layer->_Weights+(i*layer->_Input2Width) + first, second); \
        }                                                               \
    STOP_TIMER(timer);

double mulsum2_wrapper(Layer *layer, float *inpACT, float* outACT) {
   DECLARE_TIMER(timer);
    int sparsity = layer->_FeedForwardSparsity;
    if (sparsity == 0) {
        FEED_FORWARD_CALL(mulsum2_base);
    }
    else if (sparsity < 25) {
        FEED_FORWARD_CALL_2(mulsum2_base, mulsum2_opt1_75_25);
    }
    else if (sparsity == 25) {
        FEED_FORWARD_CALL(mulsum2_opt1_75_25);
    }
    else if (sparsity < 50) {
        sparsity -= 25;
        FEED_FORWARD_CALL_2(mulsum2_opt1_75_25, mulsum2_opt1_50_50);
    }
    else if (sparsity == 50) {
        FEED_FORWARD_CALL(mulsum2_opt1_50_50);
    }
    else if (sparsity < 75) {
        sparsity -= 50;
        FEED_FORWARD_CALL_2(mulsum2_opt1_50_50, mulsum2_opt1_25_75);
    }
    else if (sparsity == 75) {
        FEED_FORWARD_CALL(mulsum2_opt1_25_75);
    }
    else if (sparsity < 100) {
        sparsity -= 75;
        FEED_FORWARD_CALL_2(mulsum2_opt1_25_75, mulsum2_opt1_0_100);
    }
    else if (sparsity == 100) {
        FEED_FORWARD_CALL(mulsum2_opt1_0_100);
    }
	return ELAPSED_USEC_TIME(timer);
}

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
		for (int l = tl->_startLayer; l < numLayers; l++)
		{
			float *inpACT = inputActivation[l];
			float *outACT = outputActivation[l];
			Layer *layer = (tl->_LayerState + l);
            double elapsedTime;
#ifdef USE_SPARSE_KERNELS 
            elapsedTime = mulsum2_wrapper(layer, inpACT, outACT);  
#else
	                DECLARE_TIMER(timer);
			START_TIMER(timer);
			for (int i = 0; i < layer->_OutputFeature; i++)
			{
				for (int j = 0; j < layer->_Input2Height; j++)
				{
					outACT[i*layer->_Input2Height + j] = avx2_mulsum_2_mem(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), layer->_Input2Width);
				}
			}
			STOP_TIMER(timer);
			elapsedTime = ELAPSED_USEC_TIME(timer);
#endif
			tl->_FLOPTime[l] += elapsedTime;  
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

#define MULSUM3_GEN_WRAPPER(wrapper1,wrapper2)                  \
    if (sparsity == 0) {                                        \
        wrapper1(mulsum3_base);                                 \
    }                                                           \
    else if (sparsity < 25) {                                   \
        sparsity -= 0;                                          \
        wrapper2(mulsum3_base, mulsum3_opt1_75_25);             \
    }                                                           \
    else if (sparsity == 25) {                                  \
        wrapper1(mulsum3_opt1_75_25);                           \
    }                                                           \
    else if (sparsity < 50) {                                   \
        sparsity -= 25;                                         \
        wrapper2(mulsum3_opt1_75_25, mulsum3_opt1_50_50);       \
    }                                                           \
    else if (sparsity == 50) {                                  \
        wrapper1(mulsum3_opt1_50_50);                           \
    }                                                           \
    else if (sparsity < 75) {                                   \
        sparsity -= 50;                                         \
        wrapper2(mulsum3_opt1_50_50, mulsum3_opt1_25_75);       \
    }                                                           \
    else if (sparsity == 75) {                                  \
        wrapper1(mulsum3_opt1_25_75);                           \
    }                                                           \
    else if (sparsity < 100) {                                  \
        sparsity -= 75;                                         \
        wrapper2(mulsum3_opt1_25_75, mulsum3_opt1_0_100);       \
    }                                                           \
    else if (sparsity == 100) {                                 \
        wrapper1(mulsum3_opt1_0_100);                           \
    }


#define BACK_PROP_WRAPPER(mulsum3_func)                                 \
    START_TIMER(timer);                                                      \
    for (int i = 0; i < layer->_OutputFeature; i++)                     \
        for (int j = 0; j < layer->_Input2Height; j++)                  \
            mulsum3_func(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), outACT[i*layer->_Input2Height + j], layer->_Input2Width); \
    STOP_TIMER(timer);

#define BACK_PROP_WRAPPER_2(m3func1, m3func2)                           \
    INT64 second = (INT64)((layer->_Input2Width * sparsity) / 25.0);    \
    INT64 first = layer->_Input2Width - second;                         \
    START_TIMER(timer);                                                      \
    for (int i = 0; i < layer->_OutputFeature; i++)                     \
        for (int j = 0; j < layer->_Input2Height; j++) {                \
            m3func1(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), outACT[i*layer->_Input2Height + j], first); \
            m3func2(inpACT+(j*layer->_Input2Width) + first, layer->_Weights+(i*layer->_Input2Width) + first, outACT[i*layer->_Input2Height + j], second); \
        }                                                               \
    STOP_TIMER(timer);                                                       
    

double BackPropWrapper(Layer *layer, float *inpACT, float *outACT) {
    int sparsity = layer->_BackPropSparsity;
   DECLARE_TIMER(timer);

    MULSUM3_GEN_WRAPPER(BACK_PROP_WRAPPER, BACK_PROP_WRAPPER_2)

    return ELAPSED_USEC_TIME(timer);
}

DWORD DNNModelThreadBackward(ThreadLayerState *tl)
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
            double elapsedTime;
#ifdef USE_SPARSE_KERNELS
            elapsedTime = BackPropWrapper(layer, inpACT, outACT);
#else
	                DECLARE_TIMER(timer);
			START_TIMER(timer);
			for (int i = 0; i < layer->_OutputFeature; i++)
			{
				for (int j = 0; j < layer->_Input2Height; j++)
				{
					avx2_mulsum_3_mem(inpACT+(j*layer->_Input2Width), layer->_Weights+(i*layer->_Input2Width), outACT[i*layer->_Input2Height + j], layer->_Input2Width); 
				}
			}
			STOP_TIMER(timer);
			elapsedTime = ELAPSED_USEC_TIME(timer);
#endif
                        
			tl->_FLOPTime[l] += elapsedTime; 
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

#define DELTA_COMPUTE_WRAPPER(mulsum3_func)                                 \
    START_TIMER(timer);                                                      \
    for (int i = 0; i < layer->_OutputFeature; i++)                     \
        for (int j = 0; j < layer->_Input2Height; j++)                  \
            mulsum3_func(deltaWeights+(i*layer->_Input2Width), inpACT+(j*layer->_Input2Width), outACT[i*layer->_Input2Height + j], layer->_Input2Width); \
    STOP_TIMER(timer);                                                       

#define DELTA_COMPUTE_WRAPPER_2(m3func1,m3func2)                        \
    INT64 second = (INT64)((layer->_Input2Width * sparsity) / 25.0);    \
    INT64 first = layer->_Input2Width - second;                         \
    START_TIMER(timer);                                                      \
    for (int i = 0; i < layer->_OutputFeature; i++)                     \
        for (int j = 0; j < layer->_Input2Height; j++) {                \
            m3func1(deltaWeights+(i*layer->_Input2Width), inpACT+(j*layer->_Input2Width), outACT[i*layer->_Input2Height + j], first); \
            m3func2(deltaWeights+(i*layer->_Input2Width) + first, inpACT+(j*layer->_Input2Width) + first, outACT[i*layer->_Input2Height + j], second); \
        }                                                               \
    STOP_TIMER(timer);                                                       
    

double DeltaComputeWrapper(Layer *layer, float *deltaWeights, float *inpACT, float *outACT) {
   DECLARE_TIMER(timer);
    int sparsity = layer->_DeltaComputeSparsity;

    MULSUM3_GEN_WRAPPER(DELTA_COMPUTE_WRAPPER, DELTA_COMPUTE_WRAPPER_2)
    
    return ELAPSED_USEC_TIME(timer);
}


#define WEIGHT_UPDATE_WRAPPER(mulsum3_func)                             \
    START_TIMER(timer);                                                      \
    mulsum3_func(layer->_Weights, deltaWeights, 1.0f, layer->_WeightSize); \
    STOP_TIMER(timer);

#define WEIGHT_UPDATE_WRAPPER_2(m3func1,m3func2)                        \
    INT64 second = (INT64)((layer->_WeightSize * sparsity) / 25.0);     \
    INT64 first = layer->_WeightSize - second;                          \
    START_TIMER(timer);                                                      \
    m3func1(layer->_Weights, deltaWeights, 1.0f, first);                \
    m3func2(layer->_Weights + first, deltaWeights + first, 1.0f, second); \
    STOP_TIMER(timer);

double WeightUpdateWrapper(Layer *layer, float *deltaWeights) {
   DECLARE_TIMER(timer);
    int sparsity = layer->_WeightUpdateSparsity;

    MULSUM3_GEN_WRAPPER(WEIGHT_UPDATE_WRAPPER,WEIGHT_UPDATE_WRAPPER_2)

    return ELAPSED_USEC_TIME(timer);
}
        


DWORD DNNModelThreadDeltaWeightUpdate(ThreadLayerState *tl)
{
	SetTrainingThreadAffinity(tl->_threadNum);

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
		INT64 sampleId = ATOMIC_INCREMENT64(g_CurrentSamplePos);
		if (sampleId >= G_SAMPLE_COUNT) break;
		int numLayers = ((sampleId % G_WORKER_COUNT) == 0) ? tl->_numLayers : tl->_numLayers-1;
		for (int l = tl->_startLayer; l < numLayers; l++)
		{
			float *inpACT = inputActivation[l];
			float *outACT = outputActivation[l];
			Layer *layer = (tl->_LayerState + l);
			DECLARE_TIMER(timer);
            double elapsedTime;

            // momemtum computation
			START_TIMER(timer);
			avx2_mulsum_3_mem(deltaWeights[l], weightMomentum[l], 1.0f, tl->_LayerState[l]._WeightSize);
            STOP_TIMER(timer);
			tl->_FLOPTime[l] += ELAPSED_USEC_TIME(timer);

            // deltaweight computation
			if (layer->_Input2Height == 1)
			{
                START_TIMER(timer);
                for (int i = 0; i < layer->_OutputFeature; i++)
                    avx2_mulsum_3_mem(deltaWeights[l]+(i*layer->_Input2Width), inpACT, outACT[i], layer->_Input2Width); 
                STOP_TIMER(timer);
                tl->_FLOPTime[l] += ELAPSED_USEC_TIME(timer);
			}
			else 
			{
#ifdef USE_SPARSE_KERNELS
				elapsedTime = DeltaComputeWrapper(layer, deltaWeights[l], inpACT, outACT);
#else
				START_TIMER(timer);
				for (int i = 0; i < layer->_OutputFeature; i++)
					for (int j = 0; j < layer->_Input2Height; j++)
						avx2_mulsum_3_mem(deltaWeights[l]+(i*layer->_Input2Width), inpACT+(j*layer->_Input2Width), outACT[i*layer->_Input2Height + j], layer->_Input2Width); 
				STOP_TIMER(timer);
				elapsedTime = ELAPSED_USEC_TIME(timer);
#endif 
				tl->_FLOPTime[l] += elapsedTime;
                            
				// weight update
#ifdef USE_SPARSE_KERNELS
				elapsedTime = WeightUpdateWrapper(layer, deltaWeights[l]);
#else				
				START_TIMER(timer);
				avx2_mulsum_3_mem(layer->_Weights, deltaWeights[l], 1.0f, layer->_WeightSize);
				STOP_TIMER(timer);
				elapsedTime = ELAPSED_USEC_TIME(timer);
#endif
				tl->_FLOPTime[l] += elapsedTime;
			}

            // weight copy
            START_TIMER(timer);
			avx2_fmemcpy(weightMomentum[l], deltaWeights[l], tl->_LayerState[l]._WeightSize);
			STOP_TIMER(timer);

			tl->_FLOPTime[l] += ELAPSED_USEC_TIME(timer);
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

void runDNNModelThreads (int numThreads, DNNPass dp)
{
	ThreadLayerState *tl = new ThreadLayerState[numThreads];
	g_CurrentSamplePos = -1;
	for (int i = 0; i < numThreads; i++)
	{

		tl[i].Init(i, DNNModel);
	}

	DoModelCompute(numThreads, tl, dp);

	int *sampleCount = new int[DNNModel._nLayers];
	double *averageSampleTime = new double[DNNModel._nLayers];
	for (int i = G_START_LAYER; i < DNNModel._nLayers; i++)
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
	for (int i = G_START_LAYER; i < DNNModel._nLayers; i++)
	{		
		INT64 nTotalFlops = (DNNModel._Replicated[i] == true) ? ((INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP) :((INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP * DNNModel._nWorkers);
		//INT64 nTotalFlops = (INT64)DNNModel._Layers[i]._Connections * NUM_DNN_UNIT_FLOP;
		double avgGFLOPs = (averageSampleTime[i] > 0.0f) ? nTotalFlops/(averageSampleTime[i] * 1e3) : 0.0f;
		printf("%10d %10d %10.2f %10.6f %lld\n", i, DNNModel._nThreads[i], avgGFLOPs, averageSampleTime[i]/(1E3), nTotalFlops);
	}
	fflush(stdout);
	for (int i = 0; i < numThreads; i++)
	{
		tl[i].Fini();
	}
	delete [] sampleCount;
	delete [] averageSampleTime;
	delete [] tl;
}

void runDNNModel(void)
{
	printf("%-10s %-10s %-10s %-10s %-10s\n", "Layers", "Threads", "GFLOP/s", "MSec", "FLOPs");
	runDNNModelThreads(G_THREAD_COUNT, DNN_FORWARD);
	if (G_TRAINING)
	{
		runDNNModelThreads(G_THREAD_COUNT, DNN_BACKWARD);
		runDNNModelThreads(G_THREAD_COUNT, DNN_WEIGHTUPDATE);
	}
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

int main(int argc, char *argv[])
{	
	g_CanonicalConfig.Init();
#if 0
	SetCanonicalConfig(argc, argv, g_CanonicalConfig);
	g_CanonicalConfig.Print();
	LayerConfig* lc = ModelConfig[g_CanonicalConfig._modelType][g_CanonicalConfig._workerCount];
	
	if (lc == NULL) return 0;

	lc->_FeedForwardSparsity = g_CanonicalConfig._feedFowardSparsity;
	lc->_BackPropSparsity = g_CanonicalConfig._backPropSparsity;
	lc->_DeltaComputeSparsity = g_CanonicalConfig._deltaComputeSparsity;
	lc->_WeightUpdateSparsity = g_CanonicalConfig._weightUpdateSparsity;

	DNNModel.Init(ModelLayerCount[(int)G_MODEL_TYPE], lc, G_WORKER_COUNT, G_REPLICATED_OUTPUT_LAYER);
	DNNModel.Print(ModelName[(int)G_MODEL_TYPE]);
	runDNNModel();
	DNNModel.Fini();
#endif
	return 0;
}
