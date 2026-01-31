#pragma once

namespace Blanketmen {

struct CpuInfo
{
    static constexpr uint32 MaxCpuIdCount = 1024;

    uint32 logical_count;
    uint32 available_count;
    uint32 available_cpu_ids[MaxCpuIdCount];

    bool IsAvailable(uint32 cpu_index) const noexcept
    {
        for (uint32 i = 0; i < available_count; ++i)
        {
            if (available_cpu_ids[i] == cpu_index)
            {
                return true;
            }
        }
        return false;
    }

    void GetLogicalCpuCount()
    {
        logical_count = static_cast<uint32>(Thread::hardware_concurrency());
        if (logical_count == 0)
        {
            logical_count = 1;
        }
    }

    void GetAvailableCpuIds();
};

struct CpuUtils
{
    [[nodiscard]] static CpuInfo Query();
    [[nodiscard]] static Status<void> PinThread(uint32 cpu_index) noexcept;
    [[nodiscard]] static Status<void> PinThread(Thread& thread, uint32 cpu_index) noexcept;
};

inline CpuInfo CpuUtils::Query()
{
    CpuInfo info { };
    info.GetLogicalCpuCount();
    info.GetAvailableCpuIds();
    return info;
}

} // namespace Blanketmen

#if defined(_WIN32)
#include "CpuUtils_Windows.hpp"
#elif defined(__linux__)
#include "CpuUtils_Linux.hpp"
#else
#include "CpuUtils_Unsupported.hpp"
#endif