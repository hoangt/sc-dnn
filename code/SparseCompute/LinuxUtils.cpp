#include "Utils.h"
#include <iostream>
#include <vector>
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

long numOnlineCPUs = sysconf(_SC_NPROCESSORS_ONLN);

void SetTrainingThreadAffinity(int threadNum)
{
#ifndef M5_BUILD 
  cpu_set_t cpuset;
  
  CPU_ZERO(&cpuset);
  CPU_SET((threadNum % numOnlineCPUs), &cpuset);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
#endif

}

static struct option long_options[] = {
  {"samples", required_argument, 0, 's'},
  {"threads", required_argument, 0, 't'},
  {"workers", required_argument, 0, 'w'},
  {"model", required_argument, 0, 'm'},
  {"partition", no_argument, 0, 'p'},
  {"nodeltaweight", no_argument, 0, 'd'},
  {"startlayer", required_argument, 0, 'l'},
  {"classify", no_argument, 0, 'c'},
  {"affinity", no_argument, 0, 'a'},
  {"forwardsparsity", required_argument, 0, 'f'},
  {"backwardsparsity", required_argument, 0, 'b'},
  {"deltacomputesparsity", required_argument, 0, 'e'},
  {"weightupdatesparsity", required_argument, 0, 'u'},
  {"signalcachelinesparsity", required_argument, 0, 'g'},
  {"activationcachelinesparsity", required_argument, 0, 'v'},
  {"deltacachelinesparsity", required_argument, 0, 'y'},
  {"kernel", required_argument, 0, 'k'},
  {"pass", required_argument, 0, 'h'},
  { "outputscale", required_argument, 0, 'x' },
  { "threadmodel", required_argument, 0, 'z' },
  {"usemainthread", no_argument, 0, 'q'},
  {0, 0, 0, 0}
};

void SetCanonicalConfig(int argc, char* argv[], CanonicalConfig& config)
{
  while (1) {
    int long_option_index = 0;
    int c = getopt_long_only(argc, argv, "", long_options, &long_option_index);
    if (c == -1) {
      break;
    }
    switch (c) {
    case 'q':
	config._useMainThread = true;
	break;
    case 'x':
        config._outputScale = atoi(optarg);
        break;
    case 'z':
        config._threadModel = atoi(optarg);
        break;
    case 'h':
      ProcessPassParam(optarg);
      break;
   case 'v':
	config._activationCacheLineSparsity = atoi(optarg); 
	break;
   case 'g':
      config._signalCacheLineSparsity = atoi(optarg);
      break;
    case 'y':
      config._deltaCacheLineSparsity = atoi(optarg);
      break;
    case 's':
      config._sampleCount = atoi(optarg);
      break;
    case 't':
      config._threadCount = atoi(optarg);
      break;
    case 'w':
      config._workerCount = atoi(optarg);
      break;
    case 'm':
      config._modelType = ProcessModelParam(optarg);
      break;
    case 'p':
      config._replicatedOutputLayer = false;
      break;
    case 'd':
      config._deltaWeightOpt = false;
      break;
    case 'l':
      {
	int layer = atoi(optarg);
	config._startLayer = (layer < ModelLayerCount[(int)config._modelType]) ? layer : DEFAULT_START_LAYER;
      }
      break;
    case 'c':
      config._training = false;
      break;
    case 'a':
      config._affinity = true;
      break;
    case 'f':
      config._forwardSparsity = atoi(optarg);
      break;
    case 'b':
      config._backwardSparsity = atoi(optarg);
      break;
    case 'e':
      config._deltaComputeSparsity = atoi(optarg);
      break;
    case 'u':
      config._weightUpdateSparsity = atoi(optarg);
      break;
    case 'k':
      {
        int kernelVersion = atoi(optarg);
	assert (kernelVersion < (int)KernelVersion::KERNEL_VERSION_COUNT);
	config._kernelVersion = static_cast<KernelVersion>(kernelVersion);
      }
      break;
    default:
      printf("%c: **No Match**\n", c);
    }
  }
}

static void* s_DNNModelThreadForward(void *arg)
{
  DNNModelThreadForward((ThreadLayerState*) arg);
  return NULL;
}

static void* s_DNNModelThreadBackward(void *arg)
{
  DNNModelThreadBackward((ThreadLayerState*) arg);
  return NULL;
}

static void* s_DNNModelThreadWeightUpdate(void *arg)
{
  if (G_DELTA_WEIGHT_OPT) {
    DNNModelThreadDeltaWeightUpdate((ThreadLayerState*) arg);
  }
  else {
    DNNModelThreadWeightUpdate((ThreadLayerState*) arg);
  }
  return NULL;
}

void DoModelComputeWithMainThread(const int numThreads, ThreadLayerState *tl, const DNNPass dp)
{
	const int numWorkerThreads = numThreads - 1;
	pthread_t* helperThreads = new pthread_t[numWorkerThreads];
	int* errors = new int[numWorkerThreads];

	for (int i = 0; i < numWorkerThreads; i++) {

		if (dp == DNN_FORWARD)
			errors[i] = pthread_create(&helperThreads[i], NULL, &s_DNNModelThreadForward, (void*)(tl + i));
		else if (dp == DNN_BACKWARD)
			errors[i] = pthread_create(&helperThreads[i], NULL, &s_DNNModelThreadBackward, (void*)(tl + i));
		else
			errors[i] = pthread_create(&helperThreads[i], NULL, &s_DNNModelThreadWeightUpdate, (void*)(tl + i));
	}

	if (dp == DNN_FORWARD)
	{
		s_DNNModelThreadForward((void*)(tl + numWorkerThreads));
	}
	else if (dp == DNN_BACKWARD)
	{
		s_DNNModelThreadBackward((void*)(tl + numWorkerThreads));
	}
	else
	{
		s_DNNModelThreadWeightUpdate((void*)(tl + numWorkerThreads));
	}

	for (int i = 0; i < numWorkerThreads; i++) {
		if (errors[i] == 0) {
			errors[i] = pthread_join(helperThreads[i], NULL);
		}
	}
}

void DoModelComputeWithoutMainThread(const int numThreads, ThreadLayerState *tl, const DNNPass dp)
{
  pthread_t* helperThreads = new pthread_t [numThreads];
  int* errors = new int[numThreads];

  for (int i = 0; i < numThreads; i++) {
    
    if (dp == DNN_FORWARD)
      errors[i] = pthread_create(&helperThreads[i], NULL, &s_DNNModelThreadForward, (void*) (tl+i));
    else if (dp == DNN_BACKWARD)
      errors[i] = pthread_create(&helperThreads[i], NULL, &s_DNNModelThreadBackward, (void*) (tl+i));
    else 
      errors[i] = pthread_create(&helperThreads[i], NULL, &s_DNNModelThreadWeightUpdate, (void*) (tl+i));
  }
  
  for (int i = 0; i < numThreads; i++) {
    if (errors[i] == 0) {
      errors[i] = pthread_join(helperThreads[i], NULL);
    }
  }
}

void DoModelCompute(const int numThreads, ThreadLayerState *tl, const DNNPass dp)
{
	if (G_USE_MAIN_THREAD)
		DoModelComputeWithMainThread(numThreads, tl, dp);
	else
		DoModelComputeWithoutMainThread(numThreads, tl, dp);
}
