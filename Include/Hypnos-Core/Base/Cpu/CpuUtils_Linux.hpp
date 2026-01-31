#pragma once

#include <pthread.h>
#include <sched.h>

namespace Blanketmen {

inline void CpuInfo::GetAvailableCpuIds()
{
    available_count = 0;
    cpu_set_t set;
    CPU_ZERO(&set);
    if (sched_getaffinity(0, sizeof(set), &set) != 0)
    {
        return;
    }

    for (uint32 i = 0; i < CPU_SETSIZE; ++i)
    {
        if (CPU_ISSET(i, &set) && i < MaxCpuIdCount)
        {
            available_cpu_ids[available_count++] = static_cast<uint32>(i);
        }
    }
}

inline Status<void> CpuUtils::PinThread(uint32 cpu_index) noexcept
{
    if (cpu_index >= static_cast<uint32>(CPU_SETSIZE))
    {
        return Status<void>::Error(ErrorCode::InvalidArgument, "CPU index out of range");
    }

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(static_cast<int>(cpu_index), &set);

    const int result = pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
    return result != 0 ? Status<void>::Error(ErrorCode::IOError, "pthread_setaffinity_np failed") : Status<void>::Success();
}

inline Status<void> CpuUtils::PinThread(Thread& thread, uint32 cpu_index) noexcept
{
    if (cpu_index >= static_cast<uint32>(CPU_SETSIZE))
    {
        return Status<void>::Error(ErrorCode::InvalidArgument, "CPU index out of range");
    }

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(static_cast<int>(cpu_index), &set);

    const int result = pthread_setaffinity_np(thread.native_handle(), sizeof(set), &set);
    return result != 0 ? Status<void>::Error(ErrorCode::IOError, "pthread_setaffinity_np failed") : Status<void>::Success();
}

} // namespace Blanketmen
