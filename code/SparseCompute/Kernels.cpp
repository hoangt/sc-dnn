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

void UnrollFeedForward(Layer* layer, const float* inputAct, float* outputAct)
{
    for (int i = 0; i < layer->_OutputFeature; i++) {
        for (int j = 0; j < layer->_Input2Height; j++) {
            outputAct[i*layer->_Input2Height + j] = mulsum2_unroll(inputAct + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), layer->_Input2Width);
        }
    }
}

void SparseFeedForward(Layer* layer, const float* inputAct, float* outputAct)
{
    for (int i = 0; i < layer->_OutputFeature; i++) {
        for (int j = 0; j < layer->_Input2Height; j++) {
            outputAct[i*layer->_Input2Height + j] = mulsum2_sparse(inputAct + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), layer->_Input2Width);
        }
    }
}

void HardwareFeedForward(Layer* layer, const float* inputAct, float* outputAct)
{
    for (int i = 0; i < layer->_OutputFeature; i++) {
        for (int j = 0; j < layer->_Input2Height; j++) {
            outputAct[i*layer->_Input2Height + j] = mulsum2_hardware(inputAct + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), layer->_DenseInput2Width);
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

void UnrollBackPropagate(Layer* layer, const float* inputError, float* outputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        for (int j = 0; j < layer->_Input2Height; j++)
        {
            int signalIndex = i*layer->_Input2Height + j;
            float signal = inputError[signalIndex];
            mulsum3_unroll(outputError + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), signal, layer->_Input2Width);
        }
    }
}

void SparseBackPropagate(Layer* layer, const float* inputError, float* outputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        for (int j = 0; j < layer->_Input2Height; j++)
        {
            int signalIndex = i*layer->_Input2Height + j;
            float signal = inputError[signalIndex];
            if (signal != 0.0f)
            {
                mulsum3_unroll(outputError + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), signal, layer->_Input2Width);
            }
        }
    }
}

void HardwareBackPropagate(Layer* layer, const float* inputError, float* outputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        for (int j = 0; j < layer->_Input2Height; j++)
        {
            int signalIndex = i*layer->_Input2Height + j;
            if (signalIndex < layer->_minSparseSignalWordIndex) 
            {
                if (signalIndex % 16 == 0) 
                {
                    mulsum3_hardware(outputError + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), 0.0f, layer->_Input2Width);
                }
            }
            else if (signalIndex < layer->_minDenseSignalIndex)
            {
                mulsum3_hardware(outputError + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), 0.0f, layer->_Input2Width);
            }
            else 
            {
                mulsum3_unroll(outputError + (j*layer->_Input2Width), layer->_Weights + (i*layer->_Input2Width), inputError[signalIndex], layer->_DenseInput2Width);
            }
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

void UnrollWeightDeltas_2D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        mulsum3_unroll(weightDeltas + (i*layer->_Input2Width), inputAct, inputError[i], layer->_Input2Width);
    }
}

void SparseWeightDeltas_2D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        if (inputError[i] != 0.0f)
        {
            mulsum3_unroll(weightDeltas + (i*layer->_Input2Width), inputAct, inputError[i], layer->_Input2Width);
        }
    }
}

void HardwareWeightDeltas_2D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        if (i < layer->_minSparseSignalWordIndex)
        {
            if (i % 16 == 0)
            {
                mulsum3_hardware(weightDeltas + (i*layer->_Input2Width), inputAct, 0.0f, layer->_Input2Width);
            }
        }
        else if (i < layer->_minDenseSignalIndex)
        {
            mulsum3_hardware(weightDeltas + (i*layer->_Input2Width), inputAct, 0.0f, layer->_Input2Width);
        }
        else
        {
            mulsum3_unroll(weightDeltas + (i*layer->_Input2Width), inputAct, inputError[i], layer->_Input2Width);
        }
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

void UnrollWeightDeltas_3D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++) 
    {
        for (int j = 0; j < layer->_Input2Height; j++) 
        {
            int signalIndex = i*layer->_Input2Height + j;
            float signal = inputError[signalIndex];
            mulsum3_unroll(weightDeltas + (i*layer->_Input2Width), inputAct + (j*layer->_Input2Width), signal, layer->_Input2Width);
        }
    }
}

void SparseWeightDeltas_3D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++) 
    {
        for (int j = 0; j < layer->_Input2Height; j++) 
        {
            int signalIndex = i*layer->_Input2Height + j;
            float signal = inputError[signalIndex];
            if (signal != 0.0f)
            {
                mulsum3_unroll(weightDeltas + (i*layer->_Input2Width), inputAct + (j*layer->_Input2Width), signal, layer->_Input2Width);
            }
        }
    }
}

void HardwareWeightDeltas_3D(Layer* layer, float* weightDeltas, const float* inputAct, const float* inputError)
{
    for (int i = 0; i < layer->_OutputFeature; i++)
    {
        for (int j = 0; j < layer->_Input2Height; j++)
        {
            int signalIndex = i*layer->_Input2Height + j;
            if (signalIndex < layer->_minSparseSignalWordIndex)
            {
                if (signalIndex % 16 == 0)
                {
                    mulsum3_hardware(weightDeltas + (i*layer->_Input2Width), inputAct + (j*layer->_Input2Width), 0.0f, layer->_Input2Width);
                }
            }
            else if (signalIndex < layer->_minDenseSignalIndex)
            {
                mulsum3_hardware(weightDeltas + (i*layer->_Input2Width), inputAct + (j*layer->_Input2Width), 0.0f, layer->_Input2Width);
            }
            else
            {
                mulsum3_unroll(weightDeltas + (i*layer->_Input2Width), inputAct + (j*layer->_Input2Width), inputError[signalIndex], layer->_Input2Width);
            }

        }
    }
}

void BaselineWeightUpdate(Layer* layer, const float* weightDeltas)
{
    mulsum3_baseline(layer->_Weights, weightDeltas, 1.0f, layer->_WeightSize);
}

void UnrollWeightUpdate(Layer* layer, const float* weightDeltas)
{
    mulsum3_unroll(layer->_Weights, weightDeltas, 1.0f, layer->_WeightSize);
}

void HardwareWeightUpdate(Layer* layer, const float* weightDeltas)
{
    mulsum3_unroll((layer->_Weights + layer->_minDenseDeltaWordIndex), (weightDeltas + layer->_minDenseDeltaWordIndex), 1.0f, layer->_DenseDeltaSize);
}

FeedForward g_FeedFowardKernels[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineFeedForward, &UnrollFeedForward, &SparseFeedForward, &UnrollFeedForward, &HardwareFeedForward };
BackPropagate g_BackPropagateKernels[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineBackPropagate, &UnrollBackPropagate, &SparseBackPropagate, &SparseBackPropagate, &HardwareBackPropagate };
ComputeWeightDelta_2D g_ComputeWeightDeltaKernels_2D[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineWeightDeltas_2D, &UnrollWeightDeltas_2D, &SparseWeightDeltas_2D, SparseWeightDeltas_2D, &HardwareWeightDeltas_2D };
ComputeWeightDelta_3D g_ComputeWeightDeltaKernels_3D[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineWeightDeltas_3D, &UnrollWeightDeltas_3D, &SparseWeightDeltas_3D, SparseWeightDeltas_3D, &HardwareWeightDeltas_3D };
WeightUpdate g_ComputeWeightUpdateKernels[KernelVersion::KERNEL_VERSION_COUNT] = { &BaselineWeightUpdate, &UnrollWeightUpdate, &UnrollWeightUpdate, &UnrollWeightUpdate, &HardwareWeightUpdate };


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
