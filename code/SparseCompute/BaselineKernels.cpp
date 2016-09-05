#include "Utils.h"
#include "AssemblyHeaders.h"


void BaselineFeedForward(Layer* layer, const float* inputAct, float* outputAct)
{
    for (int i = 0; i < layer->_OutputFeature; i++) {
        for (int j = 0; j < layer->_Input2Height; j++) {
            outputAct[i*layer->_Input2Height + j] = mulsum2_baseline(inputAct + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), layer->_Input2Width);
        }
    }
}

void BaselineBackPropagate(Layer* layer, const float* inputError, float* outputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        for (int j = 0; j < layer->_Input2Height; j++)
        {
            int signalIndex = i*layer->_Input2Height + j;
            float signal = inputError[signalIndex];
            mulsum3_baseline(outputError + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), signal, layer->_Input2Width);
        }
    }
}

void BaselineWeightDeltas_2D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        mulsum3_baseline(weightDeltas + (i*layer->_Input2Width), inputAct, inputError[i], layer->_Input2Width);
    }
}

void BaselineWeightDeltas_3D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++) 
    {
        for (int j = 0; j < layer->_Input2Height; j++) 
        {
            int signalIndex = i*layer->_Input2Height + j;
            float signal = inputError[signalIndex];
            mulsum3_baseline(weightDeltas + (i*layer->_Input2Width), inputAct + (j*layer->_Input2Width), signal, layer->_Input2Width);
        }
    }
}

void BaselineWeightUpdate(Layer* layer, const float* weightDeltas)
{
    mulsum3_baseline(layer->_Weights, weightDeltas, 1.0f, layer->_WeightSize);
}

FeedForward g_FeedFowardKernels[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineFeedForward, nullptr, nullptr, nullptr, nullptr};
BackPropagate g_BackPropagateKernels[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineBackPropagate, nullptr, nullptr, nullptr, nullptr};
ComputeWeightDelta_2D g_ComputeWeightDeltaKernels_2D[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineWeightDeltas_2D, nullptr, nullptr, nullptr, nullptr};
ComputeWeightDelta_3D g_ComputeWeightDeltaKernels_3D[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineWeightDeltas_3D, nullptr, nullptr, nullptr, nullptr};
WeightUpdate g_ComputeWeightUpdateKernels[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineWeightUpdate, nullptr, nullptr, nullptr, nullptr};


void InitDNNKernels(DNNKernels& dnnKernels, const KernelVersion kernel)
{
    dnnKernels = { 
        g_FeedFowardKernels[kernel], 
        g_BackPropagateKernels[kernel], 
        g_ComputeWeightDeltaKernels_2D[kernel], 
        g_ComputeWeightDeltaKernels_3D[kernel],
        g_ComputeWeightUpdateKernels[kernel]    
    };
}
