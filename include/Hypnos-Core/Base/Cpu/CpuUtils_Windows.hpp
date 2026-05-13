#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace Blanketmen {

inline uint32 CpuUtils::GetAvailableCpuIds(uint32* cpu_ids)
{
    if (cpu_ids == nullptr)
    {
        return 0;
    }

    DWORD_PTR process_mask = 0;
    DWORD_PTR system_mask = 0;
    if (GetProcessAffinityMask(GetCurrentProcess(), &process_mask, &system_mask) == 0)
    {
        return 0;
    }

    uint32 count = 0;
    const uint32 bits = static_cast<uint32>(sizeof(DWORD_PTR) * 8);
    for (uint32 i = 0; i < bits; ++i)
    {
        if ((process_mask & (static_cast<DWORD_PTR>(1) << i)) != 0 && i < MaxCpuIdCount)
        {
            cpu_ids[count++] = i;
        }
    }
    return count;
}

inline Status<void> CpuUtils::PinThread(Thread& thread, uint32 cpu_id) noexcept
{
    if (cpu_id >= static_cast<uint32>(sizeof(DWORD_PTR) * 8))
    {
        return Status<void>::Error(ErrorCode::InvalidArgument, "CPU index out of range");
    }

    const DWORD_PTR mask = static_cast<DWORD_PTR>(1) << cpu_id;
    return SetThreadAffinityMask(thread.native_handle(), mask) == 0
        ? Status<void>::Error(ErrorCode::IOError, "SetThreadAffinityMask failed")
        : Status<void>::Success();
}

inline Status<void> CpuUtils::PinThread(uint32 cpu_id) noexcept
{
    if (cpu_id >= static_cast<uint32>(sizeof(DWORD_PTR) * 8))
    {
        return Status<void>::Error(ErrorCode::InvalidArgument, "CPU index out of range");
    }

    const DWORD_PTR mask = static_cast<DWORD_PTR>(1) << cpu_id;
    return SetThreadAffinityMask(GetCurrentThread(), mask) == 0
        ? Status<void>::Error(ErrorCode::IOError, "SetThreadAffinityMask failed")
        : Status<void>::Success();
}

} // namespace Blanketmen
