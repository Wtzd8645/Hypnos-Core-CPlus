#pragma once

#include <pthread.h>
#include <sched.h>

namespace Blanketmen {

inline uint32 CpuUtils::GetAvailableCpuIds(uint32* cpu_ids)
{
    if (cpu_ids == nullptr)
    {
        return 0;
    }

    cpu_set_t set;
    CPU_ZERO(&set);
    if (sched_getaffinity(0, sizeof(set), &set) != 0)
    {
        return 0;
    }

    uint32 count = 0;
    for (uint32 i = 0; i < CPU_SETSIZE; ++i)
    {
        if (CPU_ISSET(i, &set) && i < MaxCpuIdCount)
        {
            cpu_ids[count++] = i;
        }
    }
    return count;
}

inline Status<void> CpuUtils::PinThread(uint32 cpu_id) noexcept
{
    if (cpu_id >= static_cast<uint32>(CPU_SETSIZE))
    {
        return Status<void>::Error(ErrorCode::InvalidArgument, "[CPU] Pin thread failed. CPU ID exceeds CPU_SETSIZE limit.");
    }

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(static_cast<int>(cpu_id), &set);

    const int result = pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
    return result != 0 ? Status<void>::Error(ErrorCode::IOError, "[CPU] pthread_setaffinity_np failed.") : Status<void>::Success();
}

inline Status<void> CpuUtils::PinThread(Thread& thread, uint32 cpu_id) noexcept
{
    if (cpu_id >= static_cast<uint32>(CPU_SETSIZE))
    {
        return Status<void>::Error(ErrorCode::InvalidArgument, "[CPU] Pin thread failed. CPU ID exceeds CPU_SETSIZE limit.");
    }

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(static_cast<int>(cpu_id), &set);

    const int result = pthread_setaffinity_np(thread.native_handle(), sizeof(set), &set);
    return result != 0 ? Status<void>::Error(ErrorCode::IOError, "[CPU] pthread_setaffinity_np failed.") : Status<void>::Success();
}

} // namespace Blanketmen
