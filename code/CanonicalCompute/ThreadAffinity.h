#pragma once
#include <windows.h>
#include <stdint.h>
#include <vector>
#include <assert.h>


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

        const char* ToString(const bool value) const
        {
            return value ? "Enabled" : "Disabled";
        }

        void Print(void) const
        {
            printf("ThreadAffinityConfig(%s): HyperThreading:%s LogicalProcessor:%u PhysicalProcessor:%u\n", ToString(_enabled), ToString(_hyperThreadingEnabled), _logicalProcessorCount, _physicalProcessorCount);
        }

        void Configure(const uint32_t threadCount)
        {
			assert(_enabled == true);

            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            _logicalProcessorCount = sysinfo.dwNumberOfProcessors;

            _logicalProcessor.resize(threadCount);
            _affinityMask.resize(threadCount);

            DWORD bufferLength = 0;
            bool logicalProcessorInfo = GetLogicalProcessorInformationEx(LOGICAL_PROCESSOR_RELATIONSHIP::RelationProcessorCore, nullptr, &bufferLength);
            if ((logicalProcessorInfo == false) && (GetLastError() == ERROR_INSUFFICIENT_BUFFER) && (bufferLength > 0))
            {
                PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)malloc(bufferLength);
                logicalProcessorInfo = GetLogicalProcessorInformationEx(LOGICAL_PROCESSOR_RELATIONSHIP::RelationProcessorCore, buffer, &bufferLength);
                if (logicalProcessorInfo == true)
                {
                    DWORD byteOffset = 0;
                    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX ptr = buffer;
                    while ((byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)) <= bufferLength)
                    {
                        if (ptr == nullptr) break;
                        if ((ptr->Relationship == LOGICAL_PROCESSOR_RELATIONSHIP::RelationProcessorCore) && (ptr->Processor.Flags == LTP_PC_SMT))
                        {
                            _hyperThreadingEnabled = true;

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

        DWORD LogicalProcessor(const uint32_t threadId)
        {
            assert(_enabled && threadId < _logicalProcessor.size());
            return _logicalProcessor[threadId];
        }

        DWORD AffinityMask(const uint32_t threadId)
        {
            assert(_enabled && threadId < _affinityMask.size());
            return _affinityMask[threadId];
        }
    };


/* 
	  // Call globally
	   struct ThreadAffinityConfig _trainingThreadAffinity;
	   _trainingThreadAffinity.Configure(threadCount);

	   // Inside each thread
       const DWORD logicalProcessor = _trainingThreadAffinity.LogicalProcessor(threadId);
       const DWORD affinityMask = _trainingThreadAffinity.AffinityMask(threadId);
       SetThreadAffinityMask(GetCurrentThread(), affinityMask);
*/